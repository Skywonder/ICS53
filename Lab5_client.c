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

int main (int argc, char *argv[]){
	int sockfd;//file descriptor
	int portno; //portno stores the port number on which the server accepts connection
	int address_length = 0;
	int isEqual = 0;
	int isActive = 1;
	int i, rw;//rw is the return value for read() and write()
	
	unsigned char buffer[256];
	unsigned char exit_string[5] = "+++\n";
	unsigned char webName[256];
	unsigned char sentMessage[256];
	struct hostent *server;
	struct sockaddr_in server_address;
	

	if(argc < 3 && argc != 0){
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	
	//set portnumber
	portno = atoi(argv[2]); //port number
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		error("Error opening socket");
	}
	
	//grab host
	server = gethostbyname(argv[1]); //host 
	//making sure server gets valid host
	if(server == NULL){
		fprintf(stderr, "Error invalid hostname\n");
		exit(0);
	}

	bzero((char *) &server_address, sizeof(server_address)); //set all values in a buffer 0 : pointer of buffer, size of buffer - initialization step
	server_address.sin_family = AF_INET; //address family
	bcopy((char *) server ->h_addr, (char *)&server_address.sin_addr.s_addr, server -> h_length);
	server_address.sin_port = htons(portno);//convert to network byte order htons()
	
	if(connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
		perror("ERROR connecting");
	}
	while(isActive == 1){ //reset buffer before proceeding
		printf("> ");
		bzero(buffer, 256);
		bzero(sentMessage, 256);
		bzero(webName, 256);
		fgets(buffer, 255, stdin);
		if(strcmp(buffer, "\n") != 0){
			isEqual = strcmp(buffer, exit_string);//if the str is the same as exit command
			if(isEqual == 0){
				isActive = 0;
			}
			else{
				address_length = (strlen(buffer)) - 1;
				sentMessage[0] = address_length;
				i = 0;
				while(i < address_length){
					sentMessage[(i+1)] = buffer[i];
					i++;
				}
				rw = write(sockfd, sentMessage, strlen(sentMessage));
				if(rw < 0){
					perror("Error write to socket failed");
				}
				bzero(buffer, 256);
				rw = read(sockfd, buffer, 255);
				i = 0;
				address_length = (int)buffer[0];
				while(i < address_length){
					webName[i] = buffer[(i+1)];
					i++;
				}
				if(rw < 0){
					perror("Error reading socket");
				}
					//print out result
				printf("\n%s\n\n", webName);
					
			}
				
		}
			
	}
		
	return 0;
}
