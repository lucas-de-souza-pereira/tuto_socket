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
    int server_fd;

// 1. créer le socket

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

//2. définir l'adresse du serveur 

    struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(8080);

//3. Lier le socket à l'adresse 

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

// 4. Mettre le socket en écoute

    listen(server_fd, 5);

// 5. Attendre et Accepter une connexion entrentre :

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
 

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);


    // Affichage de l'adresse IP et du port du client
    char *ip_client = inet_ntoa(client_addr.sin_addr); // IP en texte
    int port_client = ntohs(client_addr.sin_port); // Port lisible

    printf("Client connecté depuis %s:%d\n", ip_client, port_client);

// 6.  Bloc 6 — Lire un message du client et lui répondre

    char buffer[1024] = {0};
    read(client_fd, buffer, 1024);

    printf("Message reçu du client : %s\n", buffer);

    // si erreur read retourne 0, code pour tester l'erreur :
    // if (read(client_fd, buffer, 1024) < 0) {
    //     perror("Erreur lors de la lecture");
    // }
    

    char *reponse = "Hello client ! Bienvenue !";
    send(client_fd,reponse, strlen(reponse),0);

// 7. Fermer les sockets

    close(client_fd);   // Ferme la communication avec ce client
    close(server_fd);   // Ferme le socket d'écoute

    return 0;
}