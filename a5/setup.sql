-- ebookshop lab: create database, table, and sample rows
-- Run from Windows Command Prompt (adjust user/path as needed), for example:
--   mysql -u root -p < setup.sql
-- Or paste into MySQL Workbench / mysql client after connecting

-- Table name matches assignment: ebookshop
-- Columns: book_id, book_title, book_author, book_price, quantity

DROP TABLE IF EXISTS ebookshop;

CREATE TABLE ebookshop (
    book_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    book_title VARCHAR(200) NOT NULL,
    book_author VARCHAR(120) NOT NULL,
    book_price DECIMAL(10, 2) NOT NULL,
    quantity INT NOT NULL
);

INSERT INTO ebookshop (book_title, book_author, book_price, quantity) VALUES
    ('Clean Code', 'Robert C. Martin', 42.99, 12),
    ('Effective Java', 'Joshua Bloch', 54.50, 8),
    ('Head First Servlets', 'Basham et al.', 39.99, 5),
    ('MySQL Cookbook', 'DuBois', 49.00, 3),
    ('Java Servlet Programming', 'Hunter', 45.00, 7),
    ('Database Systems', 'Connolly & Begg', 62.25, 4);

-- Verify:
-- SELECT * FROM ebookshop;
