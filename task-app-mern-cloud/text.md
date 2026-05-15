**Install Node.js:**

```
# Download and import the Nodesource GPG key
curl -fsSL https://deb.nodesource.com/setup_20.x | sudo -E bash -

# Install Node.js
sudo apt-get install -y nodejs
```

**Install and Start MongoDB:**

Ubuntu requires adding the official MongoDB GPG key and repository. Run these commands:

```   
# 1. Import the MongoDB GPG key
curl -fsSL https://www.mongodb.org/static/pgp/server-7.0.asc | \
   sudo gpg -o /usr/share/keyrings/mongodb-server-7.0.gpg \
   --dearmor

# 2. Create the list file for Ubuntu 22.04 (Jammy)
echo "deb [ arch=amd64,arm64 signed-by=/usr/share/keyrings/mongodb-server-7.0.gpg ] https://repo.mongodb.org/apt/ubuntu jammy/mongodb-org/7.0 multiverse" | \
   sudo tee /etc/apt/sources.list.d/mongodb-org-7.0.list

# 3. Reload local package database and install MongoDB
sudo apt-get update
sudo apt-get install -y mongodb-org

# 4. Start the database service and enable it to run on boot
sudo systemctl start mongod
sudo systemctl enable mongod 
```


Step 5: Install NPM Dependencies and Run
----------------------------------------

Go back to your SSH terminal window connected to your EC2 instance.

1.  **Initialize a Node project and install Express and Mongoose:**
`   npm init -y  npm install express mongoose   `

1.  **Start the Server:** We use nohup so the Node server keeps running in the background when you close the terminal.
`   nohup node task_server.js > server.log 2>&1 &   `

Step 6: Test Your Application
-----------------------------

1.  Open your web browser.
    
2.  Navigate to your cloud instance: http://51.45.21.228:8004