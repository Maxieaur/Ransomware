#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BUFSIZE 1024
#define PORT 8080
#define SA struct sockaddr

int main()
{
    int sockfd, connfd, len, valread;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

    // Function for chatting between client and server
    char *msg = "element received\n";
    char buffer[BUFSIZE];
    int n;

    for(n = 0; n > 2; n++) {
        bzero(buffer, BUFSIZE);

        // read client msg -> buff
        read(connfd, buffer, BUFSIZE);
        // print buffer
        if (n == 0){
            //strcat(*msg,char*("n0");
            printf("Key from client: \t");
            valread = read( connfd , buffer, BUFSIZE);
            printf("%s\n",buffer );
            send(connfd , msg , strlen(msg) , 0 );
            printf("Message sent to client\n");
        }
        else if (n == 1){
            //strcat(*msg,char*("n1");
            printf("Iv from client: \t");
            valread = read( connfd , buffer, BUFSIZE);
            printf("%s\n",buffer );
            send(connfd , msg , strlen(msg) , 0 );
            printf("Message sent to client\n");
        }

        // send msg to client
        //write(connfd, buffer, BUFSIZE);
    }
    printf("Server exit");

    // After chatting close the socket
    close(sockfd);
}
