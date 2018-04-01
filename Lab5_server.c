//Kuanping Chang 71018021

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

struct data{
	unsigned char web_addr[256];
	unsigned char webName[256];
};

int main(int argc, char *argv[]){
	
	if(argc < 2){
		fprintf(stderr, "ERROR, no port entered\n");
		exit(1);
	}
	//address for server and client
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	//create socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("ERROR opening socket location");
	}
	bzero((char *) &server_address, sizeof(server_address));
	
	int n_sockfd;
	int portno = atoi(argv[1]);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(portno);
	if(bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
		perror("ERROR BINDING");
	}
	
	int client_length;
	int isActive = 1;
	int address_length;
	int i;
	int f_addr = 0;
	
	//buffers and store location
	unsigned char buffer[256];
	unsigned char webAddr[256];
	unsigned char sentMessage[256];
	unsigned char addrName[256];
	
	//create the database
	struct data database[3];
	strcpy(database[0].web_addr, "harris@ics.uci.edu");
	strcpy(database[0].webName, "Ian G. Harris");
	strcpy(database[1].web_addr, "joe@cnn.com");
	strcpy(database[1].webName, "Joe Smith");
	strcpy(database[2].web_addr, "jane@slashdot.org");
	strcpy(database[2].webName, "Jane Smith");
	//for read or write info
	int rw;
	
	printf("Address Server Started\n");
	while(1){ //only quit by ctrl-c
		isActive = 1;
		listen(sockfd, 5);
		client_length = sizeof(client_address);
		n_sockfd = accept(sockfd, (struct sockaddr *) &client_address, &client_length);
		bzero(buffer, 256);
		bzero(webAddr, 256);
		bzero(sentMessage, 256);
		if(n_sockfd < 0){
			perror("Error on accepting new client");
		}
		while(isActive == 1){
			bzero(buffer, 256);
			bzero(webAddr, 256);
			bzero(sentMessage, 256);
			rw = read(n_sockfd, buffer, 255);
			if(rw == 0){ //no read 
				isActive = 0;
			}
			else if(rw < 0){//cant be negative
				perror("Error reading from socket");
			}
			else{
				f_addr = 0;
				address_length = (int) buffer[0];
				i = 0;
				while(i < address_length){
					webAddr[i] = buffer[(i+1)];
					i++;
				}
				i = 0;
				printf("\n%s\n", webAddr);
				while((i < 3) && (f_addr == 0)){
					if(strcmp(webAddr, database[i].web_addr) == 0){
						address_length = strlen(database[i].webName);
						strcpy(addrName, database[i].webName);
						f_addr = 1;
					}
					else{
						i++;
					}
				}
				if(f_addr == 0){
					strcpy(addrName, "unknown");
					address_length = 7;
				}
				sentMessage[0] = address_length;
				i = 0;
				while(i < address_length){
					sentMessage[(i+1)] = addrName[i];
					i++;
				}
				rw = write(n_sockfd, sentMessage, strlen(sentMessage));
				if(rw < 0){
					perror("Error writing to socket");
				}
			}
		}
		close(n_sockfd);
		sleep(1);
	}
	return 0;
}
