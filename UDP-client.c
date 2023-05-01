#ifdef _WIN32
#define _WIN32_WINNT _WIN32_WINNT_WIN7 // select minimal legacy support, needed for inet_pton, inet_ntop
#include <winsock2.h>				   //for all socket programming
#include <ws2tcpip.h>				   //for getaddrinfo, inet_pton, inet_ntop
#include <stdio.h>					   //for fprintf
#include <unistd.h>					   //for close
#include <stdlib.h>					   //for exit
#include <string.h>					   //for memset
#include <stdint.h>					   //for uint16_t
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
int biggestNumber(int *numbers, int size);

int main(int argc, char *argv[])
{
	OSInit();

	struct sockaddr *internet_address = NULL;
	socklen_t internet_address_length = 0;
	int internet_socket = initialization(&internet_address, &internet_address_length);

	execution(internet_socket, internet_address, internet_address_length);

	cleanup(internet_socket, internet_address);

	OSCleanup();

	return 0;
}

int initialization(struct sockaddr **internet_address, socklen_t *internet_address_length)
{
	struct addrinfo internet_address_setup;
	struct addrinfo *internet_address_result;
	memset(&internet_address_setup, 0, sizeof internet_address_setup);
	internet_address_setup.ai_family = AF_UNSPEC;
	internet_address_setup.ai_socktype = SOCK_DGRAM;
	getaddrinfo("::1", "24042", &internet_address_setup, &internet_address_result);

	int internet_socket;
	internet_socket = socket(internet_address_result->ai_family, internet_address_result->ai_socktype, internet_address_result->ai_protocol);

	*internet_address_length = internet_address_result->ai_addrlen;
	*internet_address = (struct sockaddr *)malloc(internet_address_result->ai_addrlen);
	memcpy(*internet_address, internet_address_result->ai_addr, internet_address_result->ai_addrlen);

	freeaddrinfo(internet_address_result);

	return internet_socket;
}

void execution(int internet_socket, struct sockaddr *internet_address, socklen_t internet_address_length)
{
    int received_numbers[3];
    char goMessage[] = "GO";
    ssize_t number_of_bytes_received = 0;

    //Send "GO" message
    sendto(internet_socket, goMessage, sizeof(goMessage), 0, internet_address, internet_address_length);
       
	   
	int received_number = 0;
	char buffer[1000];
	int numbers[3];
    //Receive 3 integers
    for (int i = 0; i < 3; i++)
    {

        number_of_bytes_received = recvfrom(internet_socket,(char*) &buffer, sizeof(received_number), 0, internet_address, &internet_address_length);
        if (number_of_bytes_received == -1)
        {
            perror("Error receiving data");
            exit(1);
        }
       buffer[number_of_bytes_received] = '\0';
    	received_number = ntohs(*(uint16_t *)buffer); //convert from network byte order
        numbers[i] = received_number;
        printf("Received number[%d]: %d\n",i,received_number);
    }

	
	int biggest_num = biggestNumber(numbers, 3);
	printf("The biggest number is: %d\n", biggest_num);
	

}


int biggestNumber(int *numbers, int size) { //bubble sort
    for (int i = 0; i < size-1; i++) {
        for (int j = 0; j < size-i-1; j++) {
            if (numbers[j] > numbers[j+1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j+1];
                numbers[j+1] = temp;
            }
        }
    }
    return numbers[size-1];
}


void cleanup(int internet_socket, struct sockaddr *internet_address)
{
	free(internet_address);

	close(internet_socket);
}