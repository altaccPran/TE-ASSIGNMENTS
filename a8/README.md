# Assignment 8 — Apache Struts 2.5 login validation (manual Tomcat deploy on macOS)

This web app demonstrates a **login form** with **server-side validation** using **Struts 2** (`ActionSupport` + `validate()`), **Struts UI tags** on `login.jsp`, and a **welcome** page on success.

## Project layout (under Tomcat `webapps`)

Copy the whole **`StrutsLoginApp`** folder into:

**`$CATALINA_HOME/webapps/StrutsLoginApp`**

Typical layout (Homebrew Tomcat 9: `CATALINA_HOME=$(brew --prefix tomcat@9)/libexec`):

```text
webapps/StrutsLoginApp/
  login.jsp
  welcome.jsp
  WEB-INF/
    web.xml
    classes/
      struts.xml
      com/lab/struts/LoginAction.class
      com/lab/struts/LoginAction.java   (optional: keep source for your repo only)
    lib/
      (minimal set — see [`StrutsLoginApp/WEB-INF/lib/README.txt`](StrutsLoginApp/WEB-INF/lib/README.txt);
      do not drop in every JAR from the full distribution or optional plugins will break startup)
```

- **JSPs** live at the **webapp root** (`login.jsp`, `welcome.jsp`).
- **`struts.xml`** must be on the classpath: **`WEB-INF/classes/struts.xml`**.
- **`web.xml`** is **`WEB-INF/web.xml`**.
- **Compiled action** is **`WEB-INF/classes/com/lab/struts/LoginAction.class`**.

## Minimum Struts 2.5.x JARs (`WEB-INF/lib`)

Use the **explicit file list** in [`StrutsLoginApp/WEB-INF/lib/README.txt`](StrutsLoginApp/WEB-INF/lib/README.txt) (pinned to **2.5.32** for this repo). Copying **every** JAR from the official distribution’s `lib` folder also copies **optional plugins** (OSGi, GXP, Tiles, etc.) that register `struts-plugin.xml` and often **fail on Tomcat** without extra dependencies.

For reference, the usual **core** pieces (names/versions vary by release — match your download) are:

| JAR (pattern) | Role |
|---------------|------|
| `struts2-core-*.jar` | Struts core, integrates XWork |
| `ognl-*.jar` | Expression language |
| `freemarker-*.jar` | UI tag templates |
| `commons-fileupload-*.jar` | Multipart / form handling |
| `commons-io-*.jar` | IO utilities |
| `commons-lang3-*.jar` | String / object utilities |
| `commons-logging-*.jar` | Logging facade (often used by Struts stack) |
| `javassist-*.jar` | Bytecode (OGNL / plugins) |
| `asm-*.jar`, `asm-commons-*.jar` | Bytecode (versions must match the stack) |
| `log4j-api-*.jar`, `log4j-core-*.jar` | Logging (Struts 2.5.x stacks often ship Log4j 2) |

**Do not** put `servlet-api.jar` from Tomcat into `WEB-INF/lib` for normal deployment — Tomcat already provides it on the **server** classpath. You only need **`servlet-api.jar` on the `javac` classpath** when compiling (see below).

