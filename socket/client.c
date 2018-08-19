#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{  
	int client_sockfd;
	struct sockaddr_in server_addr;                     // server address
	int len;  
	char buf[BUFSIZ];                                   // buffer

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // server ip
	server_addr.sin_port = htons(1234);                   // server port

	// creat socket
	if((client_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("[client] socket creat error\n");
		exit(1);
	}

	// connect
	if(connect(client_sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
		printf("[clinet] connect error\n");
		exit(1);
	}

	printf("connected to server\n");
	len=recv(client_sockfd, buf, BUFSIZ, 0); // receive data from server
	buf[len]='\0';
	printf("%s",buf);

	// loop
	while(1) {
		printf("[client] >> ");
		scanf("%s",buf);

		len=send(client_sockfd, buf, strlen(buf), 0);
		len=recv(client_sockfd, buf, BUFSIZ, 0);
		buf[len]='\0';
		if(!strcmp("q", buf)) {
			exit(0);
		}
		printf("[client] %s\n",buf);
	}

	// close socket
	close(client_sockfd);
	return 0;  
}  
