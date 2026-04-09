Place the MySQL Connector/J JAR here so the JSP can load com.mysql.cj.jdbc.Driver.

Example filenames:
  mysql-connector-j-8.3.0.jar
  mysql-connector-java-8.0.33.jar

Download: https://dev.mysql.com/downloads/connector/j/

Use the app's WEB-INF/lib (recommended for this lab) so only StudentApp sees the driver.
You may also copy the same JAR to Tomcat's global lib folder (CATALINA_HOME/lib), but WEB-INF/lib is the usual place for a single web application.

After adding or changing the JAR, restart Tomcat.
