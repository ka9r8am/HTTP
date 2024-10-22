# Simple HTTP Server in C

A minimal HTTP server in C that handles **GET** and **POST** requests.

## Features
- Supports HTTP GET and POST methods.
- Simple HTML responses.
- Echoes POST request data.

## Prerequisites
- **GCC** or any C compiler.

## How to Build and Run

1. **Clone the repo:**
   ```bash
   git clone <repository-url>
   cd simple-http-server
   ```

2. **Compile:**
   ```bash
   gcc -o http_server http_server.c
   ```

3. **Run:**
   ```bash
   ./http_server
   ```
   - For **GET** request: 
     ```bash
     curl http://localhost:8080
     ```
   - For **POST** request:
     ```bash
     curl -X POST -d "data=hello" http://localhost:8080
     ```

## Example Response

**GET**:
```html
<html><body><h1>GET request received</h1></body></html>
```

**POST**:
```html
<html><body><h1>POST request received</h1><p>Data: hello</p></body></html>
```

## Improvements
- Add more HTTP methods.
- Handle multiple clients (e.g., using threads).

---

This version is concise and still provides the necessary information to use the project!
