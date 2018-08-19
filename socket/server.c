#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])  
{
	int server_fd;                     // server fd
	int client_fd;                     // client fd
	struct sockaddr_in server_addr;    // server address
	struct sockaddr_in client_addr;    // client address

	int len;
	int sin_size;  
	char buf[BUFSIZ];  // data buffer

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY; // allow all local address
	server_addr.sin_port = htons(1234);       // server port

	// creat socket
	if((server_fd = socket(PF_INET, SOCK_STREAM, 0))<0) {    
		printf("[server] socket creat error\n");
		exit(1);  
	}

	// bind socket to net address
	if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
		printf("[server] bind error\n");
		exit(1);
	}  

	// listen
	listen(server_fd, 8);  

	sin_size=sizeof(struct sockaddr_in);  

	// wait client request
	printf("wait client request\n");
	if((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &sin_size))<0) {
		printf("[server] accept error\n");
		exit(1);
	}

	printf("accept client %s\n",inet_ntoa(client_addr.sin_addr));
	len = send(client_fd, "[server] Welcome\n", 32, 0);

	// recv from clinet and reply
	while(((len = recv(client_fd, buf, BUFSIZ, 0)) > 0)) {
		if(len >= BUFSIZ) {
			len = BUFSIZ-1;
		}

		buf[len]='\0';  
		printf("%s.server\n", buf);  
		if(send(client_fd, buf, len+7, 0)<0) {  
			printf("[server] write error\n");  
			return 1;  
		}  
	}

	// close socket
	close(client_fd);  
	close(server_fd);  
	return 0;  
}
