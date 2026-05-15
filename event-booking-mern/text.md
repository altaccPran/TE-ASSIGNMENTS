Install NPM Dependencies and Run
----------------------------------------

Go back to your SSH terminal window connected to your EC2 instance.

1.  **Initialize a Node project and install Express and Mongoose:**
    

`   npm init -y  npm install express mongoose   `

2.  **Start the Server:** We use nohup so the Node server keeps running in the background when you close the terminal.
    

`   nohup node event_server.js > server.log 2>&1 &   `

Step 6: Test Your Application
-----------------------------

1.  Open your web browser.
    
2.  Navigate to your cloud instance: http://51.45.21.22:8005