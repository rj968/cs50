from flask import redirect, session
from functools import wraps

def login_required(f):
 """Decorate routes to require login"""
 @wraps(f)
 def decorated_function(*args,**kwargs):
  if session.get("userID") is None:
   return redirect("/login")
  return f(*args, **kwargs)
 return decorated_function