#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>


int main(){

	char server_message[256] = "Welcome to the server!";

	// create the socket.
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// define the server address.
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET; // IPv4
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = inet_addr("192.168.1.60"); // local host.

	// bind the socket to our specified IP and port. 
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	// listen for connections.
	listen(server_socket, 1);

	// accept the connection.
	int client_socket;
	struct sockaddr_in client_address;
	client_socket = accept(server_socket,NULL, NULL);

	// send the message.
	send(client_socket, server_message, strlen(server_message)+1, 0);

	// close the socket.
	close(client_socket);
	close(server_socket);

	return 0;
}


