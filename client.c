#include <stdio.h>
#include <stdlib.h> 

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

int domain = AF_INET; // IPv4
int type = SOCK_STREAM; // TCP
int protocol = 0; // system chooses based on type, domain.




int main(){

	// creates a client socket.
	int network_socket;
	network_socket = socket(domain, type, protocol);

	// specify an address for the socket.
	struct sockaddr_in server_address;
	server_address.sin_family = domain; // IPv4
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

	// strat tho connection.
	int conection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));

	// check for errors in the connection.
	if (conection_status == -1) {
		printf("There was an error making a connection.\n\n");
	} 
	else {
		printf("Connection established! \n\n");
	}

	//recive the data from the server.
	char server_response[256];
	int bytes_recv = recv(network_socket, &server_response, sizeof(server_response), 0);
	
	if (bytes_recv < 0){
		printf("Recive failed.");}
	else {
	printf("the server sent: %s \n", server_response);}

	printf("%d", bytes_recv);

	// close the socket.
	close(network_socket);

	return 0;
}

