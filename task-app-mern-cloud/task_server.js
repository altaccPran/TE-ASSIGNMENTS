const express = require('express');
const mongoose = require('mongoose');

const app = express();
const PORT = 8004;

// Middleware to parse JSON bodies
app.use(express.json());

// Connect to local MongoDB instance
mongoose.connect('mongodb://127.0.0.1:27017/taskmanager')
    .then(() => console.log('✅ MongoDB Connected Successfully'))
    .catch(err => console.error('❌ MongoDB Connection Error:', err));

// Define the Task Schema and Model
const taskSchema = new mongoose.Schema({
    title: { type: String, required: true },
    description: { type: String, required: true },
    status: { 
        type: String, 
        enum: ['Pending', 'In Progress', 'Completed'], 
        default: 'Pending' 
    },
    createdAt: { type: Date, default: Date.now }
});

const Task = mongoose.model('Task', taskSchema);

// --- REST API ROUTES ---

// GET: Retrieve all tasks
app.get('/api/tasks', async (req, res) => {
    try {
        const tasks = await Task.find().sort({ createdAt: -1 });
        res.json(tasks);
    } catch (err) {
        res.status(500).json({ error: 'Failed to fetch tasks' });
    }
});

// POST: Create a new task
app.post('/api/tasks', async (req, res) => {
    try {
        const newTask = new Task({
            title: req.body.title,
            description: req.body.description,
            status: req.body.status || 'Pending'
        });
        const savedTask = await newTask.save();
        res.status(201).json(savedTask);
    } catch (err) {
        res.status(400).json({ error: 'Failed to create task' });
    }
});

// PUT: Update an existing task (e.g., changing status)
app.put('/api/tasks/:id', async (req, res) => {
    try {
        const updatedTask = await Task.findByIdAndUpdate(
            req.params.id, 
            { $set: req.body }, 
            { new: true } // Return the updated document
        );
        res.json(updatedTask);
    } catch (err) {
        res.status(400).json({ error: 'Failed to update task' });
    }
});

// DELETE: Remove a task
app.delete('/api/tasks/:id', async (req, res) => {
    try {
        await Task.findByIdAndDelete(req.params.id);
        res.json({ message: 'Task deleted successfully' });
    } catch (err) {
        res.status(400).json({ error: 'Failed to delete task' });
    }
});

