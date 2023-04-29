#ifdef _WIN32
#define _WIN32_WINNT _WIN32_WINNT_WIN7 // select minimal legacy support, needed for inet_pton, inet_ntop
#include <winsock2.h>				   //for all socket programming
#include <ws2tcpip.h>				   //for getaddrinfo, inet_pton, inet_ntop
#include <stdio.h>					   //for fprintf
#include <unistd.h>					   //for close
#include <stdlib.h>					   //for exit
#include <string.h>					   //for memset
void OSInit(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
}
void OSCleanup(void)
{
	WSACleanup();
}
#else
#include <sys/socket.h> //for sockaddr, socket, socket
#include <sys/types.h>	//for size_t
#include <netdb.h>		//for getaddrinfo
#include <netinet/in.h> //for sockaddr_in
#include <arpa/inet.h>	//for htons, htonl, inet_pton, inet_ntop
#include <errno.h>		//for errno
#include <stdio.h>		//for fprintf, perror
#include <unistd.h>		//for close
#include <stdlib.h>		//for exit
#include <string.h>		//for memset
void OSInit(void) {}
void OSCleanup(void) {}
#endif

int initialization(struct sockaddr **internet_address, socklen_t *internet_address_length);
void execution(int internet_socket, struct sockaddr *internet_address, socklen_t internet_address_length);
void cleanup(int internet_socket, struct sockaddr *internet_address);

int main(int argc, char *argv[])
{
	//////////////////
	// Initialization//
	//////////////////

	OSInit();

	struct sockaddr *internet_address = NULL;
	socklen_t internet_address_length = 0;
	int internet_socket = initialization(&internet_address, &internet_address_length);

	/////////////
	// Execution//
	/////////////

	execution(internet_socket, internet_address, internet_address_length);

	////////////
	// Clean up//
	////////////

	cleanup(internet_socket, internet_address);

	OSCleanup();

	return 0;
}

int initialization(struct sockaddr **internet_address, socklen_t *internet_address_length)
{
	// Step 1.1
	struct addrinfo internet_address_setup;
	struct addrinfo *internet_address_result;
	memset(&internet_address_setup, 0, sizeof internet_address_setup);
	internet_address_setup.ai_family = AF_UNSPEC;
	internet_address_setup.ai_socktype = SOCK_DGRAM;
	getaddrinfo("::1", "24042", &internet_address_setup, &internet_address_result);

	// Step 1.2
	int internet_socket;
	internet_socket = socket(internet_address_result->ai_family, internet_address_result->ai_socktype, internet_address_result->ai_protocol);

	// Step 1.3
	*internet_address_length = internet_address_result->ai_addrlen;
	*internet_address = (struct sockaddr *)malloc(internet_address_result->ai_addrlen);
	memcpy(*internet_address, internet_address_result->ai_addr, internet_address_result->ai_addrlen);

	freeaddrinfo(internet_address_result);

	return internet_socket;
}

void execution(int internet_socket, struct sockaddr *internet_address, socklen_t internet_address_length)
{
	int ReceivedNumber = 0;
	// Step 2.1
	sendto(internet_socket, "GO", 3, 0, internet_address, internet_address_length);

	// Step 2.2
	int number_of_bytes_received = 0;
	char buffer[1000];
	int numbers[42];
	for (int i = 0; i < 3; i++)
	{
		number_of_bytes_received = recvfrom(internet_socket, buffer, (sizeof buffer) - 1, 0, internet_address, &internet_address_length);
		buffer[number_of_bytes_received] = '\0';
		//printf("Received : %s. As decimal: %d\n", buffer, buffer);
		numbers[i] = ntohl(strtol( buffer, NULL, 10)); // strtol( const char *str, char **str_end, int base );
		//printf("IN NTOHL: %d\n", numbers[i]);
	}
}

void cleanup(int internet_socket, struct sockaddr *internet_address)
{
	// Step 3.2
	free(internet_address);

	// Step 3.1
	close(internet_socket);
}