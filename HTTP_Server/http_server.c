#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

int server_socket;
char* full_response = NULL;
char* response_data = NULL;

// Signal handler to gracefully shutdown the server
void handle_sigint(int sig) {
    printf("\nShutting down server...\n");
    if (server_socket != -1) {
        //
        close(server_socket);
    }
    if (full_response) {
        free(full_response);
    }
    if (response_data) {
        free(response_data);
    }
    printf("Memory cleared, Goodbye!\n");
    exit(0);
}

// Function to read the contents of an HTML file
char* read_html_file(const char* filename) {
    // Open the HTML file to be served
    FILE *html_data;
    html_data = fopen(filename, "r");
    if (html_data == NULL) {
        printf("Error opening file. \n");
        return NULL;
    }

    // Read the file character by character
    char* response_data = NULL;
    size_t total_size = 0;
    char c;

    while ((c = fgetc(html_data)) != EOF) {
        response_data = realloc(response_data, total_size + 2);
        if (!response_data) {
            perror("Memory allocation failed");
            fclose(html_data);
            return NULL;
        }
        response_data[total_size] = c;
        total_size++;
    }
    // Null-terminate the string
    if (response_data) {
        response_data[total_size] = '\0';
    }

    // Close the file and return the contents as a string
    fclose(html_data);
    return response_data;
}

// Function to update the connection log file
void update_log(const char* ip_address, int port, int socket_fd) {
    // Get current time and date
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    char time_str[100];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local);    
    
    // Append log to file "logs.txt"
    FILE *log_file = fopen("logs.txt", "a");
    if (log_file == NULL) {
        perror("Error opening log file");
        return;
    }
    fprintf(log_file, "[%s] Client connected: IP %s, Port %d, Socket FD %d\n", time_str, ip_address, port, socket_fd);
    fclose(log_file);            
}


// Function to accept a client connection
int accept_client_connection(int server_socket){

    struct ClientInfo {
        char ip_address[INET_ADDRSTRLEN];
        int port;
        int socket_fd;
    };

    struct ClientInfo current_client;

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Accept the incoming client connection
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket < 0) {
        perror("Accept failed");
        return -1;
    }

    // Retrieve client IP address and port information
    printf("accepted\n");
    inet_ntop(AF_INET, &client_addr.sin_addr, current_client.ip_address, INET_ADDRSTRLEN);
    current_client.port = ntohs(client_addr.sin_port);
    current_client.socket_fd = client_socket;
    printf("Client connected: IP %s, Port %d, Socket FD %d\n", current_client.ip_address, current_client.port, current_client.socket_fd);


    // Update connection log
    update_log(current_client.ip_address, current_client.port, client_socket);
    printf("inside accept\n");
    return client_socket;
}


int main() {
    // Register signal handler for graceful shutdown
    signal(SIGINT, handle_sigint);

    // Read the HTML file to serve
    const char* filename = "index.html";
    response_data = read_html_file(filename);
    if (response_data == NULL) {
        return 1; // Exit the program if file reading fails
    }

    size_t size = strlen(response_data);
    printf("Size : %zu\n", size);

    // Create HTTP response header
    char http_header[1024];
    snprintf(http_header, sizeof(http_header), "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n", size);
    
    size_t full_response_size = 1024 + size;
    full_response = malloc(full_response_size + 1);

    if (!full_response) {
        perror("Memory allocation for response failed");
        free(response_data);
        return 1;
    }

    // Combine header and HTML data into full response
    strcpy(full_response, http_header);
    strcat(full_response, response_data);

    // Create a server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        free(full_response);
        free(response_data);
        return 1;
    }

    // Define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; // IPv4
    server_address.sin_port = htons(8001); // Port 8001
    server_address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP

    // Bind the socket to the specified address and port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));


    // Start listening for incoming connections
    if (listen(server_socket, 20) < 0) {
        perror("Listening failed");
        close(server_socket);
        free(full_response);
        free(response_data);
        return 1;
    }

    // Server loop to handle incoming client connections
    while (1) {
        int client_socket = accept_client_connection(server_socket);
        printf("test1\n");
        if (client_socket >= 0) {
            printf("test2\n");
            // Send the response to the client
            ssize_t bytes_sent = send(client_socket, full_response, full_response_size, 0);
            printf("byte sent: %d \n", bytes_sent);
            close(client_socket);
        }

        printf("Closed connection.\n\n");
        sleep(1);
    }

    return 0;
}
