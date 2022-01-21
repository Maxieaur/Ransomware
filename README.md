# Ransomware 2021-2022
L’objectif de ce projet est de développer un ransomware, afin de mieux comprendre, à la fois le fonctionnement de ce type de programme, mais également de pouvoir appliquer les concepts vus au cours en utilisant le langage C pour réaliser le projet.

Les ransomware sont une famille de logiciels malveillants dont l’objectif est, en général, déchiffrer des donn´ees sensibles sur un système afin d’obtenir une rançon en échange de la clé qui permettra de déchiffrer ces données. La maximisation des gains est donc primordiale pour l’attaquant et l’implémentation du ransomware sera impactée par cet objectif.

L’objectif de ce projet est de réaliser un ransomware relativement simple qui sera en mesure de chiffrer le contenu d’un répertoire. Le chemin du répertoire sera passé en paramètre et la clé de déchiffrement sera envoyée via le réseau.

Le ransomware fonctionne sur la distribution Linux. Une fois celui-ci lancé sur la machinede la victime, une clé sera générée et utilisée pour chiffrer le contenu d’un répertoire. Une fois le chiffrement terminé, la clé sera envoyée à l’aide des sockets sur la machine de l’attaquant. La librairie ransomlib.h et ransomlib.c est utilisée, qui se repose sur openssl, implémentant une série d'algorithmes de chiffrement.



* usage: fonction permettant d’afficher l’aide  

* is_encrypted: cette fonction permet de vérifier si un fichier est chiffré ou non. Cela permet par exemple d’éviter de chiffrer plusieurs fois le même fichier et de déchiffrer un fichier qui ne l’est pas. Attention qu’il ne s’agit pas de vérifier le contenu du fichier mais d’utiliser l’extension dans le nom du fichier.  

* list_dir: fonction utilisée de manière récursive afin de lister tous les éléments présents dans un répertoire. Pour chaque fichier trouvé, cette fonction utilisera la librairie ransomlib pour les opérations de chiffrement en passant la clé et le vecteur d’initialisation en paramètres. Le derniér paramètre de flag permet lui d’indiquer si l’on souhaite chiffrer ou déchiffrer le contenu d’un répertoire. Ce flag devra être passé en argument lors de l’exécution du programme.  

* generate_key: cette fonction a pour objectif de générer une clé et le vecteur d’initialisation correspondant. Cette fonction recoit comme paramètre l’adresse des couples clé:iv au format binaire (key, iv) et au format hexadécimal (pKey,pIv). Ces variables sont déclarées dans la fonction principale main et initialisées dans la fonction generate key à l’aide de la librairie OpenSSL pour le binaire et de la librairie ransom.h pour l’hexadécimal. La liste complète des paramètres est définie ci-dessous:  
    * key: pointeur vers la clé au format binaire déclarée dans la fonction main. Cette variable est un tableau de caractères non signés dont la taille est définie par une constante OpenSSL représant la taille d’une clé AES 256 bits.  
    * sizeKey: taille d’une clé AES 256 bits. La fonction de génération aléatoire d’OpenSSL a besoin de cette information pour créer la clé.  
    * iv: pointeur vers le vecteur d’initialisation au format binaire déclarée dans la fonction main. Cette variable est un tableau de caractères non signés dont la taille est définie par une constante OpenSSL représentant la taille d’un bloc CBC AES.  
    * sizeIv: taille d’un bloc CBC AES. La fonction de génération aléatoire d’OpenSSL a besoin de cette information pour créer le vecteur d’initialisation.  
    * pKey: pointeur vers un tableau de caractères déclaré dans la fonction main. Cette variable doit être initialisée en transformant la clé au format binaire vers un format hexadécimal. La taille de ce tableau doit être suffisante pour stocker une version hexadécimale de la clé binaire.  
    * pIv: pointeur vers un tableau de caractères déclaré dans la fonction main. Cette variable doit être initialisée en transformant le vecteur d’initialisation au format binaire vers un format hexadécimal. La taille de ce tableau doit être suffisante pour stocker une version hexadécimale du vecteur binaire.  

* send key: on utilise cette fonction pour envoyer la clé et le vecteur d’initialisation via un socket vers la machine de l’attaquant.  

* main: la fonction principale aura pour objectif de traiter les arguments passés à l’ex´ecution (argv). On retrouvera dans les arguments au minimum: le chemin vers
le répertoire à chiffrer, le mode d’opération (déchiffrer ou chiffrer) ainsi que la clé et le vecteur d’initialisation en cas de déchiffrement.  




--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Vérifier si gcc est installé aisi que la librairie Openssl:  
    sudo apt-get install build-essential  
    sudo apt-get install libssl-dev  

Pour compiler le programme:  
    gcc -o ransom ransom.c ransomlib.c -lcrypto  
    gcc server.c -o server  

Le dossier "important" contient une série de dossiers et fichiers sur lesquel vous pouvez tester votre programme. 

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
