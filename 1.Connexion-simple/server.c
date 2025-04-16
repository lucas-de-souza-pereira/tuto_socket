#include <stdio.h>              // printf(), perror() → affichage et messages d'erreur
#include <stdlib.h>             // exit(), malloc() → fonctions utilitaires de base
#include <string.h>             // memset(), strlen(), strcpy() → gestion de chaînes de caractères
#include <unistd.h>             // close(), read(), write() → appels système Unix (fermeture, lecture, écriture)
#include <sys/socket.h>         // socket(), connect(), send(), recv() → fonctions de base pour les sockets
#include <netinet/in.h>         // struct sockaddr_in, htons() → structures et fonctions pour les adresses IPv4
#include <arpa/inet.h>          // inet_pton(), inet_ntoa() → conversion des adresses IP entre texte et binaire


// Côté serveur :
// 1. Création du socket : socket()
// 2. Association à une adresse IP et un port : bind()
// 3. Écoute des connexions entrantes : listen()
// 4. Acceptation des connexions : accept()
// 5. Attendre et Accepter une connexion entrante :
// 6. Communication : read() / write() ou recv() / send()
// 7. Fermeture : close()


int main(){
    int server_fd; // Descripteur de fichier du socket serveur (int utilisé par le système pour le représenter)

// 1. créer le socket

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    //int socket(int domain, int type, int protocol);
    // domain | Famille d’adresses (ex. AF_INET pour IPv4)
    // type | Type de socket (ex. SOCK_STREAM pour TCP, SOCK_DGRAM pour UDP)
    // protocol | Protocole spécifique (en général 0)


//2. définir l'adresse du serveur 

    struct sockaddr_in server_addr;
    //structure qui contient l'addresse du serveur, type addresse, IP, Port...

    server_addr.sin_family = AF_INET;
    // indique la famille d'adresses utilisée (AF_INET = IPv4), doit être la même que dans socket()

    server_addr.sin_addr.s_addr = INADDR_ANY;
    // type d'adresse utilisé : ANY --> le serveur écoutera sur toutes les interfaces 
    //réseau disponibles (127.0.0.1, 192.168.x.x, etc.).
    // Cela permet au serveur d’écouter peu importe l'interface réseau (utile sur une vraie machine avec plusieurs IP)


    server_addr.sin_port = htons(8080);
    //choisir le port 
    //htons = Host TO Network Short : 
        //Tu donnes un nombre en format local (host) (celui utilisé par ton ordi)
        //Elle le transforme en format réseau (network)
    //Les données sur le réseau doivent respecter un format appelé endianness (big-endian).
    // htons convertit un entier (16 bits) pour qu’il soit compatible avec ce format.

    //Fonction | Signification | Utilisation
        // htons() | host to network short | pour les ports (2 octets)
        // htonl() | host to network long | pour les adresses IP (4 octets)
        // ntohs() | network to host short | pour recevoir un port
        // ntohl() | network to host long | pour recevoir une IP


//3. Lier le socket à l'adresse 

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    // bind : fonction qui attache le socket à une adresse IP et à un port.
    // Un socket tout seul ne « vit nulle part ». bind() lui dit :
    //     "Tu écouteras sur cette IP + ce port".
    
    // server_fd = le socket serveur créé plus tôt
    
                    // attention : partie technique //
                    // Pourquoi ce cast ? (struct sockaddr*)&server_addr
    
    // bind est défini comme : 
    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    
    // → Il attend une structure générique sockaddr*,
    // mais nous on a une sockaddr_in (spécifique à IPv4).
    // → On utilise (struct sockaddr*) pour transformer le pointeur,
    //    et le rendre compatible avec ce que bind attend.
    // &server_addr = l’adresse en mémoire de notre structure (définie au bloc 2)

    // Size of : On indique la taille de la structure passée, ici celle de server_addr.
    

// 4. Mettre le socket en écoute

    listen(server_fd, 5);
    // Met le socket en mode écoute (attente de connexions entrantes).
    // 5 = nombre max de connexions en file d’attente (backlog).

    // attention :  Ce nombre ne limite pas le nombre total de clients, juste combien peuvent attendre en même temps.

// 5. Attendre et Accepter une connexion entrentre :

    struct sockaddr_in client_addr;

    // déclare une structure pour stocker l'addresse du client 

    socklen_t client_len = sizeof(client_addr);
    // préparer une variable contenant la taille de la stucture client_addr
    
    // accept() bloque jusqu'à ce qu’un client tente de se connecter
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

    // client_fd = nouveau socket pour discuter avec CE client
    // server_fd continue à écouter d'autres connexions

    // (struct sockaddr*)&client_addr, client_len
    // comme pour le blind on transforme le type de variable pour qu'il compatible avec l'IVP4 
    //“J’accepte une nouvelle connexion, et je veux que tu écrives les infos du client dans 
    // ma variable client_addr.
    // Je te précise sa taille (client_len) et je te donne son adresse mémoire, castée au bon format.”



    // voici un exemple nous allons pour printer l'adresse ip du client 

    // Affichage de l'adresse IP et du port du client
    char *ip_client = inet_ntoa(client_addr.sin_addr); // IP en texte
    int port_client = ntohs(client_addr.sin_port); // Port lisible

    printf("Client connecté depuis %s:%d\n", ip_client, port_client);

// 6.  Bloc 6 — Lire un message du client et lui répondre

    char buffer[1024] = {0};
    // Créer un espace mémoire en tampon pour recevoir les données envoyé par le client.
    // On crée un tableau de 1024 caractères (vide) pour stocker la réponse.
    // Le {0} initialise tout à zéro (important pour bien terminer la chaîne avec \0).
    

    read(client_fd, buffer, 1024);
    // On lit les données venant du client.
    // Le socket client_fd est celui que accept() a retourné.
    // Attention, cette fonction est bloquante : elle attend qu’un message arrive.

    printf("Message reçu du client : %s\n", buffer);
    // On affiche le message reçu dans le terminal du serveur.

    // si erreur read retourne 0, code pour tester l'erreur :
    // if (read(client_fd, buffer, 1024) < 0) {
    //     perror("Erreur lors de la lecture");
    // }
    

    char *reponse = "Hello client ! Bienvenue !";
    send(client_fd,reponse, strlen(reponse),0);

    // attention send peut être aussi bloquant, 
    // Si le buffer d’envoi du socket est plein (ex : le client ne lit pas), 
    // alors send() attend que de l’espace soit libéré pour envoyer les données

    // Mais :
    // Dans les échanges simples (petit message + client qui lit), ça se passe rapidement.
    // Pour rendre send non bloquant, il faut configurer le socket avec fcntl()
    // ou utiliser select() ou des threads (si on veux gérer plusieurs clients)

    // le 0 à la fin est un flag : 
        // 0	Aucun comportement spécial (par défaut)
        // MSG_DONTWAIT	Envoi non bloquant
        // MSG_OOB	Données urgentes (très rare)
        // MSG_NOSIGNAL	Ne pas envoyer SIGPIPE si le client a fermé la connexion


// 7. Fermer les sockets

    close(client_fd);   // Ferme la communication avec ce client
    close(server_fd);   // Ferme le socket d'écoute


    return 0;
}