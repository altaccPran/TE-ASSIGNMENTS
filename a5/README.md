# Assignment 5 — Servlet + MySQL `ebookshop` lab

This folder contains a **manual Tomcat deployment** (no Eclipse project files): a small web app **`MyBookApp`** that lists rows from a MySQL table **`ebookshop`** using a **`DisplayBooksServlet`** and JDBC.

## What was implemented

- **`setup.sql`**: Creates database `ebookshop`, table **`ebookshop`** with columns `book_id`, `book_title`, `book_author`, `book_price`, `quantity`, and inserts **six** sample rows.
- **`MyBookApp/index.html`**: Static home page with a **link** that issues an **HTTP GET** to the servlet URL `displayBooks`.
- **`MyBookApp/WEB-INF/web.xml`**: Deployment descriptor: maps **`DisplayBooksServlet`** to **`/displayBooks`** and passes **JDBC init parameters** (`jdbcUrl`, `jdbcUser`, `jdbcPassword`).
- **`MyBookApp/WEB-INF/classes/DisplayBooksServlet.java`**: Extends **`HttpServlet`**, overrides **`doGet`**, uses **`PrintWriter`** to send HTML, runs **`SELECT * FROM ebookshop`**, prints an HTML **table**, and handles **`SQLException`** / missing driver without crashing silently (message in page + **`System.err`**).
- **`javax.servlet.*`** APIs (Tomcat **8.x / 9.x**). For Tomcat **10+**, packages are **`jakarta.servlet.*`** — use Tomcat 9 or adapt imports and deployment.

## Prerequisites

