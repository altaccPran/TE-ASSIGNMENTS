# Assignment 6 — JSP + MySQL `students_info` lab

This folder implements a small **JavaServer Pages (JSP)** example that runs a **`SELECT *`**-style query on a MySQL table **`students_info`** and renders an HTML table. JDBC runs inside **JSP scriptlets** (`<% %>`), as required for the lab.

## Files

| File | Purpose |
|------|---------|
| `setup.sql` | Creates database **`jsp_lab`**, table **`students_info`** (`stud_id`, `stud_name`, `class`, `division`, `city`), and **six** sample rows. |
| `StudentApp/display_students.jsp` | JSP page: JDBC connection, `SELECT`, loop over results, basic **internal CSS** in `<head>`, **SQLException** / missing-driver handling. |
| `StudentApp/WEB-INF/lib/` | Drop **`mysql-connector-j-*.jar`** here (see `README.txt` inside that folder). |

## Manual deployment on Windows (Apache Tomcat)

### 1. Prerequisites

- **JDK** and **Apache Tomcat** (8.5 / 9.x with **`javax`** APIs).
- **MySQL Server** and **MySQL Connector/J** (JAR file).

### 2. Create the database

From **Command Prompt** (adjust path and user):

```bat
mysql -u root -p < C:\path\to\TE-ASSIGNMENTS\a6\setup.sql
```

Verify:

```bat
mysql -u root -p -e "USE jsp_lab; SELECT * FROM students_info;"
```

### 3. Folder structure under Tomcat `webapps`

Create the app folder next to Tomcat’s other applications:

```text
%CATALINA_HOME%\webapps\StudentApp\
    display_students.jsp
    WEB-INF\
        lib\
            mysql-connector-j-8.x.x.jar
```

- Copy the entire **`StudentApp`** directory from this repo into **`%CATALINA_HOME%\webapps\`** so the path is exactly **`webapps\StudentApp\`**.

### 4. Where to put `mysql-connector-j.jar`

Put **one** Connector/J JAR in:

```text
%CATALINA_HOME%\webapps\StudentApp\WEB-INF\lib\mysql-connector-j-8.x.x.jar
```

**Why `WEB-INF/lib`:** Tomcat adds every JAR in this folder to the **web application’s classpath**, so the JSP can load **`com.mysql.cj.jdbc.Driver`**. This is the usual approach for a single app.

**Alternative (not required):** You can instead copy the same JAR into **`%CATALINA_HOME%\lib\`** (global Tomcat libraries). Then **all** webapps see it. For coursework, **`WEB-INF/lib`** per app is clearer.

**After** adding or replacing the JAR, **restart** Tomcat.

### 5. JDBC settings in the JSP

Open **`display_students.jsp`** and, if needed, edit at the top of the scriptlet:

- **`JDBC_URL`** — host, port, database name (**`jsp_lab`** must match `setup.sql`).
- **`JDBC_USER`** / **`JDBC_PASSWORD`** — must match how you connect with the **`mysql`** client (e.g. empty password for local `root`).

### 6. Start Tomcat and test

1. Run **`%CATALINA_HOME%\bin\startup.bat`** (or your Tomcat service).
2. In the browser, open:

   **`http://localhost:8080/StudentApp/display_students.jsp`**

   - **Context path:** `/StudentApp` (folder name under `webapps`).
   - **JSP name:** `display_students.jsp`.

You should see a styled HTML table of students, or a short error message if the DB or driver is misconfigured.

## Troubleshooting

| Issue | What to check |
|-------|----------------|
| Blank page or Tomcat error page | Tomcat **`logs`**; JSP compile errors often in **`localhost.*.log`**. |
| “MySQL JDBC driver not found” | Connector JAR missing from **`StudentApp/WEB-INF/lib`**, or Tomcat not restarted. |
| SQL / connection errors | MySQL service running; **`setup.sql`** applied; **URL/user/password** in JSP match MySQL. |
| **404** | Wrong URL; folder must be **`webapps/StudentApp`** and file **`display_students.jsp`**. |

## Theory (short — for your journal)

- **JSP** lets you mix **HTML** with **Java** (scriptlets, expressions, declarations). The container translates the JSP into a **servlet**, compiles it, and runs it per request.
- **Servlets** are often used for **control** logic; **JSP** is often used for **presentation** (view). This lab keeps JDBC in the JSP for demonstration; larger apps usually move data access to servlets or DAO classes.
- **JDBC** in a JSP uses the same **`DriverManager`** / **`Connection`** / **`Statement`** / **`ResultSet`** APIs as a normal Java class.

## References

- JSP / servlet documentation bundled with your Tomcat version  
- MySQL Connector/J documentation  
