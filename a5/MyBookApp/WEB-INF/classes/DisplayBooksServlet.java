import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Lab servlet: loads rows from MySQL table ebookshop and prints an HTML table.
 * Uses javax.servlet (Tomcat 8/9). JDBC driver: com.mysql.cj.jdbc.Driver (MySQL 8+).
 */
public class DisplayBooksServlet extends HttpServlet {

    private static final long serialVersionUID = 1L;

    private String jdbcUrl;
    private String jdbcUser;
    private String jdbcPassword;

    @Override
    public void init(ServletConfig config) throws ServletException {
        super.init(config);
        this.jdbcUrl = config.getInitParameter("jdbcUrl");
        this.jdbcUser = config.getInitParameter("jdbcUser");
        this.jdbcPassword = config.getInitParameter("jdbcPassword");
        if (jdbcUrl == null || jdbcUrl.isEmpty()) {
            throw new ServletException("init-param jdbcUrl is missing in web.xml");
        }
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse res)
            throws ServletException, IOException {

        res.setContentType("text/html;charset=UTF-8");
        PrintWriter out = res.getWriter();

        out.println("<!DOCTYPE html>");
        out.println("<html><head><meta charset=\"UTF-8\"><title>ebookshop</title></head><body>");
        out.println("<h1>ebookshop inventory</h1>");
        out.println("<p>Data from MySQL table <code>ebookshop</code> (SELECT *).</p>");

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            System.err.println("MySQL JDBC driver not found: " + e.getMessage());
            out.println("<p style=\"color:red;\">Error: MySQL JDBC driver not found. ");
            out.println("Add mysql-connector JAR to WEB-INF/lib and restart Tomcat.</p>");
            out.println("</body></html>");
            return;
        }

        final String sql = "SELECT * FROM ebookshop";

        try (
                Connection conn = DriverManager.getConnection(jdbcUrl, jdbcUser, jdbcPassword);
                Statement st = conn.createStatement();
                ResultSet rs = st.executeQuery(sql)) {

            out.println("<table border=\"1\" cellpadding=\"6\" cellspacing=\"0\">");
            out.println("<tr><th>book_id</th><th>book_title</th><th>book_author</th>"
                    + "<th>book_price</th><th>quantity</th></tr>");

            while (rs.next()) {
                out.print("<tr>");
                out.print("<td>" + escapeHtml(String.valueOf(rs.getInt("book_id"))) + "</td>");
                out.print("<td>" + escapeHtml(rs.getString("book_title")) + "</td>");
                out.print("<td>" + escapeHtml(rs.getString("book_author")) + "</td>");
                out.print("<td>" + escapeHtml(String.valueOf(rs.getBigDecimal("book_price"))) + "</td>");
                out.print("<td>" + escapeHtml(String.valueOf(rs.getInt("quantity"))) + "</td>");
                out.println("</tr>");
            }

            out.println("</table>");

        } catch (SQLException e) {
            System.err.println("SQL error: " + e.getMessage());
            e.printStackTrace(System.err);
            out.println("<p style=\"color:red;\">Database error. Check MySQL is running, ");
            out.println("credentials in web.xml, and that setup.sql was applied.</p>");
            out.println("<pre>");
            out.print(escapeHtml(e.getMessage()));
            out.println("</pre>");
        }

        out.println("<p><a href=\"index.html\">Back to home</a></p>");
        out.println("</body></html>");
    }

    /** Minimal escaping for HTML body text */
    private static String escapeHtml(String s) {
        if (s == null) {
            return "";
        }
        return s.replace("&", "&amp;")
                .replace("<", "&lt;")
                .replace(">", "&gt;")
                .replace("\"", "&quot;");
    }
}
