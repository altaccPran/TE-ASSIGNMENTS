Deploying a Cloud-Based Student Record System on AWS
====================================================

This guide walks you through deploying a full-stack Python application that utilizes an **SQLite Database** to manage student records. The server will run on **Port 8003**.
    



Step 4: Run the Application Server
----------------------------------
`   nohup python3 student_server.py > server.log 2>&1 &   `

_(Note: You do not need to install SQLite. It is built directly into Python! When the script runs, it will automatically generate the students.db file in the same folder)._

Step 5: Test and Manage Your App
--------------------------------

1.  Open your web browser.
    
2.  Navigate to your cloud instance: http://51.45.21.228:8003