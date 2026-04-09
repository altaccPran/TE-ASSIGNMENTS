<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ page import="java.sql.*"%>
<%@ page import="java.util.*"%>
<%--
  Lab: display MySQL table students_info using JDBC in JSP scriptlets.
  Adjust JDBC_URL / JDBC_USER / JDBC_PASSWORD to match your MySQL setup.
--%>
<%!
    private static String h(String s) {
        if (s == null) {
            return "";
        }
        return s.replace("&", "&amp;")
                .replace("<", "&lt;")
                .replace(">", "&gt;")
                .replace("\"", "&quot;");
    }
%>
<%
    final String JDBC_URL = "jdbc:mysql://localhost:3306/jsp_lab?useSSL=false&serverTimezone=UTC&allowPublicKeyRetrieval=true";
    final String JDBC_USER = "root";
    final String JDBC_PASSWORD = "";

    final String sql = "SELECT stud_id, stud_name, `class`, division, city FROM students_info";

    String err = null;
    List<String[]> rows = new ArrayList<String[]>();

    try {
        Class.forName("com.mysql.cj.jdbc.Driver");
        try (Connection conn = DriverManager.getConnection(JDBC_URL, JDBC_USER, JDBC_PASSWORD);
             Statement st = conn.createStatement();
             ResultSet rs = st.executeQuery(sql)) {
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
    <style type="text/css">
        body {
            font-family: Segoe UI, Tahoma, Arial, sans-serif;
            margin: 24px;
            background: #f5f6fa;
            color: #222;
        }
        h1 {
            font-size: 1.35rem;
            margin-bottom: 8px;
        }
        p.note {
            font-size: 0.9rem;
            color: #555;
            margin-top: 0;
        }
        table {
            border-collapse: collapse;
            width: 100%;
            max-width: 720px;
            background: #fff;
            box-shadow: 0 1px 3px rgba(0,0,0,0.08);
        }
        th, td {
            border: 1px solid #ccc;
            padding: 8px 12px;
            text-align: left;
        }
        th {
            background: #2c3e50;
            color: #fff;
            font-weight: 600;
        }
        tr:nth-child(even) {
            background: #f9f9f9;
        }
        .err {
            color: #b00020;
            max-width: 720px;
        }
    </style>
</head>
<body>
    <h1>students_info</h1>
    <p class="note">Rows from <code>SELECT</code> executed in JSP scriptlets.</p>

<% if (err != null) { %>
    <p class="err"><strong>Error:</strong> <%= h(err) %></p>
<% } else if (rows.isEmpty()) { %>
    <p>No rows found (table empty or query returned nothing).</p>
<% } else { %>
    <table>
        <thead>
            <tr>
                <th>stud_id</th>
                <th>stud_name</th>
                <th>class</th>
                <th>division</th>
                <th>city</th>
            </tr>
        </thead>
        <tbody>
<% for (String[] r : rows) { %>
            <tr>
                <td><%= h(r[0]) %></td>
                <td><%= h(r[1]) %></td>
                <td><%= h(r[2]) %></td>
                <td><%= h(r[3]) %></td>
                <td><%= h(r[4]) %></td>
            </tr>
<% } %>
        </tbody>
    </table>
<% } %>
</body>
</html>
