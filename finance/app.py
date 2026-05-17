import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    """ stock name, no, current price, total """
    db.execute("DELETE FROM portfolios")
    symbols = db.execute("SELECT (symbol) FROM transactions WHERE userid = ? GROUP BY symbol", session["user_id"])
    qtys = db.execute("SELECT sum(qty) as qty FROM transactions WHERE userid = ? AND symbol in (SELECT (symbol) FROM transactions WHERE userid = ? GROUP BY symbol) GROUP BY symbol", session["user_id"], session["user_id"])
    for symbol , qty in zip(symbols, qtys):
        stock = lookup(symbol["symbol"])
        t = round(qty["qty"] * stock["price"],2)
        db.execute("INSERT INTO portfolios (userid,symbol,name,price,qty,total) VALUES (?,?,?,?,?,?)",session["user_id"], symbol["symbol"], stock["name"],stock["price"],qty["qty"],t)
    data = db.execute("SELECT * FROM portfolios WHERE qty")
    result = db.execute("SELECT SUM(total) FROM portfolios")[0]['SUM(total)']
    if result is None:
        shareTotal = 0
    else:
        shareTotal = round(result, 2)
    cash = round(db.execute("SELECT cash FROM users WHERE id=?",session["user_id"])[0]['cash'],2)
    grandTotal = round(shareTotal+cash,2)
    return render_template("index.html", data=data, cash=cash,grandTotal=grandTotal, shareTotal=shareTotal)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == 'POST':
        symbol = request.form.get("symbol")
        if lookup(symbol) is None:
            return apology("Invalid symbol")
        shares = int(request.form.get("shares"))
        if not isinstance(request.form.get("shares"), int) and not shares > 0:
            return apology("Invalid buy amount.")
        price = lookup(symbol)["price"];
        amt = price * shares
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']
        if amt > cash:
            return apology("Insufficient cash balance")
        db.execute("INSERT INTO transactions(type,userID, symbol, price, qty,total) VALUES ('BUY',?,?,?,?,?)",session["user_id"], symbol, price, shares,round(amt,2))
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", amt, session["user_id"])
        return redirect("/")

    else:
        return render_template('buy.html')


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    data = db.execute("SELECT * FROM transactions WHERE userid = ?",session["user_id"])
    return render_template('history.html', data=data)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""
    db.execute("DELETE FROM portfolios")

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

#find how to append row and how to see the error of lookup
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if lookup(request.form.get("symbol")) != None:
            quotes = lookup(request.form.get("symbol"))
        else:
            return apology("incorrect symbol")
        return render_template("quoted.html", quotes=quotes)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)
        if not request.form.get("confirmation"):
            return apology("please confirm password", 403)
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match")
        hash = generate_password_hash(request.form.get("password"))

        try:
            db.execute("INSERT INTO users (username,hash) values (?,?)", request.form.get("username"), hash)
        except ValueError:
            return apology("username already in use")
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if lookup(symbol) is None:
            return apology("Invalid symbol")
        shares = int(request.form.get("shares"))
        if not isinstance(request.form.get("shares"), int) and not shares > 0:
            return apology("Invalid amount.")
        owned_shares = db.execute("SELECT sum(qty) FROM portfolios WHERE symbol=?",symbol)[0]['sum(qty)']
        if owned_shares is None:
            return apology("Insufficient shares owned")
        if owned_shares < shares:
            return apology("Insufficient shares owned")
        price = lookup(symbol)["price"]
        amt = price * shares
        db.execute("INSERT INTO transactions(type,userID, symbol, price, qty,total) VALUES ('SELL',?,?,?,?,?)",session["user_id"], symbol, price, -shares,round(amt))
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", amt, session["user_id"])
        return redirect("/")
    else:
        return render_template("sell.html")


@app.route("/cash", methods=["GET", "POST"])
@login_required
def add_cash():
    if request.method == 'POST':
        type = request.form.get("type")
        amt = request.form.get("amt")
        if not amt:
            return apology("Invalid amount")
        try:
            amt = float(amt)
        except ValueError:
            return apology("Invalid amount")
        if amt <= 0:
            return apology("Invalid amount")
        if type == "Add":
            db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", amt, session['user_id'])
            return redirect("/")
        elif type == "Withdraw":
            if amt > db.execute("SELECT cash FROM users WHERE id = ?" , session["user_id"])[0]["cash"]:
                return apology("Insufficient account balance")
            else:
                db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", amt, session['user_id'])
                return redirect("/")
        else: return apology("uh-oh")
    return render_template("cash.html")
