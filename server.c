#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h> // for close()
#include <arpa/inet.h> // for init_addr
#include <stdlib.h>

#define BUFSIZE 1024
#define SERVER_IP "127.0.0.1"

void handle_error()
{
    printf("Error\n");
    exit(0);
}

int main(void)
{
    int sockid;
    int server_port = 8888;
    char *server_ip = SERVER_IP;

    sockid = socket(AP_INET,SOCK_STREAM,0);

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    char *buffer[BUFSIZE];
    int n, len, client_socket;

    int bind_result = bind(sockid,(const struct sockaddr *)&server_addr,sizeof (server_addr));

    if (bind_result<0){
        printf("Error during binding\n");
    }
    else {
        printf("Server listening on %s:%d\n",server_ip,server_port);
        n = liskten(sockid,1);

        if(n<0){
            printf("Error during listen()\n");
            handle_error();
        }

        len = sizeof(client_addr);
        client_socket = accept(sockid,(struct sockaddr *)&client_addr,&len);

        if(client_socket<0){
            printf("Error during accept()\n");
            handle_error()
        }

        printf("Accepted connection from %d %s:%d\n",client_socket,inet_ntoa(client_addr.sin_addr),client_addr.sin_port);

        n = recv(client_socket, (char *)buffer, BUFSIZE, MSG_WAITALL);

        printf("Message of size %d received: %s\n",n,*buffer);
    }

    close(sockid);
}