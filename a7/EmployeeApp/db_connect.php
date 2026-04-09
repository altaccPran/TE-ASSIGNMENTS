<?php
/**
 * Shared database connection for the Employee Directory app.
 * Uses MySQLi with UTF-8. Adjust credentials if your XAMPP MySQL user differs.
 */

declare(strict_types=1);

$DB_HOST = '127.0.0.1';
$DB_USER = 'root';
$DB_PASS = '';
$DB_NAME = 'company_db';
$DB_PORT = 3306;

mysqli_report(MYSQLI_REPORT_ERROR | MYSQLI_REPORT_STRICT);

try {
    $mysqli = new mysqli($DB_HOST, $DB_USER, $DB_PASS, $DB_NAME, $DB_PORT);
} catch (mysqli_sql_exception $e) {
    http_response_code(500);
    exit(
        '<!DOCTYPE html><html><head><meta charset="utf-8"><title>Database error</title></head><body>' .
        '<h1>Could not connect to MySQL</h1>' .
        '<p>Check that MySQL is running in XAMPP and that <code>setup.sql</code> has been applied.</p>' .
        '<p><strong>Details:</strong> ' . htmlspecialchars($e->getMessage(), ENT_QUOTES, 'UTF-8') . '</p>' .
        '</body></html>'
    );
}

$mysqli->set_charset('utf8mb4');
