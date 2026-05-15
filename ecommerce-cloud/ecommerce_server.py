import http.server
import socketserver
import json
import sqlite3
import os
from datetime import datetime

# Configuration
PORT = 8002
DB_FILE = 'ecommerce.db'

def init_db():
    """Initialize the SQLite database and seed it with sample products if empty."""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()
    
    # Create Products Table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS products (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            description TEXT NOT NULL,
            price REAL NOT NULL,
            image_url TEXT NOT NULL,
            stock INTEGER NOT NULL
        )
    ''')
    
    # Create Orders Table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS orders (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            product_id INTEGER,
            customer_name TEXT NOT NULL,
            quantity INTEGER NOT NULL,
            total_price REAL NOT NULL,
            order_date TEXT NOT NULL,
            FOREIGN KEY(product_id) REFERENCES products(id)
        )
    ''')
    
    # Seed Data: Insert sample products if the store is empty
    cursor.execute('SELECT COUNT(*) FROM products')
    if cursor.fetchone()[0] == 0:
        sample_products = [
            ('Premium Wireless Headphones', 'High fidelity audio with active noise cancellation and 30-hour battery life.', 199.99, 'https://images.unsplash.com/photo-1505740420928-5e560c06d30e?w=500&q=80', 50),
            ('Smart Fitness Watch', 'Track your health metrics, heart rate, and receive notifications on the go.', 149.50, 'https://images.unsplash.com/photo-1523275335684-37898b6baf30?w=500&q=80', 30),
            ('Mechanical Keyboard', 'RGB backlighting, tactile switches, and ergonomic design for coders.', 89.00, 'https://images.unsplash.com/photo-1595225476474-87563907a212?w=500&q=80', 15),
            ('Minimalist Desk Lamp', 'Adjustable color temperature and brightness with USB charging port.', 45.00, 'https://images.unsplash.com/photo-1507473885765-e6ed057f782c?w=500&q=80', 100)
        ]
        cursor.executemany('INSERT INTO products (name, description, price, image_url, stock) VALUES (?, ?, ?, ?, ?)', sample_products)
        print("Database seeded with sample products.")
        
    conn.commit()
    conn.close()

def execute_query(query, params=(), fetch=False, commit=False):
    """Helper function to execute SQL queries safely."""
    conn = sqlite3.connect(DB_FILE)
    conn.row_factory = sqlite3.Row 
    cursor = conn.cursor()
    try:
        cursor.execute(query, params)
        result = None
        if fetch:
            result = [dict(row) for row in cursor.fetchall()]
        if commit:
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
    <title>CloudMart E-Commerce</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <style>
        .modal-hidden { display: none; }
        .modal-active { display: flex; }
    </style>