Download: [Struts 2 download page](https://struts.apache.org/download.cgi) → **Full Distribution** → use its **`lib`** folder.

## Compile `LoginAction.java` on macOS

1. Install **JDK** and ensure **`javac`** is on your **`PATH`** (e.g. Homebrew `openjdk@17`).

2. Place **all** Struts JARs (and their dependencies) under **`StrutsLoginApp/WEB-INF/lib/`** first.

3. From a terminal:

```bash
export CATALINA_HOME="$(brew --prefix tomcat@9)/libexec"   # adjust if Tomcat is elsewhere

cd /path/to/StrutsLoginApp/WEB-INF/classes

# Build classpath: all Struts JARs + Tomcat servlet-api (for compile-time only)
CP="$(echo ../lib/*.jar | sed 's/ /:/g'):${CATALINA_HOME}/lib/servlet-api.jar"

javac -encoding UTF-8 -cp "$CP" com/lab/struts/LoginAction.java
```

**Note:** On **macOS/Linux**, classpath entries are separated by **`:`** (not `;` like Windows).

If `echo ../lib/*.jar` expands oddly with **zsh**, use:

```bash
CP=""
for j in ../lib/*.jar; do CP="$CP:$j"; done
CP="${CP#:}:${CATALINA_HOME}/lib/servlet-api.jar"
javac -encoding UTF-8 -cp "$CP" com/lab/struts/LoginAction.java
```

Verify:

```bash
ls com/lab/struts/LoginAction.class
```

## Deploy and run

1. **Copy** (or symlink) **`StrutsLoginApp`** into **`$CATALINA_HOME/webapps/`**.

2. **Restart** Tomcat, e.g.:

   ```bash
   brew services restart tomcat@9
   ```

3. Open in a browser:

   - **`http://localhost:8080/StrutsLoginApp/`** (welcome file → `login.jsp`)
   - Or **`http://localhost:8080/StrutsLoginApp/login.jsp`**

4. Submit the form — it posts to the **`login`** action (`login.action`).  
   - Validation errors → **`login.jsp`** with **field errors** and **values preserved**.  
   - Success → **`welcome.jsp`** with **Congratulations** and **`<s:property value="name"/>`**.

## Validation rules (reference)

Implemented in **`LoginAction.validate()`**:

| Rule | Behaviour |
|------|-----------|
| (d) Empty fields | Field errors for name, mobile, email |
| (a) Name | Letters/spaces/apostrophe/dot/hyphen; must start with a letter; not numeric-only |
| (b) Mobile | Exactly **10 digits**, first digit **6–9** (typical Indian mobile pattern — change regex if required) |
| (c) Email | Simple email regex |
| (e) Wrong values | Workflow returns **`input`** → **`login.jsp`** with messages |
| (f) Success | **`SUCCESS`** → **`welcome.jsp`** |

## Troubleshooting

| Symptom | Check |
|---------|--------|
| **404** | Context path **`/StrutsLoginApp`**; JSP names; Tomcat `webapps` folder |
| **ClassNotFoundException** / **NoClassDefFoundError** | Every required JAR is in **`WEB-INF/lib`**; restart Tomcat |
| **Filter / Struts startup error** | **`web.xml`** filter class; **`struts.xml`** in **`WEB-INF/classes`**; DTD reachable or offline cache |
| **Validation never runs** | DefaultStack includes **`validation`** and **`workflow`** — do not remove `extends="struts-default"` |

## Files in this repo

| File | Role |
|------|------|
| [`StrutsLoginApp/login.jsp`](StrutsLoginApp/login.jsp) | Struts form + `s:fielderror` |
| [`StrutsLoginApp/welcome.jsp`](StrutsLoginApp/welcome.jsp) | Success + `s:property` |
| [`StrutsLoginApp/WEB-INF/classes/com/lab/struts/LoginAction.java`](StrutsLoginApp/WEB-INF/classes/com/lab/struts/LoginAction.java) | `ActionSupport`, `validate()` |
| [`StrutsLoginApp/WEB-INF/classes/struts.xml`](StrutsLoginApp/WEB-INF/classes/struts.xml) | Action mapping + results |
| [`StrutsLoginApp/WEB-INF/web.xml`](StrutsLoginApp/WEB-INF/web.xml) | `StrutsPrepareAndExecuteFilter` on `/*` |

## Theory (short — for your journal)

- **JSP** + **Struts UI tags** render the form; **Action** classes hold fields and validation.
- **`validate()`** adds **field errors**; the **workflow** interceptor returns **`input`** without calling **`execute()`** when errors exist.
- **Struts** maps **`login.action`** to **`LoginAction`**, with **`input`** → `login.jsp` and **`success`** → `welcome.jsp`.

## References

- [Apache Struts 2 documentation](https://struts.apache.org/)
- Servlet 3.1 deployment descriptor (Tomcat 8.5 / 9)
