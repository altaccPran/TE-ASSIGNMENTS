# Assignment 5 — Servlet + Hosted MySQL (`ebookshop`)

This project is a **manual Tomcat deployment** of a Java Servlet app (`MyBookApp`) that connects to a **hosted MySQL database** and displays data from the `ebookshop` table.

---

## ✅ Environment

- **OS:** Ubuntu
- **Tomcat:** Manual install at `/opt/tomcat` (NOT system service)
- **MySQL:** Hosted / Remote
- **DB Name:** `te31435_db`
- **Table Name:** `ebookshop`

---

## 📦 Project Structure

```text
/opt/tomcat/webapps/MyBookApp/
│── index.html
│── WEB-INF/
│   ├── web.xml
│   ├── classes/
│   │   └── DisplayBooksServlet.class
│   └── lib/
│       └── mysql-connector-j-8.x.x.jar
```

---

## 1. Database Setup (Hosted DB)

⚠️ **You cannot create databases. Use the assigned one.**

### `setup.sql`

```sql
USE te31435_db;

DROP TABLE IF EXISTS ebookshop;

CREATE TABLE ebookshop (
    book_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    book_title VARCHAR(200) NOT NULL,
    book_author VARCHAR(120) NOT NULL,
    book_price DECIMAL(10, 2) NOT NULL,
    quantity INT NOT NULL
);

INSERT INTO ebookshop (book_title, book_author, book_price, quantity) VALUES
('Clean Code', 'Robert C. Martin', 42.99, 12),
('Effective Java', 'Joshua Bloch', 54.50, 8),
('Head First Servlets', 'Basham et al.', 39.99, 5),
('MySQL Cookbook', 'DuBois', 49.00, 3),
('Java Servlet Programming', 'Hunter', 45.00, 7),
('Database Systems', 'Connolly & Begg', 62.25, 4);
```

### Run SQL

```bash
mysql -h 10.10.8.119 -u te31435 -p te31435_db < setup.sql
```

---

## 2. Configure JDBC (`web.xml`)

Add the following initialization parameters to your `web.xml`:

```xml
<init-param>
    <param-name>jdbcUrl</param-name>
    <param-value>jdbc:mysql://10.10.8.119:3306/te31435_db?useSSL=false&amp;allowPublicKeyRetrieval=true&amp;serverTimezone=UTC</param-value>
</init-param>

<init-param>
    <param-name>jdbcUser</param-name>
    <param-value>te31435</param-value>
</init-param>

<init-param>
    <param-name>jdbcPassword</param-name>
    <param-value>te31435</param-value>
</init-param>
```

---

## 3. Compile Servlet

Navigate to your app directory and compile the Java file:

```bash
cd /opt/tomcat/webapps/MyBookApp

sudo javac -encoding UTF-8 \
  -cp "/opt/tomcat/lib/servlet-api.jar:/opt/tomcat/webapps/MyBookApp/WEB-INF/lib/mysql-connector-j-9.6.0.jar" \
  -d WEB-INF/classes \
  WEB-INF/classes/DisplayBooksServlet.java
```

---

## 4. Run Tomcat

**Enter sudo mode (root shell):**
```bash
sudo -i
```

**Start / Restart Tomcat:**
```bash
/opt/tomcat/bin/shutdown.sh
/opt/tomcat/bin/startup.sh
```

---

## 5. Access Application

Open your browser and navigate to:
- **Home:** [http://localhost:8080/MyBookApp/](http://localhost:8080/MyBookApp/)
- **Servlet:** [http://localhost:8080/MyBookApp/displayBooks](http://localhost:8080/MyBookApp/displayBooks)

---

## 6. Verify Database

Connect to the remote database to ensure your tables are correct:

```bash
mysql -h 10.10.8.119 -u te31435 -p te31435_db
```

```sql
SHOW TABLES;
SELECT * FROM ebookshop;
```

---

## ⚠️ Common Errors

- **`ClassNotFoundException` (JDBC driver missing)**
  Ensure the `.jar` file is correctly placed:
  ```bash
  cp mysql-connector-j-8.x.x.jar /opt/tomcat/webapps/MyBookApp/WEB-INF/lib/
  ```

- **HTTP 500 (DB error)**
  - Wrong credentials in `web.xml`
  - DB not reachable (check network/VPN)

- **HTTP 404**
  - Wrong folder name in `webapps`
  - Wrong servlet mapping in `web.xml`

- **Permission denied (`/opt/tomcat`)**
  Fix ownership with:
  ```bash
  sudo chown -R $USER:$USER /opt/tomcat
  ```

---

## 🔍 Logs

To troubleshoot, check the Tomcat output logs:
```bash
tail -f /opt/tomcat/logs/catalina.out
```

---

## ❌ Do NOT

- Do **NOT** use a localhost DB.
- Do **NOT** use the `root` user for the database.
- Do **NOT** create the `ebookshop` database yourself (use `te31435_db`).
- Do **NOT** use `systemctl tomcat9`.

---

## ✅ Summary

- **DB:** Remote (`10.10.8.119`)
- **Database:** `te31435_db`
- **Table:** `ebookshop`
- **Server:** Manual Tomcat (`/opt/tomcat`)

---

## Extra (clear, separate)

### 🔹 Enter sudo shell
```bash
sudo -i
```

### 🔹 Exit sudo shell
```bash
exit
```

### 🔹 Java compile command (standalone)
```bash
javac -encoding UTF-8 \
  -cp "/opt/tomcat/lib/servlet-api.jar:/opt/tomcat/webapps/MyBookApp/WEB-INF/lib/mysql-connector-j-8.x.x.jar" \
  -d WEB-INF/classes \
  WEB-INF/classes/DisplayBooksServlet.java
```
