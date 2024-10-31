 #Code #Network 




# **client:** 
## Headers:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <netinet/in,h>
#include <sys.socket.h>
#include <sys.types.h>
```
[[stdio.h]] [[stdlib.h]] [[string.h]] [[unistd.h]] [[arpa-inet.h]] [[netinet-in.h]] [[sys.socket.h]] [[sys-types.h]]


#### **Why Each Header Is Necessary**:

- **Networking Operations:**
    - **Socket Functions (`<sys/socket.h>`, `<unistd.h>`):** To perform networking operations like creating sockets, binding, listening, accepting connections, and sending/receiving data, you need functions declared in these headers.

- **Addressing and Byte Order Conversion:**
    - **Internet Addresses (`<arpa/inet.h>`, `<netinet/in.h>`):** Handling IP addresses and ports requires structures and functions from these headers. They provide necessary tools to set up `sockaddr_in` structures and convert values to the correct byte order for network communication.

- **Standard I/O and Error Handling:**
    - **Input/Output (`<stdio.h>`):** Used for logging server status and errors, which is crucial for monitoring server activity and debugging.
    - **Error Messages (`perror()`):** Provides human-readable error messages when system calls fail, helping you understand and fix issues.

- **String Manipulation and Memory Operations:**
    - **String Functions (`<string.h>`):** Handling HTTP request parsing and response formatting involves string operations like comparison, copying, and length calculation.
    - **Memory Functions (`memset()`):** Initializing buffers to zero to prevent unexpected behavior from uninitialized memory.

- **Program Control and Utilities:**
    - **Standard Library (`<stdlib.h>`):** Includes utility functions like `exit()` for terminating the program on critical failures.

- **File Descriptor Operations:**
    - **File Operations (`<unistd.h>`):** Managing file descriptors, such as sockets, requires functions like `close()`, `read()`, and `write()`.

#### **How They Work Together in Your HTTP Server**

- **Creating the Socket:**
    - **`socket()`** function (from `<sys/socket.h>`) is used to create a socket file descriptor for network communication.
    - **Constants** like `AF_INET` and `SOCK_STREAM` are defined in `<netinet/in.h>` and `<sys/socket.h>`.

- **Binding and Listening:**
    - **`bind()`** function binds the socket to an IP address and port, using structures like `struct sockaddr_in` from `<netinet/in.h>`.
    - **`listen()`** puts the socket into listening mode, ready to accept incoming connections.

- **Accepting Connections:**
    - **`accept()`** function waits for an incoming connection and returns a new socket file descriptor for communication with the client.

- **Data Transmission:**
    - **`recv()`** and **`send()`** functions (from `<sys/socket.h>`) are used to receive data from and send data to the client.

- **Closing Connections:**
    - **`close()`** function (from `<unistd.h>`) closes the socket when communication is done.

- **Handling Data and Responses:**
    - **String Functions:** Parsing the HTTP request and constructing the response involves string manipulation functions from `<string.h>`.
    - **Formatting Output:** Functions like `snprintf()` from `<stdio.h>` are used to format the HTTP response with headers and content.

- **Error Handling and Program Control:**
    - **`perror()`** provides error messages for failed system calls.
    - **`exit()`** terminates the program when a critical error occurs.




## socket():

```c
int socket(int domain, int type, int protocol);
```

##### Domain: 

Specifies the communication domain or the protocol family that will be used for communication.

- Common Values:

    - `AF_INET`:
        - Description: IPv4 Internet protocols.
        - Usage: Most common for standard network communications.
    - `AF_INET6`:
        - Description: IPv6 Internet protocols.
        - Usage: Used when communicating over IPv6 networks.
    - `AF_UNIX` or `AF_LOCAL`:
        - Description: Local communication using Unix domain sockets.
        - Usage: Inter-process communication (IPC) on the same host.
    - `AF_BLUETOOTH`:
        - Description: Bluetooth communication.
        - Usage: For Bluetooth-based applications.


#### Type:

Specifies the communication semantics or the socket type.

- Common Values:

    - `SOCK_STREAM`:
        - Description: Provides sequenced, reliable, two-way, connection-based byte streams.
        - Underlying Protocol: Typically uses TCP.
        - Usage: Suitable for applications where reliability is crucial (e.g., HTTP, FTP).

    - `SOCK_DGRAM`:
        - Description: Supports datagrams (connectionless, unreliable messages of a fixed maximum length).
        - Underlying Protocol: Typically uses UDP.
        - Usage: Suitable for applications where speed is prioritized over reliability (e.g., DNS queries, streaming).

    - `SOCK_RAW`:
        - Description: Provides access to raw network protocols.
        - Usage: Used for custom protocol development or network diagnostics.

    - `SOCK_SEQPACKET`:
        - Description: Provides sequenced, reliable, two-way connection-based data transmission paths for datagrams of fixed maximum length.
        - Usage: Less common; used in specific applications requiring ordered data transmission.

#### Protocol:

Specifies a particular protocol to be used with the socket. Often set to `0` to select the default protocol for the given domain and type.

- Common Values:

    - `0`:
        - Description: Automatically selects the appropriate protocol based on the `domain` and `type`.

    - Specific Protocols:

        - `IPPROTO_TCP`:
            - Description: TCP protocol.
            - Usage: When explicitly specifying TCP.
        - `IPPROTO_UDP`:

            - Description: UDP protocol.
            - Usage: When explicitly specifying UDP.

        - `IPPROTO_ICMP`:
            - Description: ICMP protocol.
            - Usage: For sending ICMP messages (e.g., ping).

## Host address:

Creating the server_address which indicates the end-point of the server that we want to connect to.

```c
struct sockaddr_in server_address;
```

#### **Member Breakdown**

```c
#include <netinet/in.h> 