- **JDK** (e.g. 11 or 17) with **`javac`** on your **`PATH`** (not only a browser JRE from java.com).
- **Apache Tomcat 9** (or 8.5) with **`javax.servlet`** — note **`CATALINA_HOME`** (see [What can go wrong](#what-can-go-wrong-with-mysql-java-and-tomcat) below).
- **MySQL Server** (8.x)
- **MySQL Connector/J** JAR in **`MyBookApp/WEB-INF/lib/`**

**Windows:** use **`%CATALINA_HOME%`** (folder where Tomcat’s **`bin`**, **`lib`**, **`webapps`** live).

**macOS (Homebrew):** Tomcat is usually **not** under `~/apache-tomcat-...`. Use:

```bash
export CATALINA_HOME="$(brew --prefix tomcat@9)/libexec"
```

Confirm **`$CATALINA_HOME/lib/servlet-api.jar`** exists. Deploy/copy **`MyBookApp`** to **`$CATALINA_HOME/webapps/`**. Start with **`brew services start tomcat@9`** (or **`catalina run`**).

## 1. Create the database

1. Copy `setup.sql` to a folder you can run from, or use the full path.
2. From **Command Prompt** (adjust user if needed):

   ```bat
   mysql -u root -p < C:\path\to\TE-ASSIGNMENTS\a5\setup.sql
   ```

3. Verify:

   ```bat
   mysql -u root -p -e "USE ebookshop; SELECT * FROM ebookshop;"
   ```

## 2. Folder structure under Tomcat `webapps`

Copy the entire **`MyBookApp`** directory into Tomcat’s webapps folder so you have:

```text
%CATALINA_HOME%\webapps\MyBookApp\index.html
%CATALINA_HOME%\webapps\MyBookApp\WEB-INF\web.xml
%CATALINA_HOME%\webapps\MyBookApp\WEB-INF\classes\DisplayBooksServlet.class
%CATALINA_HOME%\webapps\MyBookApp\WEB-INF\lib\mysql-connector-j-8.x.x.jar
```

- **`WEB-INF/lib`**: Place **one** Connector/J JAR here (see `WEB-INF/lib/README.txt`). Tomcat loads it at runtime.
- **`WEB-INF/classes`**: Compiled servlet **`.class`** file goes here (same folder as the `.java` if you compile in place).

## 3. Edit JDBC configuration

Open **`WEB-INF/web.xml`** in `MyBookApp` and set **`jdbcPassword`** (and **`jdbcUser`** / **`jdbcUrl`** if your MySQL is not `localhost` or uses a different database name).

## 4. Compile the servlet on Windows

Open **Command Prompt**, `cd` to the **`MyBookApp`** folder inside `webapps` (the folder that contains `WEB-INF`).

Use **`;`** (semicolon) to separate classpath entries on Windows. Include **Tomcat’s `servlet-api.jar`** and the **MySQL Connector/J** JAR so JDBC classes resolve:

```bat
cd /d C:\path\to\apache-tomcat-9.x.x\webapps\MyBookApp

javac -encoding UTF-8 -cp "C:\path\to\apache-tomcat-9.x.x\lib\servlet-api.jar;C:\path\to\mysql-connector-j-8.x.x.jar" -d WEB-INF\classes WEB-INF\classes\DisplayBooksServlet.java
```

- **`-cp`**: `servlet-api.jar` is from **`%CATALINA_HOME%\lib\servlet-api.jar`**. The MySQL JAR path must match where you downloaded the connector.
- **`-d WEB-INF\classes`**: writes **`DisplayBooksServlet.class`** into `WEB-INF\classes`.

If `javac` reports “cannot find symbol” for `javax.servlet`, fix the path to **`servlet-api.jar`**.

## 4b. Compile the servlet on macOS / Linux

From the **`MyBookApp`** directory (contains **`WEB-INF`**):

- Use **`:`** (colon) between classpath entries — **not** `;` (that is Windows).
- Set **`CATALINA_HOME`** to the Tomcat install that contains **`lib/servlet-api.jar`** (for Homebrew Tomcat 9, use **`$(brew --prefix tomcat@9)/libexec`**).
- Put OpenJDK on **`PATH`** (e.g. Homebrew **`openjdk@17`**: **`/opt/homebrew/opt/openjdk@17/bin`**).

```bash
export CATALINA_HOME="$(brew --prefix tomcat@9)/libexec"
export PATH="/opt/homebrew/opt/openjdk@17/bin:$PATH"

javac -encoding UTF-8 \
  -cp "$CATALINA_HOME/lib/servlet-api.jar:/path/to/MyBookApp/WEB-INF/lib/mysql-connector-j-9.x.x.jar" \
  -d WEB-INF/classes \
  WEB-INF/classes/DisplayBooksServlet.java
```

If **`/usr/libexec/java_home`** prints “Unable to locate a Java Runtime”, Homebrew’s JDK may be **keg-only**. Either add **`openjdk@17/bin`** to **`PATH`** as above, or symlink the JDK into **`/Library/Java/JavaVirtualMachines/`** (see **`brew info openjdk@17`**).

## 5. Start Tomcat and test

1. Start Tomcat: **Windows** — `bin\startup.bat` or your service; **macOS (Homebrew)** — `brew services start tomcat@9` (or `$(brew --prefix tomcat@9)/bin/catalina run`).
2. Browser:

   - Home: `http://localhost:8080/MyBookApp/`
   - Servlet: `http://localhost:8080/MyBookApp/displayBooks`

You should see an HTML table of books.

After changing **`.java`**, recompile, copy/sync **`MyBookApp`** into **`webapps`** again, and restart Tomcat if needed.

## What can go wrong with MySQL, Java, and Tomcat

These are the issues that most often block this lab.

### MySQL

| Error / symptom | Likely cause | What to do |
|-----------------|--------------|------------|
| **`ERROR 2002 (HY000): Can't connect ... through socket ...`** | MySQL **server** not running (or wrong socket). | Start MySQL (e.g. **`brew services start mysql`** on Mac). Or connect with **`-h 127.0.0.1`** if socket path differs. |
| **`ERROR 1045 (28000): Access denied for user 'root'@'localhost'`** | Wrong password for **`root`**, or **`root`** requires a password you did not use. | Use the password you set at install, or connect the way your machine allows (e.g. **`mysql -u root`** with no **`-p`** if that works). Put the **same** user/password in **`web.xml`** **`jdbcUser`** / **`jdbcPassword`**. |
| Servlet shows “Database error” | DB not created, wrong credentials, or MySQL down. | Run **`setup.sql`**, verify with **`SELECT * FROM ebookshop`**, match **`web.xml`** to how you log in with **`mysql`**. |

### Java / `javac`

| Error / symptom | Likely cause | What to do |
|-----------------|--------------|------------|
| **“Unable to locate a Java Runtime”** when running **`javac`** / **`java`** | No **JDK** on **`PATH`**, or only a stub. | Install a **JDK** (e.g. Homebrew **`openjdk@17`**) and add **`.../bin`** to **`PATH`**. |
| **`package javax.servlet does not exist`** during **`javac`** | **`servlet-api.jar`** not on **`-cp`**, usually because **`CATALINA_HOME`** is **wrong or empty**. | On **Windows**, point **`-cp`** at **`%CATALINA_HOME%\lib\servlet-api.jar`**. On **Mac (Homebrew Tomcat)**, do **not** guess **`~/apache-tomcat-...`** — use **`export CATALINA_HOME="$(brew --prefix tomcat@9)/libexec"`** and verify **`ls "$CATALINA_HOME/lib/servlet-api.jar"`**. |
| **`/usr/libexec/java_home` fails** on macOS | Homebrew JDK is **keg-only** (not registered for Apple’s tool). | Set **`PATH`** to Homebrew’s **`openjdk.../bin`**, or symlink the **`.jdk`** bundle into **`/Library/Java/JavaVirtualMachines/`** (see **`brew info openjdk@17`**). |

### Tomcat / deployment

| Error / symptom | Likely cause | What to do |
|-----------------|--------------|------------|
| **404** on **`/MyBookApp/...`** | App not under **`webapps`**, or wrong context name. | Copy **`MyBookApp`** to **`$CATALINA_HOME/webapps/`** (Homebrew: **`libexec/webapps`**). URL must match folder name (**`/MyBookApp/`**). |
| **404** on **`displayBooks`** | **`web.xml`** typo, or old deployment without servlet. | Check **`<url-pattern>/displayBooks</url-pattern>`** and restart Tomcat. |
| **500** / HTML “Database error” | JDBC URL, user, password, or MySQL not reachable. | Fix **`web.xml`** init-params; ensure MySQL listens on **`localhost:3306`**. |
| **`ClassNotFoundException: com.mysql.cj.jdbc.Driver`** at runtime | Connector JAR missing from deployed app. | Ensure **`WEB-INF/lib/mysql-connector-....jar`** is inside the **deployed** **`MyBookApp`**, then **restart Tomcat**. |

## Troubleshooting (short)

| Symptom | What to check |
|--------|----------------|
| **404** on `/MyBookApp/displayBooks` | `web.xml` mapping, `MyBookApp` folder name under **`webapps`**, Tomcat logs |
| **500** / “Database error” | MySQL running, `setup.sql` applied, **user/password** in `web.xml`, JDBC URL |
| **ClassNotFoundException: com.mysql.cj.jdbc.Driver** | Connector JAR missing under **`WEB-INF/lib`**, restart Tomcat |
| **`javac`**: `javax.servlet` missing | **`CATALINA_HOME`** and **`servlet-api.jar`** path on **`-cp`** (see table above) |

## Theory (short — for your report)

- A **Servlet** is a Java class deployed in a **web container** (Tomcat) that handles HTTP requests; **`HttpServlet`** provides **`doGet`**, **`doPost`**, etc.
- The container loads the class, calls **`init`** once, then **`service`** → **`doGet`** per request, and **`destroy`** on shutdown.
- This lab uses **`doGet`** and **`PrintWriter`** to return **dynamic HTML** built from a **JDBC** `ResultSet`.

## References

- Oracle / Jakarta Servlet API (as bundled with Tomcat)
- MySQL Connector/J documentation
