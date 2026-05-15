Deploying a Full-Stack Python Blog on AWS EC2
=============================================

This guide walks you through deploying a custom Python web server that acts as a full-stack application (serving HTML and managing a JSON database) on **Port 8001**.


    

Step 1: Configure Port 8001 (Crucial Step)
------------------------------------------

Your backend is set to run on port 8001. We must tell the AWS Firewall to allow traffic to this port.

1.  Scroll down to **Network settings** and ensure "Auto-assign Public IP" is **Enabled**.
    
2.  Under **Firewall (security groups)**, select **"Create security group"**.
    
3.  Check the box for **"Allow SSH traffic from Anywhere"**.
    
4.  Click **"Edit"** in the top right of the Network Settings box to see advanced rules.
    
5.  Click **"Add security group rule"**:
    
    *   **Type:** Custom TCP
        
    *   **Port Range:** 8001
        
    *   **Source type:** Anywhere (0.0.0.0/0)
        
6.  Click **"Launch Instance"**.
    

Step 3: Transfer the Python File to the Server
----------------------------------------------

Wait a minute for the instance to initialize, grab its **Public IPv4 address**, and transfer your server file using your terminal (Mac/Linux) or a tool like FileZilla (Windows).

If using terminal (from the folder where you saved blog\_server.py):

`   scp -i path/to/your-key.pem blog_server.py ec2-user@:~/   `

Step 4: Run the Backend Server
------------------------------

1.  **Run the Server in the Background:** We use nohup so the server keeps running even after you close your SSH terminal window.
`   nohup python3 blog_server.py > server.log 2>&1 &   `

Step 5: Test Your Application
-----------------------------

Open any web browser and type in your server's IP address followed by the port:

http://51.45.21.228:8001

