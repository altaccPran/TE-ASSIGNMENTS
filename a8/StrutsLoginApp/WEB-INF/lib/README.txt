Minimal Struts 2.5.32 runtime for this lab (servlet + JSP tags + default stack).

Do not copy the entire Struts distribution `lib` folder — optional plugins (OSGi, GXP, Tiles,
Spring, JSON, etc.) register `struts-plugin.xml` entries and break a plain Tomcat deployment.

Required JARs (14 files), versions pinned to match this assignment:

  struts2-core-2.5.32.jar
  ognl-3.1.29.jar
  freemarker-2.3.31.jar
  commons-fileupload-1.4.jar
  commons-io-2.6.jar
  commons-lang3-3.8.1.jar
  commons-logging-1.2.jar
  javassist-3.20.0-GA.jar
  asm-7.3.1.jar
  asm-analysis-7.3.1.jar
  asm-commons-7.3.1.jar
  asm-tree-7.3.1.jar
  log4j-api-2.12.4.jar
  log4j-core-2.12.4.jar

If you upgrade Struts, replace all of the above as a set (same release).

The servlet API is provided by Tomcat — do not add servlet-api.jar here for normal deploy.
