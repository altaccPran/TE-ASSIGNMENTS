# DNS Lookup Program (C++)

This project demonstrates a simple **DNS lookup tool** written in C++ using sockets.
It supports:

1. **Hostname → IP** (Forward DNS lookup)
2. **IP → Hostname** (Reverse DNS lookup)

Cross-platform: works on **Windows (Winsock2)** and **Linux (POSIX sockets)**.

---

## ⚙️ Requirements

* Windows (tested on Windows 10/11) or Linux
* MinGW-w64 / MSVC (Windows) or g++ (Linux)
* Winsock2 library (`-lws2_32` on MinGW)

---

## 📂 File Structure

```
/dns-lookup-app
│── dns_lookup.cpp   # Source code
│── README.md        # Documentation
```

---

## 🛠️ Compilation

### On Linux / Ubuntu

```bash
g++ dns_lookup.cpp -o dns_lookup
./dns_lookup
```

### On Windows (MinGW)

```bash
g++ dns_lookup.cpp -o dns_lookup.exe -lws2_32
dns_lookup.exe
```

---

## ▶️ Running the Program

When run, the program asks whether to perform forward or reverse DNS lookup.

---

## 💬 Example Runs

### ✅ Hostname → IP (Google)

```
D:\Pranav CNSL temp>dns_lookup.exe
DNS Lookup Program
1. Hostname to IP
2. IP to Hostname
Enter choice: 1
Enter hostname (e.g. www.google.com): www.google.com
IP addresses for www.google.com:
  IPv6: 2404:6800:4009:828::2004
  IPv4: 142.250.192.36
```

### ✅ IP → Hostname (Google)

```
D:\Pranav CNSL temp>dns_lookup.exe
DNS Lookup Program
1. Hostname to IP
2. IP to Hostname
Enter choice: 2
Enter IP address (e.g. 8.8.8.8): 142.250.192.36
Hostname for 142.250.192.36: bom12s15-in-f4.1e100.net
```

### ✅ Hostname → IP (Reddit)

```
D:\Pranav CNSL temp>dns_lookup.exe
DNS Lookup Program
1. Hostname to IP
2. IP to Hostname
Enter choice: 1
Enter hostname (e.g. www.google.com): www.reddit.com
IP addresses for www.reddit.com:
  IPv6: 64:ff9b::a752:3d8c
  IPv6: 64:ff9b::a752:398c
  IPv4: 167.82.57.140
  IPv4: 167.82.61.140
```

### ⚠️ IP → Hostname (Reddit)

```
D:\Pranav CNSL temp>dns_lookup.exe
DNS Lookup Program
1. Hostname to IP
2. IP to Hostname
Enter choice: 2
Enter IP address (e.g. 8.8.8.8): 167.82.57.140
Could not resolve hostname for 167.82.57.140
```

*(Some IPs don’t have reverse DNS entries, so "Could not resolve hostname" is expected.)*

---

## 🚀 Future Improvements

* Support multiple IPs for reverse lookup
* Add IPv6 reverse DNS lookup
* Implement bulk lookups from file input

---

## 📜 License

This project is for **educational purposes**. Free to use and modify.
