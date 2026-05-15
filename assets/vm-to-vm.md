Assignment: Secure File Sharing Between Cloud Instances
=======================================================

Objective
---------

Create a secure cloud environment consisting of two Virtual Machines (VMs) residing in the same Virtual Network (VPC). Implement a secure, passwordless authentication mechanism to transfer files directly between the instances using internal network routing, and demonstrate proper file access permissions.

Architecture Overview
---------------------

*   **Cloud Provider:** AWS (Amazon Web Services)
    
*   **Operating System:** Ubuntu Server 24.04 LTS
    
*   **Network:** Default AWS Virtual Private Cloud (VPC)
    
*   **Nodes:**
    
    *   VM-A-Sender (The instance sending the file)
        
    *   VM-B-Receiver (The instance receiving the file)
        
*   **Protocol:** SCP (Secure Copy Protocol) via Port 22
    

Step 1: Provisioning the Virtual Machines
-----------------------------------------

1.  Log into the AWS Management Console and navigate to **EC2**.
    
2.  Click **Launch Instances**.
    
3.  **Instance 1 (VM-A-Sender):**
    
    *   **Name:** VM-A-Sender
        
    *   **AMI:** Ubuntu Server 24.04 LTS
        
    *   **Instance Type:** t2.micro
        
    *   **Network:** Ensure it is in your default VPC.
        
    *   **Key Pair:** Select your standard .pem key pair (to allow you to log in from your personal computer).
        
4.  **Instance 2 (VM-B-Receiver):**
    
    *   Repeat the exact same steps above, but name the instance VM-B-Receiver. Ensure it is launched into the **same VPC** as VM-A.
        

Step 2: Configuring the Security Group (Network Permissions)
------------------------------------------------------------

To ensure the VMs can talk to each other securely, we must configure the AWS Firewall (Security Group).

1.  In the EC2 console, go to **Security Groups**.
    
2.  Select the security group attached to your instances (usually named launch-wizard-x).
    
3.  Click **Edit inbound rules**.
    
4.  You need two rules for Port 22 (SSH):
    
    *   **Rule 1 (Your Access):** Type: SSH, Port: 22, Source: My IP (Allows you to manage the VMs from your laptop).
        
    *   **Rule 2 (Internal VM Access):** Type: SSH, Port: 22, Source: Custom -> Enter the **Private IPv4 CIDR block** of your VPC (e.g., 172.31.0.0/16).
        
    *   _Note: Rule 2 ensures that only machines within this specific private cloud network can attempt to SSH into one another._
        

Step 3: Establishing Identity (SSH Key Generation)
--------------------------------------------------

Instead of sharing your personal .pem key, VM-A should have its own identity to authenticate with VM-B.

1.  ssh -i your-local-key.pem ubuntu@
    
2.  ssh-keygen -t ed25519 -C "vm-a-internal-key"
    
3.  cat ~/.ssh/id\_ed25519.pub_Copy the entire output starting with ssh-ed25519... to your clipboard._
    

Step 4: Configuring Access Permissions on the Receiver
------------------------------------------------------

We must explicitly authorize VM-A to access VM-B by adding the public key to VM-B's trusted list.

1.  ssh -i your-local-key.pem ubuntu@
    
2.  nano ~/.ssh/authorized\_keys
    
3.  **Paste** the public key you copied from VM-A onto a new line at the bottom of this file. Save and exit (Ctrl+O, Enter, Ctrl+X).
    
4.  chmod 700 ~/.sshchmod 600 ~/.ssh/authorized\_keys_(This ensures only the owner can read/write the keys, preventing unauthorized local users from tampering with access)._
    

Step 5: Secure File Transfer via Internal Network
-------------------------------------------------

Now we will create a file on VM-A and send it to VM-B using VM-B's **Private IP Address**. Using the Private IP ensures the data never leaves the AWS physical data center.

1.  Go back to your terminal connected to **VM-A-Sender**.
    
2.  echo "This is highly sensitive financial data." > top\_secret.txt
    
3.  Retrieve VM-B's **Private IPv4 address** from the AWS Console (e.g., 172.31.45.12).
    
4.  scp top\_secret.txt ubuntu@:/home/ubuntu/_You will be asked to confirm the fingerprint the first time. Type yes.Because we configured the SSH keys in Step 4, it will transfer instantly without asking for a password._
    

Step 6: Verifying and Setting Final File Permissions
----------------------------------------------------

Finally, we must ensure the transferred file has the correct access permissions on the destination machine.

1.  Go to your terminal connected to **VM-B-Receiver**.
    
2.  ls -la top\_secret.txtcat top\_secret.txt
    
3.  chmod 400 top\_secret.txt
    
4.  ls -l top\_secret.txt_The output should show -r--------, indicating Read-only access for the owner, and no access for groups or others._
    

Conclusion
----------

You have successfully:

1.  Networked two Linux VMs within a secure VPC.
    
2.  Implemented asymmetric cryptography (SSH Keys) for machine-to-machine identity.
    
3.  Transferred data over an encrypted tunnel (SCP) using internal routing (Private IP).
    
4.  Demonstrated Linux discretionary access control (chmod 600, chmod 400).