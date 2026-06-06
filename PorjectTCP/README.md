# 🗄️ TCP Product Database Engine

This project is a **multi-threaded TCP client-server system** for managing a simple product catalog stored in a binary file. The server supports basic CRUD-like operations over raw TCP connections and ensures thread-safe access to the database using mutexes.

---

## 📌 1. Overview

The system implements a **stateless TCP-based product database**:

- The server stores product data in a local binary file (`products.db`)
- Clients communicate using simple text commands
- Each client connection handles exactly **one request** and then closes

---

##  2. System Architecture

The system consists of three main components:

### 🖥️ TCP Server
- Multi-threaded (`pthread`)
- Listens on port `8080`
- Handles each client in a separate thread
- Ensures thread-safe file access using a mutex

### 💻 TCP Client
- Sends a single command per connection
- Receives response from server
- Connection is closed immediately after response

### Database File
- File: `products.db`
- Stores fixed-size binary structures

---

## 3. Data Model

### Product Structure
```c
typedef struct {
    char name[50];
} Product;
```

### Storage Properties
- Fixed-size records (`sizeof(Product)`)
- Binary file storage
- Offset-based access using `fread / fwrite`
- No text parsing required

---

## ⚙️ 4. Functional Requirements

---

###  4.1 POST – Add Product

**Command:**
```
POST <product_name>
```

**Server Behavior:**
1. Lock mutex (`pthread_mutex_lock`)
2. Open `products.db` in append mode
3. Write product structure to file
4. Unlock mutex

**Response:**
```
Product successfully added
```

---

### 🔍 4.2 GET <name> – Get Specific Product

**Command:**
```
GET <product_name>
```

**Server Behavior:**
1. Lock file (mutex)
2. Read file block-by-block (`sizeof(Product)`)
3. Compare product names
4. Stop when match is found

**Response:**
- If found:
```
<product_name>
```
- If not found:
```
Not found product
```

---

### 📋 4.3 GET – Get All Products

**Command:**
```
GET
```

**Server Behavior:**
1. Lock file (mutex)
2. Iterate through all records
3. Send each product name to client

**Response:**
```
product1
product2
product3
...
```

If database is empty:
```
Empty
```

---

## 🔒 5. Concurrency & Thread Safety

- Each client is handled in a separate `pthread`
- Shared resource: `products.db`
- Protected using:
```c
pthread_mutex_t mutex;
```

Ensures:
- No race conditions during POST
- Safe concurrent reads

---

## 🌐 6. Networking Details

- Protocol: Raw TCP
- Port: `8080`
- Socket API: `socket / bind / listen / accept`
- Each request handled independently (stateless design)

---

##  7. Fault Tolerance

- Database file is automatically created on startup:
```c
fopen("products.db", "ab+");
```

- Invalid client connections are safely handled
- Server continues running even if a thread fails

---

## ⏱️ 8. Client Reliability

- Client must use `SO_RCVTIMEO`
- Prevents blocking in case of:
  - Network failure
  - Server delay
  - Lost response

---

## 🎯 9. Key Features

-  Multi-threaded server (pthread)
-  Mutex-protected database access
-  Binary file storage (fixed-size records)
-  Fast search using sequential block reading
-  Raw TCP communication
-  Stateless request handling

---

## 🚀 Summary

This project demonstrates a lightweight **distributed database system over TCP**, focusing on:

- concurrency control
- file-based storage design
- socket programming
- safe multi-threaded access

---
