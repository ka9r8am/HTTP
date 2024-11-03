# Simple HTTP Server in C

A minimal HTTP server in C that handles **GET** requests.

## Features
- Supports HTTP GET method.
- Simple HTML responses.

## Prerequisites
- **GCC** or any C compiler.

## How to Build and Run

1. **Clone the repo:**
   ```bash
   git clone https://github.com/ka9r8am/HTTP
   cd HTTP/HTTP_Server
   ```

2. **Compile:**
   ```bash
   make http_server
   ```
3. **Permissions:**
4. ```bash
   chmod -x http_server
   ````
5. **Run:**
   ```bash
   ./http_server
   ```
   - For **GET** request: 
     ```bash
     curl http://localhost:8080
     ```
## Example Response

**GET**:
```html
<html><body><h1>GET request received</h1></body></html>
```

## Improvements
- Add more HTTP methods.
- Handle multiple clients (e.g., using threads).

---

This version is concise and still provides the necessary information to use the project!
