# 🌐 TCP Multithreaded Client-Server Systems (C / Pthreads)

This repository contains a set of **multithreaded TCP client-server applications** written in C using **POSIX sockets and pthreads**.

The projects demonstrate:
- multi-client server architecture
- thread-per-client model
- real-time TCP communication
- stateful session handling
- basic timeout handling using socket options

---

## 📌 Projects Overview

This repository includes 3 main systems:

1.  Multi-Client TCP Server
2.  Number Guessing Game
3.  TCP Exam System with Timed Questions

---

##  1. Multi-Client TCP Server System

### 📌 Description
A basic **multithreaded TCP server** that handles multiple clients simultaneously.  
Each client connection is processed in a separate thread.

###  Features
- Multiple concurrent client connections
- Thread-per-client architecture
- Independent client sessions
- Safe parallel execution using pthreads

---

##  2. Number Guessing Game (TCP Interactive System)

### 📌 Description
The server generates a **random number** for each client.  
The client tries to guess it, and the server responds after each attempt.

### 📤 Server Responses
- `"Too high"` 📉  
- `"Too low"` 📈  
- `"Correct"` 🎯  

---

### ⚙️ Flow
1. Client connects to server
2. Server generates random number
3. Client sends guesses
4. Server responds after each guess
5. Loop continues until correct answer
6. Connection closes

---

##  3. TCP Exam System (Multi-Client Timed Quiz)

### 📌 Description
A **multi-client exam system** where each client receives a test session.

The server:
- stores exam questions
- sends questions one by one
- receives answers
- calculates final score
- sends result back to client

Each client runs in its own thread.

---

##  System Architecture

### 🖥️ Server
- Multi-threaded (`pthread`)
- Handles multiple clients at the same time
- Sends questions sequentially per client
- Evaluates answers in real time

### 💻 Client
- Connects to server
- Receives questions one by one
- Sends answers immediately
- Receives final score

---

## 📚 Question Structure

```c
typedef struct {
    char question[256];
    char optionA[64];
    char optionB[64];
    char optionC[64];
    int correct_answer;
} Question;
```

---

## ⚙️ Exam Flow

1. Client connects to server
2. Server creates a dedicated thread
3. Server sends questions sequentially
4. Client answers each question
5. Server checks answers immediately
6. Score is calculated
7. Final result is sent
8. Connection is closed

---

## ⏱️ Time Limit Feature

Each question has a limited time to answer.

### If client does not respond in time:
- Answer is marked as invalid
- No points are awarded
- Server continues to next question

### Implementation approach used:
- `SO_RCVTIMEO` socket option (receive timeout)

---

##  Networking Model

- Protocol: Raw TCP
- Architecture: Thread-per-client
- Communication style: Request / Response
- Session type: Stateful per connection

---

##  Concurrency Model

- Each client handled in a separate thread
- No shared execution between clients
- Mutex used only where shared resources exist
- Fully parallel client handling

---

## 🧠 Key Concepts

- TCP socket programming in C
- Multithreading with pthreads
- Client-server communication
- Stateful session handling
- Random number generation
- Exam evaluation system
- Socket timeout handling (`SO_RCVTIMEO`)

---

## 🚀 Summary

This project demonstrates:

-  multi-client TCP server design  
-  interactive guessing game system  
-  timed exam system over TCP  

All implemented using **raw sockets + pthreads**, without advanced multiplexing techniques like `poll` or `epoll`.
