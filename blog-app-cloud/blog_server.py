import http.server
import socketserver
import json
import os
from datetime import datetime

# Configuration
PORT = 8001
DB_FILE = 'blog_database.json'

# Ensure the database file exists when the server starts
if not os.path.exists(DB_FILE):
    with open(DB_FILE, 'w') as db_file:
        json.dump([], db_file)

# This is the exact HTML that the Python server will send to the user's browser
FRONTEND_HTML = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cloud Blog Admin</title>
    <script src="https://cdn.tailwindcss.com"></script>
</head>
<body class="bg-slate-100 min-h-screen font-sans text-slate-800 p-4 md:p-8">

    <div class="max-w-2xl mx-auto">
        <!-- Header -->
        <header class="mb-8">
            <h1 class="text-3xl font-extrabold text-blue-600">My Cloud Blog</h1>
            <p class="text-slate-500 text-sm">Full-Stack Application running on Python Server</p>
        </header>

        <!-- Create Post Form (Frontend interacting with Backend) -->
        <div class="bg-white p-6 rounded-xl shadow-md mb-8">
            <h2 class="text-xl font-bold mb-4">Create New Post</h2>
            <form id="post-form" class="space-y-4">
                <div>
                    <input type="text" id="title" placeholder="Post Title" required
                        class="w-full px-4 py-2 border border-slate-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500">
                </div>
                <div>
                    <textarea id="content" placeholder="Write your blog post here..." required rows="4"
                        class="w-full px-4 py-2 border border-slate-300 rounded-lg focus:outline-none focus:ring-2 focus:ring-blue-500"></textarea>
                </div>
                <button type="submit" 
                    class="bg-blue-600 hover:bg-blue-700 text-white font-semibold py-2 px-6 rounded-lg transition-colors shadow-sm">
                    Publish Post
                </button>
            </form>
        </div>

        <!-- Blog Posts Container -->
        <div>
            <h2 class="text-xl font-bold mb-4 text-slate-700">Recent Posts</h2>
            <div id="posts-container" class="space-y-4">
                <!-- Posts will be injected here by JavaScript -->
                <p class="text-slate-400 italic">Loading posts...</p>
            </div>
        </div>
    </div>

    <!-- Frontend Logic -->
    <script>
        // 1. Fetch and display posts from the Python Backend
        async function loadPosts() {
            try {
                const response = await fetch('/api/posts');
                const posts = await response.json();
                const container = document.getElementById('posts-container');
                
                if (posts.length === 0) {
                    container.innerHTML = '<p class="text-slate-400 italic">No posts yet. Be the first to write something!</p>';
                    return;
                }

                container.innerHTML = posts.map(post => `
                    <div class="bg-white p-6 rounded-xl shadow-sm border border-slate-200">
                        <div class="flex justify-between items-start mb-2">
                            <h3 class="text-xl font-bold text-slate-800">${post.title}</h3>
                            <button onclick="deletePost('${post.id}')" class="text-xs text-red-500 hover:text-red-700 font-medium">Delete</button>
                        </div>
                        <p class="text-xs text-slate-400 mb-3">${post.date}</p>
                        <p class="text-slate-600 whitespace-pre-wrap leading-relaxed">${post.content}</p>
                    </div>
                `).join('');
            } catch (error) {
                console.error("Error loading posts:", error);
            }
        }

        // 2. Handle form submission to create a new post
        document.getElementById('post-form').addEventListener('submit', async (e) => {
            e.preventDefault(); // Prevent page reload
            const title = document.getElementById('title').value;
            const content = document.getElementById('content').value;

            // Send POST request to Python Backend
            await fetch('/api/posts', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ title, content })
            });

            // Clear form and reload posts
            document.getElementById('post-form').reset();
            loadPosts();
        });

        // 3. Handle deleting a post
        async function deletePost(id) {
            if(confirm("Are you sure you want to delete this post?")) {
                await fetch('/api/posts/' + id, { method: 'DELETE' });
                loadPosts(); // Reload UI
            }
        }

        // Initial load
        loadPosts();
    </script>
</body>
</html>
"""

class BlogAPIHandler(http.server.SimpleHTTPRequestHandler):
    
    # Handle GET Requests (Loading the page & fetching posts)
    def do_GET(self):
        if self.path == '/':
            # Serve the HTML Interface
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            self.wfile.write(FRONTEND_HTML.encode('utf-8'))
            
        elif self.path == '/api/posts':
            # Serve the Database data as JSON
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.end_headers()
            with open(DB_FILE, 'r') as db_file:
                self.wfile.write(db_file.read().encode('utf-8'))
                
        else:
            # Handle 404 for unknown routes
            self.send_response(404)
            self.end_headers()

    # Handle POST Requests (Creating a new post)
    def do_POST(self):
        if self.path == '/api/posts':
            # Read incoming data
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            new_post = json.loads(post_data.decode('utf-8'))
            
            # Add metadata (ID and Timestamp)
            new_post['id'] = str(datetime.now().timestamp())
            new_post['date'] = datetime.now().strftime("%B %d, %Y - %H:%M")

            # Load existing DB, prepend new post, save DB
            with open(DB_FILE, 'r') as db_file:
                posts = json.load(db_file)
                
            posts.insert(0, new_post)

            with open(DB_FILE, 'w') as db_file:
                json.dump(posts, db_file, indent=4)

            # Send success response
            self.send_response(201)
            self.end_headers()

    # Handle DELETE Requests (Removing a post)
    def do_DELETE(self):
        if self.path.startswith('/api/posts/'):
            post_id = self.path.split('/')[-1]
            
            # Load DB, filter out the deleted ID, save DB
            with open(DB_FILE, 'r') as db_file:
                posts = json.load(db_file)
                
            posts = [p for p in posts if p.get('id') != post_id]

            with open(DB_FILE, 'w') as db_file:
                json.dump(posts, db_file, indent=4)
            
            self.send_response(200)
            self.end_headers()

if __name__ == "__main__":
    # Prevent Address already in use errors
    socketserver.TCPServer.allow_reuse_address = True
    
    with socketserver.TCPServer(("", PORT), BlogAPIHandler) as httpd:
        print(f"==================================================")
        print(f"🚀 Full-Stack Blog Server running on port {PORT}")
        print(f"Database file: {DB_FILE} will be created locally.")
        print(f"==================================================")
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nShutting down server.")
            httpd.server_close()