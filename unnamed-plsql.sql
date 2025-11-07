
-- Create Tables
CREATE TABLE Borrower(
    Roll INT PRIMARY KEY,
    Name VARCHAR(100),
    DateofIssue DATE,
    NameofBook VARCHAR(100),
    Status CHAR(1)
);

CREATE TABLE Fine(
    Roll INT,
    Date DATE,
    Amt DECIMAL(10,2)
);

-- Stored Procedure
DELIMITER $$
CREATE PROCEDURE return_book(IN r INT, IN b VARCHAR(100))
BEGIN
    DECLARE d INT; DECLARE f DECIMAL(10,2); DECLARE dt DATE;
    DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
    START TRANSACTION;
    SELECT DateofIssue INTO dt FROM Borrower WHERE Roll=r AND NameofBook=b AND Status='I';
    SET d=DATEDIFF(CURDATE(),dt);
    SET f=IF(d>30,(d-30)*50+150,IF(d>=15,(d-15)*5,0));
    UPDATE Borrower SET Status='R' WHERE Roll=r AND NameofBook=b;
    IF f>0 THEN INSERT INTO Fine VALUES(r,CURDATE(),f); END IF;
    COMMIT;
    SELECT f AS Fine;
END$$
DELIMITER ;

-- Sample Data (Optional)
INSERT INTO Borrower VALUES(101,'John','2024-10-01','Database Systems','I');
INSERT INTO Borrower VALUES(102,'Alice','2024-10-20','Data Structures','I');

-- Call procedure
CALL return_book(101,'Database Systems');
