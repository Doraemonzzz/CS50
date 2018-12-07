import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
import time

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    row = db.execute("SELECT cash FROM users WHERE id = :userid",
                  userid=session["user_id"])

    # 获得cash
    cash = row[0]["cash"]
    # 总金额
    total = cash

    # 获得股票
    stocks = db.execute("SELECT symbol, name, sum(shares) as all_shares FROM tran WHERE id = :userid group by symbol, name having all_shares > 0",
                  userid=session["user_id"])

    # 获得现在的价格
    quote = dict()
    for stock in stocks:
        # 查询价格
        price = lookup(stock["symbol"])["price"]
        # 存入字典
        quote[stock["symbol"]] = price
        # 累加总金额
        total += stock["all_shares"] * price

    return render_template("index.html", quote=quote, stocks=stocks, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # 获得股票
        s1 = request.form.get("symbol")
        quote = lookup(s1)

        if quote == None:
            return apology("Invalid symbol", 400)

        # 获得数量
        shares = request.form.get("shares")

        # 判断是否为整数
        try:
            shares = int(shares)
            if shares <= 0:
                return apology("You should enter positive number", 400)
        except:
            return apology("You should enter positive number", 400)

        price = quote["price"]
        name = quote["name"]
        symbol = quote["symbol"]

        # 增加时间
        t1 = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())

        # 获得现金
        row = db.execute("SELECT cash FROM users WHERE id = :userid",
                  userid=session["user_id"])

        cash = row[0]["cash"]

        # 总共价格
        total = price * shares
        # 新的金额
        new_cash = cash - total

        # 判断价格是否足够
        if new_cash < 0:
            return apology("You don't have enough money!", 400)

        # 更新
        db.execute("UPDATE users SET cash = :cash WHERE id = :userid", cash=new_cash, userid=session["user_id"])
        # 更新历史表
        db.execute("INSERT INTO tran (id, symbol, name, shares, price, time) VALUES(:id, :symbol, :name, :shares, :price, :time)", \
                            id=session["user_id"], symbol=symbol, name=name, shares=shares, price=price, time=t1)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    stocks = db.execute("SELECT * FROM tran WHERE id = :userid",
                  userid=session["user_id"])

    return render_template("history.html", stocks=stocks)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol)

        if quote == None:
            return apology("Invalid symbol", 400)

        return render_template("quoted.html", quote=quote)
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
        name = request.form.get("username")
        password1 = request.form.get("password")
        password2 = request.form.get("confirmation")

        # 判断姓名是否输入
        if not name:
            return apology("Missing username!", 400)

        # 判断密码是否输入
        elif not password1:
            return apology("Missing password!", 400)

        # 判断confirmation是否输入
        elif not password2:
            return apology("Missing confirmation!", 400)

        # 判断两次密码是否一致
        if password1 != password2:
            return apology("Password should be same!", 400)

        hash_pw = generate_password_hash(password1)

        # 查找用户名
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", \
                            username=name, hash=hash_pw)

        # 失败则返回
        if not result:
            return apology("Sorry, username has been used!", 400)

        # Remember which user has logged in
        session["user_id"] = result

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # 获得股票
        s1 = request.form.get("symbol")
        quote = lookup(s1)

        if quote == None:
            return apology("Invalid symbol", 400)

        # 获得数量
        shares = request.form.get("shares")

        # 判断是否为整数
        try:
            shares = int(shares)
            if shares <= 0:
                return apology("You should enter positive number", 400)
        except:
            return apology("You should enter positive number", 400)

        price = quote["price"]
        name = quote["name"]
        symbol = quote["symbol"]
        # 增加时间
        t1 = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())

        # 查询
        row = db.execute("SELECT symbol, sum(shares) as all_shares FROM tran WHERE id = :userid and symbol = :symbol group by symbol",
                  userid=session["user_id"], symbol=symbol)

        # 判断是否能执行
        if not row or row[0]["all_shares"] <= 0 or row[0]["all_shares"] < shares:
            return apology("You can't sell", 400)

        # 总共价格
        delta = price * shares

        # 更新
        db.execute("UPDATE users SET cash = cash + :delta WHERE id = :userid", delta=delta, userid=session["user_id"])
        # 更新历史表
        db.execute("INSERT INTO tran (id, symbol, name, shares, price, time) VALUES(:id, :symbol, :name, :shares, :price, :time)", \
                            id=session["user_id"], symbol=symbol, name=name, shares=-shares, price=price, time=t1)

        return redirect("/")
    else:
        # 获得股票
        stocks = db.execute("SELECT symbol, sum(shares) as all_shares FROM tran WHERE id = :userid group by symbol",
                      userid=session["user_id"])
        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