// --- FRONTEND ---
// The Express server will serve this HTML/React string to the browser
const FRONTEND_HTML = `
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>MERN Task Manager</title>
    
    <!-- Load Tailwind CSS -->
    <script src="https://cdn.tailwindcss.com"></script>
    <!-- Load FontAwesome -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    
    <!-- Load React, ReactDOM, and Babel -->
    <script src="https://unpkg.com/react@18/umd/react.production.min.js" crossorigin></script>
    <script src="https://unpkg.com/react-dom@18/umd/react-dom.production.min.js" crossorigin></script>
    <script src="https://unpkg.com/@babel/standalone/babel.min.js"></script>
</head>
<body class="bg-slate-50 min-h-screen font-sans text-slate-800">
    <div id="root"></div>

    <script type="text/babel">
        const { useState, useEffect } = React;

        function App() {
            const [tasks, setTasks] = useState([]);
            const [title, setTitle] = useState('');
            const [description, setDescription] = useState('');
            const [loading, setLoading] = useState(true);

            // Fetch tasks on component mount
            useEffect(() => {
                fetchTasks();
            }, []);

            const fetchTasks = async () => {
                setLoading(true);
                try {
                    const res = await fetch('/api/tasks');
                    const data = await res.json();
                    setTasks(data);
                } catch (err) {
                    console.error("Error fetching tasks:", err);
                } finally {
                    setLoading(false);
                }
            };

            const handleSubmit = async (e) => {
                e.preventDefault();
                if (!title || !description) return;

                try {
                    await fetch('/api/tasks', {
                        method: 'POST',
                        headers: { 'Content-Type': 'application/json' },
                        body: JSON.stringify({ title, description })
                    });
                    setTitle('');
                    setDescription('');
                    fetchTasks();
                } catch (err) {
                    console.error("Error creating task:", err);
                }
            };

            const updateStatus = async (id, currentStatus) => {
                // Cycle through statuses: Pending -> In Progress -> Completed -> Pending
                const statuses = ['Pending', 'In Progress', 'Completed'];
                const nextIndex = (statuses.indexOf(currentStatus) + 1) % statuses.length;
                const newStatus = statuses[nextIndex];

                try {
                    await fetch(\`/api/tasks/\${id}\`, {
                        method: 'PUT',
                        headers: { 'Content-Type': 'application/json' },
                        body: JSON.stringify({ status: newStatus })
                    });
                    fetchTasks();
                } catch (err) {
                    console.error("Error updating status:", err);
                }
            };

            const deleteTask = async (id) => {
                // Using inline UI interaction instead of browser confirm()
                try {
                    await fetch(\`/api/tasks/\${id}\`, { method: 'DELETE' });
                    fetchTasks();
                } catch (err) {
                    console.error("Error deleting task:", err);
                }
            };

            const getStatusColor = (status) => {
                switch(status) {
                    case 'Completed': return 'bg-emerald-100 text-emerald-800 border-emerald-200';
                    case 'In Progress': return 'bg-amber-100 text-amber-800 border-amber-200';
                    default: return 'bg-slate-100 text-slate-800 border-slate-200';
                }
            };

            return (
                <div class="max-w-4xl mx-auto p-4 md:p-8">
                    <header class="mb-8 flex items-center justify-between bg-white p-6 rounded-2xl shadow-sm border border-slate-100">
                        <div>
                            <h1 class="text-2xl font-black text-indigo-600 tracking-tight">
                                <i class="fa-solid fa-layer-group mr-2"></i>MERN TaskFlow
                            </h1>
                            <p class="text-slate-500 text-sm mt-1 font-medium">MongoDB, Express, React, Node.js</p>
                        </div>
                        <div class="text-right">
                            <span class="bg-indigo-50 text-indigo-600 px-3 py-1 rounded-full text-xs font-bold border border-indigo-100">
                                {tasks.length} Total Tasks
                            </span>
                        </div>
                    </header>

                    <div class="grid grid-cols-1 lg:grid-cols-3 gap-8">
                        {/* Form Section */}
                        <div class="lg:col-span-1">
                            <div class="bg-white p-6 rounded-2xl shadow-sm border border-slate-100 sticky top-8">
                                <h2 class="text-lg font-bold mb-4 text-slate-800">Add New Task</h2>
                                <form onSubmit={handleSubmit} class="space-y-4">
                                    <div>
                                        <label class="block text-xs font-bold text-slate-500 uppercase tracking-wide mb-1">Title</label>
                                        <input 
                                            type="text" 
                                            value={title}
                                            onChange={(e) => setTitle(e.target.value)}
                                            placeholder="e.g., Update Database Schema" 
                                            class="w-full px-4 py-2 border border-slate-200 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:outline-none transition-shadow"
                                            required
                                        />
                                    </div>
                                    <div>
                                        <label class="block text-xs font-bold text-slate-500 uppercase tracking-wide mb-1">Description</label>
                                        <textarea 
                                            value={description}
                                            onChange={(e) => setDescription(e.target.value)}
                                            placeholder="Details about the task..." 
                                            rows="4"
                                            class="w-full px-4 py-2 border border-slate-200 rounded-lg focus:ring-2 focus:ring-indigo-500 focus:outline-none transition-shadow resize-none"
                                            required
                                        ></textarea>
                                    </div>
                                    <button 
                                        type="submit" 
                                        class="w-full bg-indigo-600 hover:bg-indigo-700 text-white font-bold py-3 px-4 rounded-lg transition-colors shadow-sm"
                                    >
                                        Create Task
                                    </button>
                                </form>
                            </div>
                        </div>

                        {/* List Section */}
                        <div class="lg:col-span-2 space-y-4">
                            {loading ? (
                                <div class="text-center py-12 text-slate-400 font-medium">
                                    <i class="fa-solid fa-circle-notch fa-spin text-3xl mb-3 text-indigo-400"></i>
                                    <p>Loading tasks from MongoDB...</p>
                                </div>
                            ) : tasks.length === 0 ? (
                                <div class="bg-white p-12 rounded-2xl border border-slate-100 text-center shadow-sm">
                                    <div class="text-slate-300 mb-4"><i class="fa-solid fa-clipboard-check text-5xl"></i></div>
                                    <h3 class="text-lg font-bold text-slate-700 mb-1">All caught up!</h3>
                                    <p class="text-slate-500">You have no pending tasks. Add one to get started.</p>
                                </div>
                            ) : (
                                tasks.map(task => (
                                    <div key={task._id} class="bg-white p-5 rounded-2xl shadow-sm border border-slate-100 flex flex-col sm:flex-row gap-4 justify-between items-start hover:shadow-md transition-shadow group">
                                        <div class="flex-1">
                                            <div class="flex items-center gap-3 mb-1">
                                                <h3 class="text-lg font-bold text-slate-800">{task.title}</h3>
                                                <button 
                                                    onClick={() => updateStatus(task._id, task.status)}
                                                    class={\`text-xs px-2.5 py-1 rounded-full font-bold border cursor-pointer hover:opacity-80 transition-opacity \${getStatusColor(task.status)}\`}
                                                    title="Click to change status"
                                                >
                                                    {task.status}
                                                </button>
                                            </div>
                                            <p class="text-slate-600 text-sm mb-2">{task.description}</p>
                                            <p class="text-xs text-slate-400 font-medium"><i class="fa-regular fa-clock mr-1"></i> {new Date(task.createdAt).toLocaleString()}</p>
                                        </div>
                                        <button 
                                            onClick={() => deleteTask(task._id)}
                                            class="text-slate-400 hover:text-red-500 bg-slate-50 hover:bg-red-50 p-2.5 rounded-lg transition-colors"
                                            title="Delete Task"
                                        >
                                            <i class="fa-solid fa-trash-can"></i>
                                        </button>
                                    </div>
                                ))
                            )}
                        </div>
                    </div>
                </div>
            );
        }

        const root = ReactDOM.createRoot(document.getElementById('root'));
        root.render(<App />);
    </script>
</body>
</html>
`;

// Serve the Frontend HTML on the root route
app.get('/', (req, res) => {
    res.send(FRONTEND_HTML);
});

// Start the Express server
app.listen(PORT, () => {
    console.log(`==================================================`);
    console.log(`🚀 MERN Server running on port ${PORT}`);
    console.log(`⚛️  React frontend injected and active.`);
    console.log(`==================================================`);
});