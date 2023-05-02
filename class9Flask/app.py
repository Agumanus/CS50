import os, time

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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
    
    id = session["user_id"]
    
    # Look up user's portfolio
    portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ?", id)

    # Look up user's cash
    cashInquiry = db.execute("SELECT cash FROM users WHERE id = ?", id)
    cash = cashInquiry[0]["cash"]

    # Save all data in a list of dictionaries
    stocks = []

    for i in range(len(portfolio)):
        stocks.append({})
        stocks[i]["symbol"] = portfolio[i]["symbol"]
        stocks[i]["number"] = portfolio[i]["number"]
        quote = lookup(portfolio[i]["symbol"])
        stocks[i]["price"] = quote["price"]
        stocks[i]["totalValue"] = portfolio[i]["number"] * quote["price"]

    return render_template("index.html", stocks=stocks, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 403)
        
        # Ensure submitted number is type integer
        elif not request.form.get("shares").isnumeric():
            return apology("number of shares must be a positive integer", 403)
        
        # Ensure number of shares was submitted and is a positive integer
        elif not request.form.get("shares") or int(request.form.get("shares")) < 1:
            return apology("number of shares must be a positive integer", 403)
        
        # Look up stock quote
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if quote == None:
            return apology("incorrect stock symbol", 403)
        
        # Ensure user has sufficient cash
        number = int(request.form.get("shares"))
        pricePerShare = quote['price']
        priceTotal = number * pricePerShare 
        id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", id)
        remainingCash = cash[0]["cash"] - priceTotal

        symbol = quote['symbol']

        if remainingCash < 0:
            return apology("not enough cash", 403)

        # Update cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", remainingCash, id)

        # Updates user's portfolio
        rows = db.execute("SELECT * FROM portfolio WHERE symbol = ? AND user_id = ?", symbol, id)
        if len(rows) != 1:
            db.execute("INSERT INTO portfolio (user_id, symbol, number) VALUES (?, ?, ?)", id, symbol, number)
        else:
            existingNumber = db.execute("SELECT number FROM portfolio WHERE symbol = ? AND user_id = ?", symbol, id)
            updatedNumber = existingNumber[0]["number"] + number
            db.execute("UPDATE portfolio SET number = ? WHERE symbol = ? AND user_id = ?", updatedNumber, symbol, id)

        # Log transaction
        type = "buy"
        
        time_stamp = time.time()
        date_time = datetime.fromtimestamp(time_stamp)
        str_date_time = date_time.strftime("%d-%m-%Y, %H:%M:%S")

        db.execute("INSERT INTO transactions (user_id, type, timestamp, symbol, number, price) VALUES (?, ?, ?, ?, ?, ?)", id, type, str_date_time, symbol, number, pricePerShare)

        return redirect("/") 
    else:
        return render_template("buy.html")
    
    
@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    id = session["user_id"]
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", id)

    return render_template("history.html", transactions=transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 403)
        
        # Look up stock quote
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if quote == None:
            return apology("incorrect stock symbol", 403)
        
        # Present stock quote to user
        name = quote['name']
        price = quote['price']
        symbol = quote['symbol']

        return render_template("quoted.html", name=name, price=price, symbol=symbol)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    
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
        
        # Ensure passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 403)
        
        # Ensure username does not exist
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(rows) != 0:
            return apology("username already exists", 403)
        
        # Hash password
        hashedPW = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)

        # Register Username and hashed password into db
        username = request.form.get("username")
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hashedPW)

        # Redirect user to login form
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 403)
        
        # Ensure submitted number is type integer
        elif not request.form.get("shares").isnumeric():
            return apology("number of shares must be a positive integer", 403)
        
        # Ensure number of shares was submitted and is a positive integer
        elif not request.form.get("shares") or int(request.form.get("shares")) <1:
            return apology("number of shares must be a positive integer", 403)
        
        # Look up stock quote
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if quote == None:
            return apology("incorrect stock symbol", 403)
        
        # Ensure user owns enough shares
        symbol = quote["symbol"]
        id = session["user_id"]
        sharesSell = int(request.form.get("shares"))

        number = db.execute("SELECT number FROM portfolio WHERE symbol = ? AND user_id = ?", symbol, id)
        
        # Ensure user owns shares in question
        if len(number) != 1:
            return apology("you don't own these shares", 403)
        
        number = number[0]["number"]

        if number - sharesSell < 0:
            return apology("not enough shares", 403)
        
        # Update cash
        pricePerShare = quote["price"]
        revenue = sharesSell * pricePerShare
        cashBeforeSell = db.execute("SELECT cash FROM users WHERE id = ?", id)
        cashAfterSell = cashBeforeSell[0]["cash"] + revenue 
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cashAfterSell, id)

        # Updates user's portfolio
        sharesBeforeSell = number
        sharesAfterSell = sharesBeforeSell - sharesSell

        if sharesAfterSell == 0:
            db.execute("DELETE FROM portfolio WHERE symbol = ? AND user_id = ?", symbol, id)
        else:
            db.execute("UPDATE portfolio SET number = ? WHERE symbol = ? AND user_id = ?", sharesAfterSell, symbol, id)
        
        # Log transaction
        type = "sell"
        
        time_stamp = time.time()
        date_time = datetime.fromtimestamp(time_stamp)
        str_date_time = date_time.strftime("%d-%m-%Y, %H:%M:%S")

        db.execute("INSERT INTO transactions (user_id, type, timestamp, symbol, number, price) VALUES (?, ?, ?, ?, ?, ?)", id, type, str_date_time, symbol, sharesSell, pricePerShare)

        return redirect("/") 
    else:
        return render_template("sell.html")
    
@app.route("/reset", methods=["GET", "POST"])
def reset():
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)
        
        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)
        
        # Ensure passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 403)

        # Ensure username does exist
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(rows) != 1:
            return apology("username does not exist", 403)
        
        # Hash password
        hashedPW = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)

        # Update hashed password into db
        username = request.form.get("username")
        db.execute("UPDATE users SET hash = ? WHERE username = ?", hashedPW, username)

        return redirect("/login")
    else:
        return render_template("reset.html")