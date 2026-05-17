import os

import sqlite3
import datetime
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import login_required
# Configure application
app = Flask(__name__)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PSession(app)ERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# SQL QUERIES
# CREATE TABLE users(userID INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE NOT NULL CHECK(LENGTH(username < 40)), hash TEXT NOT NULL);
# CREATE TABLE archives(blogID INTEGER PRIMARY KEY AUTOINCREMENT, timestamp DATETIME NOT NULL, userID INETEGER NOT NULL,blogname TEXT CHECK(LENGTH(blogname) < 50), blog TEXT NOT NULL, FOREIGN KEY (userID) REFERENCES users(userID));
# CREATE INDEX uID ON users(userID);
# CREATE INDEX bID ON archives(blogID);

# Including database
con = sqlite3.connect("archives.db", check_same_thread=False)
db = con.cursor()

@app.after_request
def after_request(response):
 """Ensure responses aren't cached"""
 response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
 response.headers["Expires"] = 0
 response.headers["Pragma"] = "no-cache"
 return response

@app.route("/")
def home():
 """Welcome the user"""
 return render_template("home.html")

@app.route("/blog", methods=['GET','POST'])
@login_required
def blog():
 """Type or edit blog"""

 # submit/edit a blog
 if request.method == 'POST':
  blogID = request.form.get('blogArchive')
  if blogID:
   blog = db.execute("SELECT blog,blogName FROM archives WHERE blogID = ? AND userID =?", (blogID, session["userID"])).fetchall()
   return render_template("blog.html", text=blog[0][0], blogID = blogID, blogName=blog[0][1])
  
  # Updating the database
  elif request.form.get('blog'): 
   blogName = request.form.get("blogName")
   
   # if the blog is editted
   if request.form.get("blogID"):
    if blogName:
     db.execute("UPDATE archives SET blog=?,blogName=?, editStatus=1 WHERE blogID=?",(request.form.get('blog'),blogName,request.form.get('blogID')))
    else:
     db.execute("UPDATE archives SET blog=?, editStatus=1 WHERE blogID=?",(request.form.get('blog'),request.form.get('blogID')))
    con.commit
   # if a new blog is entered
   else:
    if blogName:
     db.execute("INSERT INTO archives(timestamp,userID,blog,blogname) VALUES (datetime('now'),?,?,?)", (session["userID"],request.form.get('blog'),blogName))
    else:
     db.execute("INSERT INTO archives(timestamp,userID,blog,blogname) VALUES (datetime('now'),?,?,datetime('now', 'localtime'))", (session["userID"],request.form.get('blog')))
    con.commit()
   return redirect("/blog")
  
  # if error
  else:
   return render_template("blog.html", error="PLEASE ENTER A BLOG")
 # show default blog editor
 return render_template("blog.html")

@app.route("/archives", methods=['GET','POST'])
@login_required
def archives():
 """Search blogs"""
 # show the recent 10 blogs by default
 if request.method == 'GET': 
  data = db.execute("SELECT blogID, blogname FROM archives WHERE userID = ? ORDER BY blogID DESC LIMIT 10", (session["userID"],))
 
 else:
  # check if user has selected a blog
  ID = request.form.get("ID")
  if ID:
   return render_template("blog.html", ID=ID)
  
  # return the top 10 search result by blog name
  if request.form.get("query"):
   query = '%'+request.form.get("query")+'%'
   data = db.execute("SELECT blogID, blogname FROM archives WHERE userID = ? and blogname LIKE ? LIMIT 10", (session["userID"], query))
  else: 
   data = db.execute("SELECT blogID, blogname FROM archives WHERE userID = ? ORDER BY blogID DESC LIMIT 10", (session["userID"],))
 if(data):
  return render_template("archives.html", data=data.fetchall())
 else:
  return render_template("home.html")

@app.route("/login", methods=["GET","POST"])
def login():
 """Login user"""
 # Clear any previous users
 session.clear()

 if request.method == "POST":
  if not request.form.get("username"):
    return render_template("login.html", error="Please enter a user name")
  elif not request.form.get("password"):
   return render_template("login.html", error="Please enter a password")
  rows = db.execute("SELECT * FROM users WHERE username=?", (request.form.get("username"),)).fetchall()
  if len(rows) != 1 or not check_password_hash(rows[0][2], request.form.get("password")):
   return render_template("login.html", error="Invalid username or password")
  session["userID"] = rows[0][0]
  return redirect("/")
 return render_template("login.html")

@app.route("/logout")
def logout():
 """Log user out"""
 session.clear()
 # db.close()
 return redirect("/")


@app.route("/register", methods=["GET","POST"])
def register():
 """Register user"""
 if request.method == "POST":
  if not request.form.get("username"):
   error = "Please enter a username"
   return render_template("register.html", error = error)
  if not request.form.get("password"):
   error = "Please enter a password"
   return render_template("register.html", error = error)
  if request.form.get("password") != request.form.get("confirmation"):
   error = "Passwords don't match"
   return render_template("register.html", error = error)
  hash = generate_password_hash(request.form.get("password"))
  try:
   db.execute("INSERT INTO users (username,hash) values (?,?)", (request.form.get("username"), hash))
   con.commit()
  except Exception:
   error = "Username already exists"
   return render_template("register.html", error = error)
  return redirect("/login")
 return render_template("register.html")


@app.route("/delete_blog", methods=["POST"])
def delete_blog():
 data = request.get_json()
 blog_id = data["blog_id"]
 
 db.execute("DELETE FROM archives WHERE blogID = ? AND userID = ?", (blog_id, session["userID"],))
 con.commit()
 
 return jsonify(success=True)