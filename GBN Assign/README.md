# Go-Back-N & Selective Repeat ARQ Simulation

This C++ program simulates **Automatic Repeat reQuest (ARQ)** protocols—  
specifically **Go-Back-N (GBN)** and **Selective Repeat (SR)**—in a **peer-to-peer** communication model.  
It models a bidirectional data link where **two peers** (`PeerA` and `PeerB`) simultaneously send and receive frames.

---

## ✨ Features

- **Two-Way Transmission:** Both peers send and receive frames concurrently.
- **Protocol Choice:** Choose either **Go-Back-N** or **Selective Repeat** at runtime.
- **Random Frame/ACK Loss:**  
  - Each frame or acknowledgement has a **20% probability of being lost**, simulating a noisy channel.
- **Sliding Window Mechanism:**  
  - **GBN:** Window size = \(2^m - 1\)  
  - **SR:** Window size = \(2^{m-1}\)
- **Timeout & Retransmission:**  
  - Lost frames trigger **timeouts** and **retransmissions**.
- **Interactive Window Shift:**  
  - Whenever the sender’s window slides forward, the user is asked whether to continue (`y/n`).

---

## 🏗️ How It Works

1. **Setup:**
   - User inputs:
     - `m`: number of bits for sequence numbers.
     - Protocol choice: `1` for Go-Back-N or `2` for Selective Repeat.
   - Sequence number range = \(2^m\).
   - Total frames = one full sequence cycle (\(2^m\) frames per peer).

2. **Transmission Loop:**
   - Each peer checks if it can send a new frame within its window.
   - If yes, it sends; if not, a **timeout** triggers retransmission of pending frames.

3. **Reception:**
   - Receiver logs received frames.
   - ACKs may be lost randomly.
   - **GBN:** Out-of-order frames are discarded, and a NAK is sent for the expected frame.
   - **SR:** Frames are buffered, and NAKs are sent only for missing frames.

4. **Acknowledgement & Window Slide:**
   - When a frame is acknowledged, the sender’s window slides.
   - If the window slides at least one position, the user decides whether to continue.

5. **Completion:**
   - Simulation ends once **all frames of both peers** are acknowledged.

---

## 📂 File Structure

```
├── gbn.cpp # Main simulation code (this file)
└── README.md # Documentation
```


---

## ▶️ Compilation & Execution

### Prerequisites
- **g++** (C++17 or newer)
- Linux/Unix environment (for `usleep`), or adapt delay logic for Windows.

### Steps
```bash
g++ -std=c++17 gbn.cpp -o gbn
./gbn
```
