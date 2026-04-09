# Assignment 8 — Apache Struts 2 Login (Ubuntu + Manual Tomcat)

This app demonstrates a login form with validation using **Struts 2.5** deployed on **manual Tomcat (`/opt/tomcat`)**.

---

## ✅ Environment

- **OS:** Ubuntu
- **Tomcat:** `/opt/tomcat` (manual install, NOT systemctl)
- **Java:** JDK 11 or 17
- **App:** `StrutsLoginApp`

---

## 📦 Project Structure

```text
/opt/tomcat/webapps/StrutsLoginApp/
│── login.jsp
│── welcome.jsp
│── WEB-INF/
│   ├── web.xml
│   ├── classes/
│   │   ├── struts.xml
│   │   └── com/lab/struts/
│   │       ├── LoginAction.java
│   │       └── LoginAction.class
│   └── lib/
│       └── (Struts JARs)
```

---

## 1. Place App in Tomcat

```bash
sudo cp -R StrutsLoginApp /opt/tomcat/webapps/
```

## 2. Add Required JARs

Put ONLY required Struts JARs in:
`/opt/tomcat/webapps/StrutsLoginApp/WEB-INF/lib/`

⚠️ **Do NOT copy the entire Struts distribution blindly.**

## 3. Compile Action Class (Ubuntu)

Navigate to the classes directory:
```bash
cd /opt/tomcat/webapps/StrutsLoginApp/WEB-INF/classes
```

**Build classpath:**
```bash
CP=""
for j in ../lib/*.jar; do
  CP="$CP:$j"
done

CP="${CP#:}:/opt/tomcat/lib/servlet-api.jar"
```

**Compile:**
```bash
javac -encoding UTF-8 -cp "$CP" com/lab/struts/LoginAction.java
```

## 4. Start / Restart Tomcat

```bash
sudo /opt/tomcat/bin/shutdown.sh
sudo /opt/tomcat/bin/startup.sh
```

## 5. Run Application

Open your browser and navigate to:
- [http://localhost:8080/StrutsLoginApp/](http://localhost:8080/StrutsLoginApp/)
- or: [http://localhost:8080/StrutsLoginApp/login.jsp](http://localhost:8080/StrutsLoginApp/login.jsp)

---

## ⚠️ Critical Differences from macOS

| Feature | macOS | Ubuntu |
| :--- | :--- | :--- |
| **Tomcat** | `brew` tomcat | `/opt/tomcat` |
| **Restart** | `brew services restart` | manual scripts |
| **Permissions**| no `sudo` | `sudo` required |
| **Paths** | `CATALINA_HOME` dynamic | fixed `/opt/tomcat` |

---

## 🚨 Common Errors

1. **`ClassNotFoundException`**
   → Missing JAR in `WEB-INF/lib`
2. **404 Error**
   → App not in `/opt/tomcat/webapps`
3. **Struts filter error**
   → Check `web.xml` and `struts.xml` in `WEB-INF/classes`
4. **Validation not working**
   → Ensure package extends: `extends="struts-default"`

---

## 🔍 Logs

To troubleshoot, check the Tomcat output logs:
```bash
tail -f /opt/tomcat/logs/catalina.out
```

---

## ❌ Do NOT

- Do **NOT** use `systemctl tomcat9`.
- Do **NOT** put `servlet-api.jar` in `WEB-INF/lib`.
- Do **NOT** copy all Struts plugins.

---

## ✅ Summary

- **Deploy:** `/opt/tomcat/webapps`
- **Compile:** manual `javac`
- **Restart:** `/opt/tomcat/bin`
- **Access:** `localhost:8080/StrutsLoginApp`

---

# 🔥 What actually matters (no fluff)

You must:
1. Put the app in `/opt/tomcat/webapps`
2. Put the correct JARs in `WEB-INF/lib`
3. Compile using the **Linux classpath (`:` not `;`)**
4. Restart Tomcat manually

---

If it breaks, send the output of:
```bash
tail -n 50 /opt/tomcat/logs/catalina.out
```
