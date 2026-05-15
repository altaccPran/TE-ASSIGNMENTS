const express = require('express');
const mongoose = require('mongoose');

const app = express();
const PORT = 8005;

// Middleware to parse JSON bodies
app.use(express.json());

// Connect to local MongoDB instance on the Ubuntu Server
mongoose.connect('mongodb://127.0.0.1:27017/eventdb')
    .then(() => console.log('✅ MongoDB Connected Successfully'))
    .catch(err => console.error('❌ MongoDB Connection Error:', err));

// Define the Registration Schema and Model
const registrationSchema = new mongoose.Schema({
    name: { type: String, required: true },
    email: { type: String, required: true },
    ticketType: { 
        type: String, 
        enum: ['Standard', 'VIP', 'Student'], 
        default: 'Standard' 
    },
    registrationDate: { type: Date, default: Date.now }
});

const Registration = mongoose.model('Registration', registrationSchema);

// --- REST API ROUTES ---

// GET: Retrieve all registrations
app.get('/api/registrations', async (req, res) => {
    try {
        const registrations = await Registration.find().sort({ registrationDate: -1 });
        res.json(registrations);
    } catch (err) {
        res.status(500).json({ error: 'Failed to fetch registrations' });
    }
});

// POST: Create a new event registration
app.post('/api/registrations', async (req, res) => {
    try {
        const newReg = new Registration({
            name: req.body.name,
            email: req.body.email,
            ticketType: req.body.ticketType || 'Standard'
        });
        const savedReg = await newReg.save();
        res.status(201).json(savedReg);
    } catch (err) {
        res.status(400).json({ error: 'Failed to register' });
    }
});

