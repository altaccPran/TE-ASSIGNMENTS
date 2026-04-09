-- Employee Directory lab (PHP + MySQL)
-- Database: company_db, Table: employees
-- Run this in phpMyAdmin or: mysql -u root -p < setup.sql

CREATE DATABASE IF NOT EXISTS company_db
  CHARACTER SET utf8mb4
  COLLATE utf8mb4_unicode_ci;

USE company_db;

DROP TABLE IF EXISTS employees;

CREATE TABLE employees (
    id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(120) NOT NULL,
    email VARCHAR(180) NOT NULL,
    department VARCHAR(100) NOT NULL,
    designation VARCHAR(100) NOT NULL,
    UNIQUE KEY uq_employees_email (email)
) ENGINE=InnoDB;

INSERT INTO employees (name, email, department, designation) VALUES
    ('Alice Johnson', 'alice.johnson@company.test', 'Engineering', 'Software Developer'),
    ('Bob Smith', 'bob.smith@company.test', 'Human Resources', 'HR Specialist'),
    ('Carol Davis', 'carol.davis@company.test', 'Finance', 'Financial Analyst');

-- Verify: SELECT * FROM employees;
