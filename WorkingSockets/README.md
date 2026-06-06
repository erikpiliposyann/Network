# 🌐 TCP Client-Server Mini Projects (C / Pthreads)

This repository contains three simple **TCP client-server applications** written in C.  
Each project demonstrates basic socket programming, string processing, and file handling over raw TCP.

---

## 📌 1. Reverse String Server 🔁

### 🧾 Description
A client sends a string to the server, and the server returns the **reversed string**.

### ⚙️ Workflow
1. Client connects to server
2. Sends string
3. Server reverses it
4. Server sends back reversed result
5. Connection closes

### 📤 Example
**Client → Server:**
```
hello
```

**Server → Client:**
```
olleh
```

---

## 📌 2. File Transfer System 📂

### 🧾 Description
A client requests a file by name, and the server sends back the file content.

### ⚙️ Workflow
1. Client sends filename
2. Server checks file existence
3. Server reads file content
4. Sends content back to client
5. Connection closes

### 📤 Example

**Client → Server:**
```
GET file.txt
```

**Server → Client:**
```
(file content is sent as plain text)
```

## 📌 3. Palindrome Checker 🧠

### 🧾 Description
A client sends a string, and the server checks whether it is a **palindrome**.

### ⚙️ Workflow
1. Client sends string
2. Server checks reverse equality
3. Server sends result back

### 📤 Example

**Client → Server:**
```
madam
```

**Server → Client:**
```
YES
```

or

```
NO
```

---

## 🏗️ General System Architecture

All three projects follow the same structure:

### 🖥️ Server
- TCP socket server (`socket / bind / listen / accept`)
- Handles one request per connection (stateless model)
- Processes request and returns response

### 💻 Client
- Connects to server
- Sends request string
- Receives response
- Closes connection

---

## 🌐 Networking Details

- Protocol: **Raw TCP**
- Communication: **text-based**
- No HTTP headers or protocols
- One request → one response → connection close

---

## 🧠 Key Concepts Practiced

- Socket programming in C
- TCP client-server communication
- String manipulation
- File I/O operations
- Basic request-response protocol design
- Stateless server architecture

---

## 🚀 Summary

These mini projects demonstrate fundamental concepts of:
- network programming
- client-server architecture
- data processing over TCP

They are ideal for learning how low-level network services work in C.