struct sockaddr_in {
short sin_family; // Address family (e.g., AF_INET) 
unsigned short sin_port; // Port number (in network byte order) 
struct in_addr sin_addr; // Internet address (struct in_addr) 
char sin_zero[8]; // Padding to make the structure the same size as struct sockaddr };
```

1. **`sin_family`**
    - **Type:** `short`
    - **Description:** Specifies the address family. For IPv4 addresses, this is set to `AF_INET`.

3. **`sin_port`**
    - **Type:** `unsigned short`
    - **Description:** Represents the port number on which the server will listen or the client will connect. It **must** be in **network byte order** (big-endian).

    - **Conversion Functions:**
        - `htons()`: Host to Network Short
        - `ntohs()`: Network to Host Short

3. **`sin_addr`**
    - **Type:** `struct in_addr`
    - **Description:** Contains the IPv4 address. This can be set to a specific IP or to special constants like `INADDR_ANY`.

    - **Setting IP Address:**
        - **`INADDR_ANY`:** Binds the socket to all available interfaces.
        - **Using `inet_pton()`:** Converts an IP address from text to binary form.

4. **`sin_zero`**
    
    - **Type:** `char[8]`
    - **Description:** Padding to ensure that the `sockaddr_in` structure is the same size as `struct sockaddr`. It is typically set to zero.
    - **Example:**

#### **Building the Address:** 

```c
server_address.sin_family = AF_INET;
```

Should be the same as the created socket and the server we want to connect to.


```c 
server_address.sin_port = htons(9002);
```

Specify the port to which we are connecting and using [[htons()]] to convert the int to
big-endian.

```c
server_address.sin_addr.s_addr = INADDR_ANY
```

This line tells the server to **accept connections on any of its available network interfaces** (e.g., Ethernet, Wi-Fi).

``server_address`
	This is a variable of type `struct sockaddr_in` that holds the server's address information.

**`.sin_addr`:**
	This member of the structure represents the **IP address** for the socket.

**`.s_addr`:**
	This is the actual **32-bit IPv4 address** in the `sin_addr` structure.

