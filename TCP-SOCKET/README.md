# TCP Socket Program (C++)

This project demonstrates a simple **TCP client-server application** in C++ for **Windows (Winsock2)**.
It supports three features:

1. **Hello Message Exchange** – Client and server greet each other.
2. **File Transfer** – Client sends a file to the server.
3. **Calculator Service** – Client can send arithmetic expressions to the server, which computes and returns the result.

---

## ⚙️ Requirements

* Windows (tested on Windows 10/11)
* MinGW-w64 or MSVC compiler
* Winsock2 library (`-lws2_32` for MinGW)

---

## 📂 File Structure

```
/tcp-socket-app
│── server.cpp   # Server-side code
│── client.cpp   # Client-side code
│── README.md    # Project documentation
```

---

## 🛠️ Compilation

### Using MinGW (g++)

```bash
g++ server.cpp -o server.exe -lws2_32
g++ client.cpp -o client.exe -lws2_32
```

---

## ▶️ Running

### Start the server

```bash
server.exe
```

You should see:

```
[*] Server listening on port 5000
```

### Start the client

```bash
client.exe
```

---

## 💬 Features

### 1. Hello Exchange

* Client: `Hello from Client!`
* Server: `Hello from Server!`

### 2. File Transfer

* Place a file named `sample.txt` in the client directory.
* Client sends it to server.
* Server saves it as `received_sample.txt`.

### 3. Calculator

* Client can enter an expression like:

  ```
  calc 10 + 20
  ```
* Server responds:

  ```
  Result: 30
  ```

Supported operations:

* Addition (`+`)
* Subtraction (`-`)
* Multiplication (`*`)
* Division (`/`)

---

## 📸 Example Run

### Client

```
[+] Connected to server
Server says: Hello from Server!
Enter command (hello/file/calc/quit): calc 15 * 4
Server: Result: 60
```

### Server

```
[*] Server listening on port 5000
[+] Client connected
Client says: Hello from Client!
[Calculator] Expression received: 15 * 4
Result sent to client
```

---

## 🚀 Future Improvements

* Add authentication for clients.
* Support multiple clients simultaneously.
* Extend calculator to handle more complex expressions.

---

## 📜 License

This project is for **educational purposes**. Free to use and modify.
