# Assignment 7 — PHP + MySQL Employee Directory (macOS)

This folder contains a small **single-page CRUD** app: **Employee Directory** using **PHP** (MySQLi **prepared statements**) and **MySQL**. The code does **not** depend on XAMPP specifically — only on **PHP with mysqli** and a **MySQL server** reachable from [`db_connect.php`](EmployeeApp/db_connect.php) (default: `127.0.0.1`, user `root`, empty password, database `company_db`).

## Files

| File | Role |
|------|------|
| [`EmployeeApp/setup.sql`](EmployeeApp/setup.sql) | Creates **`company_db`**, table **`employees`** (`id`, `name`, `email`, `department`, `designation`), and **3** sample rows. |
| [`EmployeeApp/db_connect.php`](EmployeeApp/db_connect.php) | Opens one **MySQLi** connection; exits with a clear HTML error if the DB is unreachable. |
| [`EmployeeApp/index.php`](EmployeeApp/index.php) | **List** all employees; **Add** form; **Edit** via `?edit=id`; **Delete** with confirm; redirects after POST (PRG). |

## Prerequisites

- **PHP** (with **mysqli** extension) and **MySQL** (or MariaDB).
- A web browser.

Pick **one** setup below (XAMPP, Homebrew, or MAMP).

---

## Option A — Homebrew (no XAMPP; good for Apple Silicon)

Useful when you prefer **CLI installs** or XAMPP does not match your Mac.

### 1. Install PHP and MySQL

```bash
brew install php mysql
brew services start mysql
```

### 2. Create the database

```bash
mysql -u root -p < /path/to/EmployeeApp/setup.sql
```

Use **`-p`** only if your `root` user has a password. Homebrew MySQL 8 may show a **temporary root password** on first install — use it once, then set a password you can put in **`db_connect.php`**, or align **`$DB_USER` / `$DB_PASS`** in [`db_connect.php`](EmployeeApp/db_connect.php) with how you actually connect.

### 3. Run PHP’s built-in web server

From inside **`EmployeeApp`**:

```bash
cd /path/to/TE-ASSIGNMENTS/a7/EmployeeApp
php -S localhost:8080
```

### 4. Open the app

**`http://localhost:8080/index.php`**

No Apache required for this lab. Stop the server with **Ctrl+C**.

---

## Option B — XAMPP for macOS

### 1. Install and start services

1. Install **XAMPP** from [Apache Friends](https://www.apachefriends.org/) (macOS installer; use an **Apple Silicon** build from **More downloads** if available, or the Intel build under **Rosetta 2** if needed).
2. Open the **XAMPP** control application.
3. **Start** **Apache** and **MySQL**.

### 2. Deploy files

Copy **`EmployeeApp`** into XAMPP’s document root, usually:

- **`/Applications/XAMPP/xamppfiles/htdocs/EmployeeApp`**, or  
- **`/Applications/XAMPP/htdocs/EmployeeApp`**

You should have `index.php` and `db_connect.php` under that folder.

### 3. Import SQL

Open **`http://localhost/phpmyadmin`**, use **Import** (or the **SQL** tab) to run **`EmployeeApp/setup.sql`**.

Default XAMPP MySQL is often **`root`** with an **empty** password — matches **`db_connect.php`** unless you changed it.

### 4. Open the app

**`http://localhost/EmployeeApp/`** or **`http://localhost/EmployeeApp/index.php`**

---

## Option C — MAMP (GUI bundle)

1. Install **MAMP** (free; prefer an **Apple Silicon** build if offered).
2. Start **Apache** and **MySQL** from the MAMP app (note the **Apache port**, often **8888**).
3. Place **`EmployeeApp`** in MAMP’s **document root** (see **Preferences → Web Server**), e.g. **`/Applications/MAMP/htdocs/EmployeeApp`**.
4. Import **`setup.sql`** via MAMP’s **phpMyAdmin** (URL/port shown in MAMP, e.g. **`http://localhost:8888/phpMyAdmin`**).
5. MAMP often uses **`root` / `root`** for MySQL — **update** **`db_connect.php`** if your credentials differ.
6. Open **`http://localhost:8888/EmployeeApp/`** (adjust host/port to match MAMP).

---

## Other stacks

- **Docker:** Compose **`php` + `mysql`**, mount **`EmployeeApp`**, run **`setup.sql`** once; set **`db_connect.php`** host to the DB service name.
- **Laravel Herd / Valet:** Fine if you already use them; point the site root at **`EmployeeApp`** and ensure MySQL credentials match.

---

## Edit `db_connect.php` when needed

If your MySQL user, password, host, or port differ from the defaults, edit [`EmployeeApp/db_connect.php`](EmployeeApp/db_connect.php):

- **`$DB_HOST`**, **`$DB_USER`**, **`$DB_PASS`**, **`$DB_NAME`**, **`$DB_PORT`**

---

## Troubleshooting (macOS)

| Issue | What to check |
|--------|----------------|
| **Database connection failed** | MySQL running (`brew services list` or XAMPP/MAMP); **`setup.sql`** applied; **`db_connect.php`** matches your user/password. |
| **404 / wrong page** | URL matches your stack (**port 8080** for `php -S`, **8888** for MAMP, **80** for XAMPP). |
| **Duplicate email** on add | **`email`** is **UNIQUE**; use another email or delete the row. |
| **`mysqli` not found** (Homebrew PHP) | Rare; check **`php -m`** for **mysqli**; install/configure PHP as needed. |
| **XAMPP on Apple Silicon** | Prefer an ARM build from Apache Friends, or run the Intel DMG under **Rosetta**. |

---

## Theory (short — for your journal)

- **PHP** runs on the server; the browser receives HTML only.
- **MySQLi prepared statements** (`prepare` / `bind_param`) separate SQL structure from data and reduce **SQL injection** risk.
- **CRUD**: **Create** (insert), **Read** (select), **Update** (update), **Delete** (delete). This app performs all four against **`employees`**.

## References

- PHP manual: MySQLi  
- [Homebrew](https://brew.sh) (PHP, MySQL)  
- [XAMPP](https://www.apachefriends.org/) / [MAMP](https://www.mamp.info/) documentation  