 **`INADDR_ANY`:**
	A constant that tells the server to bind to **all available IP addresses** on the machine.
    **Equivalent to:** `0.0.0.0` in IPv4 notation.

## Connect(): 

```c 
connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
```
The `connect()` function establishes a connection to a server. It requires three parameters:

```c
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

### **1. `sockfd`**

 **`sockfd`** is the **socket file descriptor**. It's an integer that uniquely identifies the socket within your program.
 
Represents the **endpoint** for communication.
Must be **valid** and **properly initialized** before calling `connect()`.
If `socket()` fails, `sockfd` will be `-1`, and you should handle the error accordingly.

### **2. `addr`**

**Type:** `const struct sockaddr *`

`addr` is a **pointer** to a `struct sockaddr` that contains the **address information** of the server you want to connect to.

### **3. `addrlen`**

**Type:** `socklen_t`

**`addrlen`** specifies the **size** (in bytes) of the address structure pointed to by `addr`.
It's used by `connect()` to know how much memory to read for the address.

- **Key Points:**
    - Must accurately represent the **size** of the address structure.
    - For `struct sockaddr_in`, it would typically be `sizeof(struct sockaddr_in)`.
    - Ensures that `connect()` reads the correct amount of data from the address structure.

```c
int connection_status = connect()
```

`connect()` returns an int resembling the state of the connection.

if connect() returns 0, connection is successful.
if connect() returns -1. connection failed.

## recv():

```C 
char server_respone[256];
recv(network_socket, &server_response, sizeof(server_response), 0);
```

The **`recv()`** function is used in **socket programming** to **receive data** from a **connected socket** (like a client receiving data from a server or vice versa).


```c
#include <sys/types.h>
#include <sys/socket.h>

recv(int sockfd, void *buffer, size_t len, int flags);
```
### **Parameters Explained**

1. **`sockfd`**
    - **Type:** `int`
    - **Description:** The **socket file descriptor** you want to receive data from. This socket must be **already connected** (e.g., after `connect()` or `accept()`).

1. **`buffer`**
    - **Type:** `void *`
    - **Description:** A **pointer to a memory area** where the received data will be stored.

2. **`len`**
    - **Type:** `size_t`
    - **Description:** The **maximum number of bytes** to receive and store in the buffer.

3. **`flags`**
    - **Type:** `int`
    - **Description:** Options to modify the behavior of `recv()`. Commonly set to `0` for default behavior.

### **Return Value**
- **On Success:** Returns the **number of bytes received** and stored in the buffer.
- **On Failure:** Returns **`-1`** and sets `errno` to indicate the error.

**Common Flags:**
- Use `0` for standard data reception.
- Use `MSG_PEEK` to inspect data without consuming it.
- Use `MSG_WAITALL` when you need to ensure complete data reception.
- Use `MSG_DONTWAIT` for non-blocking operations.

## Close():

```c
close(network_socket);
```

the **`close()`** function is used to **terminate a connection** by **closing a file descriptor**. This file descriptor could represent various resources, such as files, sockets, or pipes. When you’re done working with a file or a socket, calling `close()` releases the resource, freeing up system resources and ensuring no further data can be read or written through that descriptor.

### **Return Value**

- **On Success:** Returns `0`.
- **On Failure:** Returns `-1` and sets `errno` to indicate the error.



# **Host:**
## Bind():

the **`bind()`** function is used to **associate a socket with a specific IP address and port number** on the server side. This step is necessary for servers to "announce" where they will listen for incoming connections, so clients know where to connect.

```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

### **Parameters**:

1. **`sockfd`**: The socket file descriptor created with `socket()`.

2. **`addr`**: A pointer to a `struct sockaddr` (typically cast from `struct sockaddr_in` for IPv4) that specifies the IP address and port number for the socket.

3. **`addrlen`**: The size of the `addr` structure.

### **Return Value**

- **On Success**: Returns `0`.

- **On Failure**: Returns `-1` and sets `errno` to indicate the error.

## Listen():

the **`listen()`** function is used on the server side to **mark a socket as passive**, meaning it will **wait for incoming connection requests** rather than initiate a connection. After `bind()`, `listen()` prepares the server to accept connections.

```c
int listen(int sockfd, int backlog);
```

### **Parameters**

1. **`sockfd`**: The socket file descriptor created with `socket()` and bound with `bind()`.

2. **`backlog`**: The maximum number of **pending connections** allowed in the queue. This is how many clients can wait while the server processes other connections.

### **Return Value**

- **On Success:** Returns `0`.

- **On Failure:** Returns `-1` and sets `errno` to indicate the error.

## Accept:

the **`accept()`** function is used by a server to **accept an incoming connection request** from a client. Once a client connects, `accept()` creates a **new socket** specifically for communication with that client, allowing the server to handle multiple clients simultaneously.

### **Parameters**

1. **`sockfd`**: The listening socket file descriptor created with `socket()` and marked to listen with `listen()`.

2. **`addr`**: A pointer to a `struct sockaddr` where the client’s address will be stored. This structure provides information about the client’s IP and port.

3. **`addrlen`**: A pointer to a variable that specifies the size of `addr`. It’s updated with the size of the client's address once `accept()` fills in `addr`.

### **Return Value**

- **On Success:** Returns a **new socket file descriptor** for the connected client. This socket is used for sending and receiving data with the client.

- **On Failure:** Returns `-1` and sets `errno` to indicate the error.

## send():

The **`send()`** function is used to **transmit data** over a connected socket. Typically, this function is used by a server or client to send messages to the other end of the connection.

```c
ssize_t send(int sockfd, const void *buffer, size_t len, int flags);
```

### **Parameters**

1. **`sockfd`**: The socket file descriptor for the connected socket (obtained from `socket()` on the client or from `accept()` on the server).

2. **`buffer`**: A pointer to the data you want to send. This is typically a character array or a string.

3. **`len`**: The number of bytes you want to send from `buffer`.

4. **`flags`**: Options to control the sending behavior. Setting `0` means no special behavior; other flags are rarely used in basic socket programming.

### **Return Value**

- **On Success:** Returns the number of bytes actually sent (this may be less than `len` if only part of the data was sent).

- **On Failure:** Returns `-1` and sets `errno` to indicate the error.