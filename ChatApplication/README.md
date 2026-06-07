# 💬 TCP Chat Application

A multi-client chat server built in **C** using **TCP sockets**, **pthreads**, and **epoll**.

## ✨ Features

* 👥 Multi-client support using **POSIX threads (`pthread`)**
* 📡 Real-time message broadcasting
* ⌨️ **Typing indicator** – when a user starts typing, other clients see `TYPING`
* ⚡ Efficient client-side I/O handling with **`epoll`**
* 🌐 Simple TCP-based communication protocol

---

##  Architecture

```text
Client 1 ──┐
Client 2 ──┼──► TCP Server (pthread per client) ──► Broadcast to all other clients
Client 3 ──┘
```

### 🖥️ Server

* Accepts multiple TCP client connections.
* Creates one thread (`pthread`) for each connected client.
* Maintains a shared client list protected by `pthread_mutex_t`.
* Broadcasts received messages to every other connected client.

### 💻 Client

* Connects to the TCP server.
* Uses `epoll` to monitor:

  *  the server socket (incoming messages),
  *  `stdin` (user keyboard input).
* Allows sending and receiving messages simultaneously without blocking.

---

## 📂 Project Structure

```text
.
├── server.c
├── client.c
└── README.md
```

---

## 🔨 Build

Compile the server:

```bash
gcc server.c -o server -lpthread
```

Compile the client:

```bash
gcc client.c -o client
```

---

## 🚀 Usage

Start the server:

```bash
./server
```

Open one or more terminals and start clients:

```bash
./client
```

After connecting, type messages and press **Enter** to broadcast them to all other connected clients.

---

## 📋 Commands

| Action                    | Description                         |
| ------------------------- | ----------------------------------- |
| ✍️ Type a message + Enter | Sends the message and broadcasts it |
| ❌ `Ctrl + C`              | Disconnects the client              |

---

## ⌨️ How the Typing Indicator Works

1. `epoll` detects that the client's keyboard (`stdin`) is ready.
2. The client sends the special message:

```text
TYPING
```

3. The server recognizes this command.
4. The server broadcasts:

```text
Գրում է...
```

to every other connected client.
5. After the user presses **Enter**, the actual message is sent and broadcast normally.

---

## ⚙️ Technical Details

*  **Language:** C
*  **Protocol:** Raw TCP
*  **Concurrency:** `pthread` (one thread per client)
*  **Thread Safety:** `pthread_mutex_t`
*  **Client I/O:** `epoll`
*  **Port:** `8080`

---

## 📌 Summary

This project demonstrates a simple multi-client chat application built with Linux system programming concepts:

*  TCP sockets
*  POSIX threads (`pthread`)
*  `epoll` for event-driven I/O
*  Mutex synchronization
*  Real-time message broadcasting
*  Typing status notifications
