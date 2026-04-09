-- JSP lab: students_info table (MySQL)
-- Run: mysql -u root -p < setup.sql   (Windows: same from Command Prompt in project folder)

CREATE DATABASE IF NOT EXISTS jsp_lab;
USE jsp_lab;

-- Column "class" is a reserved word in MySQL — use backticks in DDL/DML when needed.

DROP TABLE IF EXISTS students_info;

CREATE TABLE students_info (
    stud_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    stud_name VARCHAR(120) NOT NULL,
    `class` VARCHAR(32) NOT NULL,
    division VARCHAR(16) NOT NULL,
    city VARCHAR(100) NOT NULL
);

INSERT INTO students_info (stud_name, `class`, division, city) VALUES
    ('Ananya Sharma', '10', 'A', 'Pune'),
    ('Rohit Verma', '10', 'B', 'Mumbai'),
    ('Sneha Iyer', '11', 'A', 'Bengaluru'),
    ('Karan Mehta', '11', 'C', 'Delhi'),
    ('Priya Nair', '12', 'A', 'Chennai'),
    ('Vikram Joshi', '9', 'B', 'Hyderabad');

-- Verify: SELECT * FROM students_info;
