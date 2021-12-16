# Ransomware---2021-2022
L’objectif de ce projet est de développer un ransomware, afin de mieux comprendre, à la fois le fonctionnement de ce type de programme, mais également de pouvoir appliquer les concepts vus au cours en utilisant le langage C pour réaliser le projet.

Les ransomware sont une famille de logiciels malveillants dont l’objectif est, en général, déchiffrer des donn´ees sensibles sur un système afin d’obtenir une rançon en échange de la clé qui permettra de déchiffrer ces données. La maximisation des gains est donc primordiale pour l’attaquant et l’implémentation du ransomware sera impactée par cet objectif.

L’objectif de ce projet est de réaliser un ransomware relativement simple qui sera en mesure de chiffrer le contenu d’un répertoire. Le chemin du répertoire sera passé en paramètre et la clé de déchiffrement sera envoyée via le réseau.

Le ransomware fonctionne sur la distribution Linux. Une fois celui-ci lancé sur la machinede la victime, une clé sera générée et utilisée pour chiffrer le contenu d’un répertoire. Une fois le chiffrement terminé, la clé sera envoyée à l’aide des sockets sur la machine de l’attaquant. La librairie ransomlib.h et ransomlib.c est utilisée, qui se repose sur openssl, implémentant une série d'algorithmes de chiffrement.