// DELETE: Cancel/Remove a registration
app.delete('/api/registrations/:id', async (req, res) => {
    try {
        await Registration.findByIdAndDelete(req.params.id);
        res.json({ message: 'Registration cancelled successfully' });
    } catch (err) {
        res.status(400).json({ error: 'Failed to cancel registration' });
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
    <title>TechConf 2026 Registration</title>
    
    <!-- Load Tailwind CSS -->
    <script src="https://cdn.tailwindcss.com"></script>
    <!-- Load FontAwesome -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    
    <!-- Load React, ReactDOM, and Babel -->
    <script src="https://unpkg.com/react@18/umd/react.production.min.js" crossorigin></script>
    <script src="https://unpkg.com/react-dom@18/umd/react-dom.production.min.js" crossorigin></script>
    <script src="https://unpkg.com/@babel/standalone/babel.min.js"></script>
</head>
<body class="bg-slate-900 min-h-screen font-sans text-slate-100">
    <div id="root"></div>

    <script type="text/babel">
        const { useState, useEffect } = React;

        function App() {
            const [registrations, setRegistrations] = useState([]);
            const [formData, setFormData] = useState({ name: '', email: '', ticketType: 'Standard' });
            const [loading, setLoading] = useState(true);
            const [successMsg, setSuccessMsg] = useState('');

            // Fetch registrations on component mount
            useEffect(() => {
                fetchRegistrations();
            }, []);

            const fetchRegistrations = async () => {
                setLoading(true);
                try {
                    const res = await fetch('/api/registrations');
                    const data = await res.json();
                    setRegistrations(data);
                } catch (err) {
                    console.error("Error fetching data:", err);
                } finally {
                    setLoading(false);
                }
            };

            const handleSubmit = async (e) => {
                e.preventDefault();
                if (!formData.name || !formData.email) return;

                try {
                    await fetch('/api/registrations', {
                        method: 'POST',
                        headers: { 'Content-Type': 'application/json' },
                        body: JSON.stringify(formData)
                    });
                    
                    setSuccessMsg('Registration successful! See you at the event.');
                    setFormData({ name: '', email: '', ticketType: 'Standard' });
                    fetchRegistrations();
                    
                    // Clear success message after 3 seconds
                    setTimeout(() => setSuccessMsg(''), 3000);
                } catch (err) {
                    console.error("Error submitting registration:", err);
                }
            };

            const handleCancel = async (id) => {
                try {
                    await fetch(\`/api/registrations/\${id}\`, { method: 'DELETE' });
                    fetchRegistrations();
                } catch (err) {
                    console.error("Error cancelling registration:", err);
                }
            };

            const getTicketColor = (type) => {
                switch(type) {
                    case 'VIP': return 'bg-amber-500/20 text-amber-400 border-amber-500/30';
                    case 'Student': return 'bg-blue-500/20 text-blue-400 border-blue-500/30';
                    default: return 'bg-emerald-500/20 text-emerald-400 border-emerald-500/30';
                }
            };

            return (
                <div class="max-w-6xl mx-auto p-4 md:p-8">
                    
                    {/* Event Hero Header */}
                    <div class="relative rounded-3xl overflow-hidden mb-12 bg-gradient-to-br from-indigo-900 to-purple-900 border border-indigo-500/30 shadow-2xl">
                        <div class="absolute inset-0 bg-[url('https://images.unsplash.com/photo-1540575467063-178a50c2df87?q=80&w=2000&auto=format&fit=crop')] opacity-20 mix-blend-overlay bg-cover bg-center"></div>
                        <div class="relative z-10 p-10 md:p-16 text-center">
                            <span class="inline-block py-1 px-3 rounded-full bg-indigo-500/30 text-indigo-300 border border-indigo-400/30 text-sm font-bold tracking-widest uppercase mb-4">Live Event</span>
                            <h1 class="text-4xl md:text-6xl font-black text-white mb-4 tracking-tight">Global TechConf 2026</h1>
                            <p class="text-lg md:text-xl text-indigo-200 max-w-2xl mx-auto mb-8">Join thousands of developers, innovators, and industry leaders for three days of deep technical dives and networking.</p>
                            <div class="flex justify-center gap-6 text-indigo-100 text-sm font-medium">
                                <div><i class="fa-solid fa-calendar-day mr-2 text-indigo-400"></i> Oct 15-17, 2026</div>
                                <div><i class="fa-solid fa-location-dot mr-2 text-indigo-400"></i> San Francisco, CA</div>
                            </div>
                        </div>
                    </div>

                    <div class="grid grid-cols-1 lg:grid-cols-12 gap-8">
                        {/* Registration Form */}
                        <div class="lg:col-span-4">
                            <div class="bg-slate-800 p-6 rounded-2xl border border-slate-700 shadow-xl sticky top-8">
                                <h2 class="text-xl font-bold text-white mb-6 flex items-center">
                                    <i class="fa-solid fa-ticket mr-3 text-purple-500"></i> Secure Your Spot
                                </h2>
                                
                                {successMsg && (
                                    <div class="bg-emerald-500/20 border border-emerald-500/50 text-emerald-400 px-4 py-3 rounded-lg mb-6 text-sm font-medium flex items-center">
                                        <i class="fa-solid fa-circle-check mr-2"></i> {successMsg}
                                    </div>
                                )}

                                <form onSubmit={handleSubmit} class="space-y-5">
                                    <div>
                                        <label class="block text-sm font-medium text-slate-400 mb-1.5">Full Name</label>
                                        <input 
                                            type="text" 
                                            value={formData.name}
                                            onChange={(e) => setFormData({...formData, name: e.target.value})}
                                            placeholder="Jane Doe" 
                                            class="w-full px-4 py-3 bg-slate-900 border border-slate-700 text-white rounded-xl focus:ring-2 focus:ring-purple-500 focus:border-transparent transition-all outline-none"
                                            required
                                        />
                                    </div>
                                    <div>
                                        <label class="block text-sm font-medium text-slate-400 mb-1.5">Email Address</label>
                                        <input 
                                            type="email" 
                                            value={formData.email}
                                            onChange={(e) => setFormData({...formData, email: e.target.value})}
                                            placeholder="jane@example.com" 
                                            class="w-full px-4 py-3 bg-slate-900 border border-slate-700 text-white rounded-xl focus:ring-2 focus:ring-purple-500 focus:border-transparent transition-all outline-none"
                                            required
                                        />
                                    </div>
                                    <div>
                                        <label class="block text-sm font-medium text-slate-400 mb-1.5">Ticket Tier</label>
                                        <select 
                                            value={formData.ticketType}
                                            onChange={(e) => setFormData({...formData, ticketType: e.target.value})}
                                            class="w-full px-4 py-3 bg-slate-900 border border-slate-700 text-white rounded-xl focus:ring-2 focus:ring-purple-500 focus:border-transparent transition-all outline-none appearance-none"
                                        >
                                            <option value="Standard">Standard Pass ($299)</option>
                                            <option value="VIP">VIP Access ($899)</option>
                                            <option value="Student">Student/Academic ($99)</option>
                                        </select>
                                    </div>
                                    <button 
                                        type="submit" 
                                        class="w-full bg-gradient-to-r from-purple-600 to-indigo-600 hover:from-purple-500 hover:to-indigo-500 text-white font-bold py-3.5 px-4 rounded-xl transition-all shadow-lg shadow-purple-500/25 mt-4"
                                    >
                                        Complete Registration
                                    </button>
                                </form>
                            </div>
                        </div>

                        {/* Attendee List Section */}
                        <div class="lg:col-span-8">
                            <div class="bg-slate-800 rounded-2xl border border-slate-700 shadow-xl overflow-hidden">
                                <div class="p-6 border-b border-slate-700 flex justify-between items-center bg-slate-800/50">
                                    <h2 class="text-xl font-bold text-white">Live Attendee Roster</h2>
                                    <span class="bg-slate-900 text-slate-300 px-3 py-1 rounded-full text-sm font-bold border border-slate-700">
                                        {registrations.length} Registered
                                    </span>
                                </div>
                                
                                <div class="p-6">
                                    {loading ? (
                                        <div class="text-center py-12 text-slate-500">
                                            <i class="fa-solid fa-spinner fa-spin text-3xl mb-3 text-purple-500"></i>
                                            <p>Syncing with MongoDB database...</p>
                                        </div>
                                    ) : registrations.length === 0 ? (
                                        <div class="text-center py-12 bg-slate-900/50 rounded-xl border border-dashed border-slate-700">
                                            <i class="fa-regular fa-id-badge text-5xl text-slate-600 mb-4"></i>
                                            <h3 class="text-lg font-bold text-slate-300 mb-1">No attendees yet</h3>
                                            <p class="text-slate-500">Be the first to register for TechConf 2026!</p>
                                        </div>
                                    ) : (
                                        <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
                                            {registrations.map(reg => (
                                                <div key={reg._id} class="bg-slate-900 p-4 rounded-xl border border-slate-700 flex flex-col justify-between group hover:border-slate-500 transition-colors">
                                                    <div>
                                                        <div class="flex justify-between items-start mb-2">
                                                            <h3 class="font-bold text-white text-lg truncate pr-2">{reg.name}</h3>
                                                            <span class={\`text-xs px-2.5 py-1 rounded-full font-bold border whitespace-nowrap \${getTicketColor(reg.ticketType)}\`}>
                                                                {reg.ticketType}
                                                            </span>
                                                        </div>
                                                        <p class="text-slate-400 text-sm truncate"><i class="fa-regular fa-envelope mr-1.5 opacity-50"></i>{reg.email}</p>
                                                    </div>
                                                    
                                                    <div class="mt-4 pt-4 border-t border-slate-800 flex justify-between items-center">
                                                        <span class="text-xs text-slate-500">
                                                            Registered: {new Date(reg.registrationDate).toLocaleDateString()}
                                                        </span>
                                                        <button 
                                                            onClick={() => handleCancel(reg._id)}
                                                            class="text-xs text-slate-500 hover:text-red-400 transition-colors px-2 py-1"
                                                            title="Cancel Registration"
                                                        >
                                                            <i class="fa-solid fa-xmark mr-1"></i> Cancel
                                                        </button>
                                                    </div>
                                                </div>
                                            ))}
                                        </div>
                                    )}
                                </div>
                            </div>
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
    console.log(`🎟️  Event Registration Server running on port ${PORT}`);
    console.log(`⚛️  React frontend injected and active.`);
    console.log(`==================================================`);
});