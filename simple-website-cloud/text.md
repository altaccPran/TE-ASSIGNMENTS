Run the Python HTTP Server
--------------------------   
`nohup python3 -m http.server 8000 > server.log 2>&1 &`
    
    *   8000: This is the port we opened in the security group.
        
    *   nohup ... &: Runs the process in the background and prevents it from being killed when you exit.
        
    *   \> server.log 2>&1: Saves access logs and errors to a file named server.log.
        

**Test the Web Server:**

Open a web browser on your computer and navigate to http://51.45.21.228:8000. 

You should see a directory listing (which will be empty right now). This means the server is running!