</head>
<body class="bg-gray-100 min-h-screen font-sans">
    
    <!-- Navbar -->
    <nav class="bg-indigo-600 text-white shadow-lg sticky top-0 z-40">
        <div class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
            <div class="flex justify-between h-16">
                <div class="flex items-center">
                    <i class="fa-solid fa-store text-2xl mr-3"></i>
                    <span class="font-bold text-xl tracking-tight">CloudMart</span>
                </div>
                <div class="flex items-center space-x-4">
                    <button onclick="showSection('shop')" class="hover:bg-indigo-500 px-3 py-2 rounded-md font-medium transition-colors">Shop</button>
                    <button onclick="showSection('orders')" class="hover:bg-indigo-500 px-3 py-2 rounded-md font-medium transition-colors">Recent Orders</button>
                </div>
            </div>
        </div>
    </nav>

    <main class="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 py-8">
        
        <!-- Shop Section -->
        <div id="shop-section">
            <h2 class="text-3xl font-extrabold text-gray-900 mb-6">Featured Products</h2>
            <div id="products-grid" class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-6">
                <!-- Products injected by JS -->
                <div class="col-span-full text-center text-gray-500 py-10">Loading products...</div>
            </div>
        </div>

        <!-- Orders Section -->
        <div id="orders-section" class="hidden">
            <h2 class="text-3xl font-extrabold text-gray-900 mb-6">Recent Simulated Purchases</h2>
            <div class="bg-white rounded-xl shadow-sm border border-gray-200 overflow-hidden">
                <table class="min-w-full divide-y divide-gray-200">
                    <thead class="bg-gray-50">
                        <tr>
                            <th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase">Order ID</th>
                            <th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase">Customer</th>
                            <th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase">Product</th>
                            <th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase">Date</th>
                            <th class="px-6 py-3 text-right text-xs font-medium text-gray-500 uppercase">Total Paid</th>
                        </tr>
                    </thead>
                    <tbody id="orders-tbody" class="bg-white divide-y divide-gray-200">
                        <!-- Orders injected by JS -->
                    </tbody>
                </table>
            </div>
        </div>

    </main>

    <!-- Custom Checkout Modal (Replaces alerts/confirms) -->
    <div id="checkout-modal" class="fixed inset-0 bg-gray-900 bg-opacity-50 z-50 modal-hidden items-center justify-center p-4">
        <div class="bg-white rounded-xl shadow-2xl max-w-md w-full overflow-hidden">
            <div class="bg-indigo-600 px-6 py-4 flex justify-between items-center text-white">
                <h3 class="font-bold text-lg">Complete Purchase</h3>
                <button onclick="closeModal()" class="text-indigo-200 hover:text-white"><i class="fa-solid fa-xmark text-xl"></i></button>
            </div>
            <div class="p-6">
                <div class="mb-4 flex items-center gap-4">
                    <img id="modal-img" src="" class="w-16 h-16 object-cover rounded-md border">
                    <div>
                        <h4 id="modal-product-name" class="font-bold text-gray-800">Product Name</h4>
                        <p id="modal-product-price" class="text-indigo-600 font-semibold">$0.00</p>
                    </div>
                </div>
                
                <form id="checkout-form" class="space-y-4">
                    <input type="hidden" id="modal-product-id">
                    <div>
                        <label class="block text-sm font-medium text-gray-700 mb-1">Your Name</label>
                        <input type="text" id="customer-name" required placeholder="John Doe" class="w-full px-4 py-2 border border-gray-300 rounded-md focus:ring-2 focus:ring-indigo-500 focus:outline-none">
                    </div>
                    
                    <div id="modal-error" class="text-red-500 text-sm hidden bg-red-50 p-2 rounded"></div>
                    <div id="modal-success" class="text-green-600 text-sm hidden bg-green-50 p-2 rounded">Purchase successful! Refreshing...</div>

                    <div class="flex gap-3 pt-2">
                        <button type="button" onclick="closeModal()" class="flex-1 px-4 py-2 bg-gray-100 hover:bg-gray-200 text-gray-800 rounded-md transition-colors font-medium">Cancel</button>
                        <button type="submit" id="confirm-btn" class="flex-1 px-4 py-2 bg-indigo-600 hover:bg-indigo-700 text-white rounded-md transition-colors font-medium">Buy Now</button>
                    </div>
                </form>
            </div>
        </div>
    </div>

    <script>
        // --- UI Navigation ---
        function showSection(section) {
            document.getElementById('shop-section').style.display = section === 'shop' ? 'block' : 'none';
            document.getElementById('orders-section').style.display = section === 'orders' ? 'block' : 'none';
            if(section === 'orders') fetchOrders();
            if(section === 'shop') fetchProducts();
        }

        // --- Fetch Data ---
        async function fetchProducts() {
            try {
                const res = await fetch('/api/products');
                const products = await res.json();
                const grid = document.getElementById('products-grid');
                
                grid.innerHTML = products.map(p => `
                    <div class="bg-white rounded-xl shadow-md overflow-hidden hover:shadow-lg transition-shadow border border-gray-100 flex flex-col">
                        <img class="h-48 w-full object-cover" src="${p.image_url}" alt="${p.name}">
                        <div class="p-5 flex-1 flex flex-col">
                            <h3 class="text-lg font-bold text-gray-900 mb-1">${p.name}</h3>
                            <p class="text-sm text-gray-500 flex-1 mb-4">${p.description}</p>
                            
                            <div class="flex items-center justify-between mb-4">
                                <span class="text-2xl font-extrabold text-gray-900">$${p.price.toFixed(2)}</span>
                                <span class="text-xs font-semibold ${p.stock > 0 ? 'text-green-600 bg-green-100' : 'text-red-600 bg-red-100'} px-2 py-1 rounded-full">
                                    ${p.stock > 0 ? p.stock + ' in stock' : 'Out of Stock'}
                                </span>
                            </div>
                            
                            <button 
                                onclick="openCheckout(${p.id}, '${p.name.replace(/'/g, "\\'")}', ${p.price}, '${p.image_url}')" 
                                ${p.stock <= 0 ? 'disabled' : ''}
                                class="w-full bg-indigo-600 hover:bg-indigo-700 disabled:bg-gray-400 text-white font-bold py-2 px-4 rounded transition-colors flex justify-center items-center gap-2">
                                <i class="fa-solid fa-cart-shopping"></i> ${p.stock > 0 ? 'Buy Now' : 'Sold Out'}
                            </button>
                        </div>
                    </div>
                `).join('');
            } catch (err) { console.error(err); }
        }

        async function fetchOrders() {
            try {
                const res = await fetch('/api/orders');
                const orders = await res.json();
                const tbody = document.getElementById('orders-tbody');
                
                if (orders.length === 0) {
                    tbody.innerHTML = `<tr><td colspan="5" class="px-6 py-4 text-center text-gray-500">No purchases have been made yet.</td></tr>`;
                    return;
                }

                tbody.innerHTML = orders.map(o => `
                    <tr class="hover:bg-gray-50">
                        <td class="px-6 py-4 whitespace-nowrap text-sm text-gray-900 font-mono">#ORD-${o.id}</td>
                        <td class="px-6 py-4 whitespace-nowrap text-sm text-gray-900 font-medium"><i class="fa-solid fa-user-circle text-gray-400 mr-1"></i> ${o.customer_name}</td>
                        <td class="px-6 py-4 whitespace-nowrap text-sm text-gray-600">${o.product_name} (x${o.quantity})</td>
                        <td class="px-6 py-4 whitespace-nowrap text-sm text-gray-500">${o.order_date}</td>
                        <td class="px-6 py-4 whitespace-nowrap text-right text-sm font-bold text-green-600">$${o.total_price.toFixed(2)}</td>
                    </tr>
                `).join('');
            } catch (err) { console.error(err); }
        }

        // --- Modal Logic ---
        function openCheckout(id, name, price, img) {
            document.getElementById('modal-product-id').value = id;
            document.getElementById('modal-product-name').innerText = name;
            document.getElementById('modal-product-price').innerText = '$' + price.toFixed(2);
            document.getElementById('modal-img').src = img;
            
            document.getElementById('customer-name').value = '';
            document.getElementById('modal-error').classList.add('hidden');
            document.getElementById('modal-success').classList.add('hidden');
            document.getElementById('confirm-btn').disabled = false;
            
            const modal = document.getElementById('checkout-modal');
            modal.classList.remove('modal-hidden');
            modal.classList.add('modal-active');
        }

        function closeModal() {
            const modal = document.getElementById('checkout-modal');
            modal.classList.remove('modal-active');
            modal.classList.add('modal-hidden');
        }

        // --- Handle Purchase ---
        document.getElementById('checkout-form').addEventListener('submit', async (e) => {
            e.preventDefault();
            const btn = document.getElementById('confirm-btn');
            const errorDiv = document.getElementById('modal-error');
            const successDiv = document.getElementById('modal-success');
            
            btn.disabled = true;
            errorDiv.classList.add('hidden');

            const payload = {
                product_id: document.getElementById('modal-product-id').value,
                customer_name: document.getElementById('customer-name').value,
                quantity: 1
            };

            try {
                const response = await fetch('/api/orders', {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(payload)
                });

                if (!response.ok) {
                    const errorMsg = await response.text();
                    errorDiv.innerText = errorMsg;
                    errorDiv.classList.remove('hidden');
                    btn.disabled = false;
                    return;
                }

                successDiv.classList.remove('hidden');
                setTimeout(() => {
                    closeModal();
                    fetchProducts(); // Refresh stock
                }, 1000);

            } catch (error) {
                errorDiv.innerText = "Network error occurred.";
                errorDiv.classList.remove('hidden');
                btn.disabled = false;
            }
        });

        // Initialize
        fetchProducts();
    </script>
