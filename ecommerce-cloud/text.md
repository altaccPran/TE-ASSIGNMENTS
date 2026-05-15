Run the Application Server
----------------------------------

1.  **Start the Server:** We use nohup so the server keeps running in the background.
`   nohup python3 ecommerce_server.py > server.log 2>&1 &   `

_(Note: The first time this runs, it will auto-generate the ecommerce.db database file and seed it with dummy products so the store isn't empty)._

Step 5: Test and Manage Your Store
----------------------------------

1.  Open your web browser.
    
2.  Navigate to your cloud instance: http://51.45.21.228:8002