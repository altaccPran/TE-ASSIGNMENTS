<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ page import="java.sql.*"%>
<%@ page import="java.util.*"%>

<%!
    private static String h(String s) {
        if (s == null) return "";
        return s.replace("&", "&amp;")
                .replace("<", "&lt;")
                .replace(">", "&gt;")
                .replace("\"", "&quot;");
    }
%>

<%
    // ✅ UPDATED FOR HOSTED DB
    final String JDBC_URL = "jdbc:mysql://10.10.8.119:3306/te31435_db?useSSL=false&serverTimezone=UTC&allowPublicKeyRetrieval=true";
    final String JDBC_USER = "te31435";
    final String JDBC_PASSWORD = "te31435";

    final String sql = "SELECT stud_id, stud_name, `class`, division, city FROM students_info";

    String err = null;
    List<String[]> rows = new ArrayList<String[]>();

    try {
        Class.forName("com.mysql.cj.jdbc.Driver");

        try (
            Connection conn = DriverManager.getConnection(JDBC_URL, JDBC_USER, JDBC_PASSWORD);
            Statement st = conn.createStatement();
            ResultSet rs = st.executeQuery(sql)
        ) {
            while (rs.next()) {
                rows.add(new String[] {
                    String.valueOf(rs.getInt("stud_id")),
                    rs.getString("stud_name"),
                    rs.getString("class"),
                    rs.getString("division"),
                    rs.getString("city")
                });
            }
        }

    } catch (ClassNotFoundException e) {
        err = "MySQL JDBC driver not found. Put mysql-connector JAR in WEB-INF/lib and restart Tomcat.";
        System.err.println("display_students.jsp: " + e.getMessage());

    } catch (SQLException e) {
        err = "Database error: " + e.getMessage();
        System.err.println("display_students.jsp SQL: " + e.getMessage());
        e.printStackTrace(System.err);
    }
%>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Students — JSP lab</title>
</head>
<body>

<h1>students_info</h1>

<% if (err != null) { %>
    <p style="color:red;"><strong>Error:</strong> <%= h(err) %></p>
<% } else if (rows.isEmpty()) { %>
    <p>No rows found.</p>
<% } else { %>

<table border="1" cellpadding="6">
    <tr>
        <th>stud_id</th>
        <th>stud_name</th>
        <th>class</th>
        <th>division</th>
        <th>city</th>
    </tr>

<% for (String[] r : rows) { %>
    <tr>
        <td><%= h(r[0]) %></td>
        <td><%= h(r[1]) %></td>
        <td><%= h(r[2]) %></td>
        <td><%= h(r[3]) %></td>
        <td><%= h(r[4]) %></td>
    </tr>
<% } %>

</table>

<% } %>

</body>
</html>