</body>
</html>
"""

class EcommerceAPIHandler(http.server.SimpleHTTPRequestHandler):
    
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

    # Handle GET Requests
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            self.wfile.write(FRONTEND_HTML.encode('utf-8'))
            
        elif self.path == '/api/products':
            # Fetch all products from DB
            records, error = execute_query("SELECT * FROM products ORDER BY id ASC", fetch=True)
            if error:
                self.send_error_response(500, error)
            else:
                self.send_json_response(200, records)
                
        elif self.path == '/api/orders':
            # Fetch recent orders joined with product names
            query = """
                SELECT o.id, o.customer_name, o.quantity, o.total_price, o.order_date, p.name as product_name 
                FROM orders o 
                JOIN products p ON o.product_id = p.id 
                ORDER BY o.id DESC
            """
            records, error = execute_query(query, fetch=True)
            if error:
                self.send_error_response(500, error)
            else:
                self.send_json_response(200, records)
        else:
            self.send_error_response(404, "Not Found")

    # Handle POST Requests (Simulating Purchase)
    def do_POST(self):
        if self.path == '/api/orders':
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            data = json.loads(post_data.decode('utf-8'))
            
            product_id = data.get('product_id')
            customer_name = data.get('customer_name')
            quantity = data.get('quantity', 1)
            
            # 1. Fetch product to check stock and get price
            product, error = execute_query("SELECT price, stock FROM products WHERE id = ?", (product_id,), fetch=True)
            if error or not product:
                self.send_error_response(404, "Product not found")
                return
                
            current_stock = product[0]['stock']
            price = product[0]['price']
            
            if current_stock < quantity:
                self.send_error_response(400, "Not enough stock available!")
                return
                
            # 2. Calculate Total
            total_price = price * quantity
            order_date = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            
            # 3. Use a direct connection for Transaction safety (Deduct stock AND Insert Order)
            conn = sqlite3.connect(DB_FILE)
            cursor = conn.cursor()
            try:
                # Deduct Stock
                cursor.execute("UPDATE products SET stock = stock - ? WHERE id = ?", (quantity, product_id))
                # Create Order
                cursor.execute(
                    "INSERT INTO orders (product_id, customer_name, quantity, total_price, order_date) VALUES (?, ?, ?, ?, ?)",
                    (product_id, customer_name, quantity, total_price, order_date)
                )
                conn.commit()
                self.send_json_response(201, {"message": "Purchase successful!"})
            except sqlite3.Error as e:
                conn.rollback()
                self.send_error_response(500, "Database transaction failed.")
            finally:
                conn.close()

if __name__ == "__main__":
    socketserver.TCPServer.allow_reuse_address = True
    
    with socketserver.TCPServer(("", PORT), EcommerceAPIHandler) as httpd:
        print(f"==================================================")
        print(f"🛒 E-Commerce Server running on port {PORT}")
        print(f"💾 SQLite Database file: {DB_FILE} initialized.")
        print(f"==================================================")
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nShutting down server.")
            httpd.server_close()