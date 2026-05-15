import http.server
import socketserver
import json
import sqlite3
import os

# Configuration
PORT = 8003
DB_FILE = 'students.db'

def init_db():
    """Initialize the SQLite database and create the table if it doesn't exist."""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS students (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            roll_number TEXT UNIQUE NOT NULL,
            email TEXT NOT NULL,
            course TEXT NOT NULL
        )
    ''')
    conn.commit()
    conn.close()

# Helper function to execute SQL queries safely
def execute_query(query, params=(), fetch=False):
    conn = sqlite3.connect(DB_FILE)
    conn.row_factory = sqlite3.Row # Allows accessing columns by name
    cursor = conn.cursor()
    try:
        cursor.execute(query, params)
        result = None
        if fetch:
            result = [dict(row) for row in cursor.fetchall()]
        conn.commit()
        return result, None
    except sqlite3.Error as e:
        return None, str(e)
    finally:
        conn.close()

init_db()

FRONTEND_HTML = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Student Records</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
</head>
<body class="bg-gray-50 min-h-screen p-4 md:p-8 font-sans">
    <div class="max-w-5xl mx-auto">
        <!-- Header -->
        <header class="mb-8 bg-white p-6 rounded-xl shadow-sm border border-gray-200 flex justify-between items-center">
            <div>
                <h1 class="text-2xl font-bold text-indigo-700"><i class="fa-solid fa-graduation-cap mr-2"></i>Student Manager</h1>
                <p class="text-gray-500 text-sm mt-1">Manage student records via SQLite Database</p>
            </div>
            <div class="text-sm font-medium text-gray-400">
                Port: 8003
            </div>
        </header>

        <div class="grid grid-cols-1 md:grid-cols-3 gap-8">
            <!-- Form Section -->
            <div class="md:col-span-1">
                <div class="bg-white p-6 rounded-xl shadow-sm border border-gray-200 sticky top-8">
                    <h2 class="text-lg font-bold text-gray-800 mb-4" id="form-title">Add New Student</h2>
                    <form id="student-form" class="space-y-4">
                        <input type="hidden" id="student-id">
                        
                        <div>
                            <label class="block text-sm font-medium text-gray-700 mb-1">Full Name</label>
                            <input type="text" id="name" required class="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-indigo-500">
                        </div>
                        
                        <div>
                            <label class="block text-sm font-medium text-gray-700 mb-1">Roll Number</label>
                            <input type="text" id="roll" required class="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-indigo-500">
                        </div>

                        <div>
                            <label class="block text-sm font-medium text-gray-700 mb-1">Email</label>
                            <input type="email" id="email" required class="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-indigo-500">
                        </div>

                        <div>
                            <label class="block text-sm font-medium text-gray-700 mb-1">Course</label>
                            <select id="course" required class="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-indigo-500">
                                <option value="Computer Science">Computer Science</option>
                                <option value="Engineering">Engineering</option>
                                <option value="Business">Business</option>
                                <option value="Arts">Arts</option>
                            </select>
                        </div>
                        
                        <div class="pt-2 flex gap-2">
                            <button type="submit" id="submit-btn" class="flex-1 bg-indigo-600 hover:bg-indigo-700 text-white font-medium py-2 px-4 rounded-md transition-colors">
                                Save Student
                            </button>
                            <button type="button" onclick="resetForm()" class="px-4 py-2 bg-gray-100 hover:bg-gray-200 text-gray-700 font-medium rounded-md transition-colors">
                                Clear
                            </button>
                        </div>
                    </form>
                </div>
            </div>

            <!-- Table Section -->
            <div class="md:col-span-2">
                <div class="bg-white rounded-xl shadow-sm border border-gray-200 overflow-hidden">
                    <table class="min-w-full divide-y divide-gray-200">
                        <thead class="bg-gray-50">
                            <tr>
                                <th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Student</th>
                                <th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Roll No.</th>
                                <th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Course</th>
                                <th class="px-6 py-3 text-right text-xs font-medium text-gray-500 uppercase tracking-wider">Actions</th>
                            </tr>
                        </thead>
                        <tbody id="student-table-body" class="bg-white divide-y divide-gray-200">
                            <!-- Rows injected by JS -->
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    </div>

    <!-- Frontend JavaScript Logic -->
    <script>
        const form = document.getElementById('student-form');
        const formTitle = document.getElementById('form-title');
        const submitBtn = document.getElementById('submit-btn');

        // Fetch and display students
        async function fetchStudents() {
            try {
                const response = await fetch('/api/students');
                const students = await response.json();
                const tbody = document.getElementById('student-table-body');
                
                if (students.length === 0) {
                    tbody.innerHTML = `<tr><td colspan="4" class="px-6 py-4 text-center text-gray-500">No students found. Add one!</td></tr>`;
                    return;
                }

                tbody.innerHTML = students.map(student => `
                    <tr class="hover:bg-gray-50">
                        <td class="px-6 py-4 whitespace-nowrap">
                            <div class="font-medium text-gray-900">${student.name}</div>
                            <div class="text-sm text-gray-500">${student.email}</div>
                        </td>
                        <td class="px-6 py-4 whitespace-nowrap text-sm text-gray-500">${student.roll_number}</td>
                        <td class="px-6 py-4 whitespace-nowrap text-sm text-gray-500">
                            <span class="px-2 inline-flex text-xs leading-5 font-semibold rounded-full bg-blue-100 text-blue-800">
                                ${student.course}
                            </span>
                        </td>
                        <td class="px-6 py-4 whitespace-nowrap text-right text-sm font-medium">
                            <button onclick="editStudent(${student.id}, '${student.name}', '${student.roll_number}', '${student.email}', '${student.course}')" class="text-indigo-600 hover:text-indigo-900 mr-3"><i class="fa-solid fa-pen-to-square"></i> Edit</button>
                            <button onclick="deleteStudent(${student.id})" class="text-red-600 hover:text-red-900"><i class="fa-solid fa-trash"></i> Delete</button>
                        </td>
                    </tr>
                `).join('');
            } catch (error) {
                console.error("Error fetching data:", error);
            }
        }

        // Handle Add / Update
        form.addEventListener('submit', async (e) => {
            e.preventDefault();
            
            const id = document.getElementById('student-id').value;
            const data = {
                name: document.getElementById('name').value,
                roll_number: document.getElementById('roll').value,
                email: document.getElementById('email').value,
                course: document.getElementById('course').value
            };

            const method = id ? 'PUT' : 'POST';
            const url = id ? `/api/students/${id}` : '/api/students';

            try {
                const response = await fetch(url, {
                    method: method,
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(data)
                });

                if (!response.ok) {
                    const errorMsg = await response.text();
                    alert("Error: " + errorMsg);
                    return;
                }

                resetForm();
                fetchStudents();
            } catch (error) {
                console.error("Operation failed:", error);
            }
        });

        // Setup form for editing
        function editStudent(id, name, roll, email, course) {
            document.getElementById('student-id').value = id;
            document.getElementById('name').value = name;
            document.getElementById('roll').value = roll;
            document.getElementById('email').value = email;
            document.getElementById('course').value = course;
            
            formTitle.innerText = "Edit Student";
            submitBtn.innerText = "Update Student";
            submitBtn.classList.replace('bg-indigo-600', 'bg-green-600');
            submitBtn.classList.replace('hover:bg-indigo-700', 'hover:bg-green-700');
        }

        // Handle Delete
        async function deleteStudent(id) {
            if (confirm("Are you sure you want to delete this record?")) {
                await fetch(`/api/students/${id}`, { method: 'DELETE' });
                fetchStudents();
            }
        }

        // Reset Form UI
        function resetForm() {
            form.reset();
            document.getElementById('student-id').value = '';
            formTitle.innerText = "Add New Student";
            submitBtn.innerText = "Save Student";
            submitBtn.classList.replace('bg-green-600', 'bg-indigo-600');
            submitBtn.classList.replace('hover:bg-green-700', 'hover:bg-indigo-700');
        }

        // Initial load
        fetchStudents();
    </script>
</body>
</html>
"""

class StudentAPIHandler(http.server.SimpleHTTPRequestHandler):
    
    def send_json_response(self, status_code, data):
        self.send_response(status_code)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        self.wfile.write(json.dumps(data).encode('utf-8'))

    def send_error_response(self, status_code, message):
        self.send_response(status_code)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()
        self.wfile.write(message.encode('utf-8'))

    # Handle GET: Serve UI or Fetch DB Records
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            self.wfile.write(FRONTEND_HTML.encode('utf-8'))
            
        elif self.path == '/api/students':
            # SQL: Read all records
            records, error = execute_query("SELECT * FROM students ORDER BY id DESC", fetch=True)
            if error:
                self.send_error_response(500, error)
            else:
                self.send_json_response(200, records)
        else:
            self.send_error_response(404, "Not Found")

    # Handle POST: Create new DB Record
    def do_POST(self):
        if self.path == '/api/students':
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            data = json.loads(post_data.decode('utf-8'))
            
            # SQL: Insert record
            query = "INSERT INTO students (name, roll_number, email, course) VALUES (?, ?, ?, ?)"
            params = (data['name'], data['roll_number'], data['email'], data['course'])
            
            _, error = execute_query(query, params)
            if error:
                # E.g., if roll_number already exists (UNIQUE constraint)
                self.send_error_response(400, "Failed to add. Roll number might already exist.")
            else:
                self.send_json_response(201, {"message": "Student added successfully"})

    # Handle PUT: Update existing DB Record
    def do_PUT(self):
        if self.path.startswith('/api/students/'):
            student_id = self.path.split('/')[-1]
            content_length = int(self.headers['Content-Length'])
            put_data = self.rfile.read(content_length)
            data = json.loads(put_data.decode('utf-8'))
            
            # SQL: Update record
            query = "UPDATE students SET name=?, roll_number=?, email=?, course=? WHERE id=?"
            params = (data['name'], data['roll_number'], data['email'], data['course'], student_id)
            
            _, error = execute_query(query, params)
            if error:
                self.send_error_response(400, "Failed to update record.")
            else:
                self.send_json_response(200, {"message": "Student updated successfully"})

    # Handle DELETE: Remove DB Record
    def do_DELETE(self):
        if self.path.startswith('/api/students/'):
            student_id = self.path.split('/')[-1]
            
            # SQL: Delete record
            query = "DELETE FROM students WHERE id=?"
            _, error = execute_query(query, (student_id,))
            
            if error:
                self.send_error_response(500, "Failed to delete record.")
            else:
                self.send_json_response(200, {"message": "Student deleted successfully"})

if __name__ == "__main__":
    socketserver.TCPServer.allow_reuse_address = True
    
    with socketserver.TCPServer(("", PORT), StudentAPIHandler) as httpd:
        print(f"==================================================")
        print(f"🎓 Student Management Server running on port {PORT}")
        print(f"💾 SQLite Database file: {DB_FILE} initialized.")
        print(f"==================================================")
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nShutting down server.")
            httpd.server_close()