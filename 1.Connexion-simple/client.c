#include <stdio.h>              // printf, perror...
#include <stdlib.h>             // exit
#include <string.h>             // memset, strcpy, strlen...
#include <unistd.h>             // close()
#include <arpa/inet.h>          // inet_pton, htons, sockaddr_in
#include <sys/socket.h>         // socket, connect, send, recv...


// 1. Créer le socket du client
// 2. Définir l'addresse du serveur à contacter 
// 3. Se connecter au serveur
// 4. Envoyer un message au serveur
// 5. Lire la réponse du serveur
// 6. Fermer la connexion

int main(){

    // 1. Créer le socket du client
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    // pareil pour le serveur : 
    // AF : IPV4 , TCP, protocol


//2. Définir l'addresse du serveur à contacter 
    // pareil que pour le côté serveur sauf qu'on définie IP :
    struct sockaddr_in server_addr; 
    server_addr.sin_family = AF_INET; // Ipv4
    server_addr.sin_port = htons(8080); // port du serveur 
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); // Ip du serveur (local host)
        // Convertit l’adresse IP en format texte ("127.0.0.1")
        // → en format binaire utilisable par la socket (remplit sin_addr)

        // Pourquoi inet_pton ?
        // inet_pton = presentation to network
        // C’est la version moderne et sécurisée de inet_addr

// 3. Se connecter au serveur

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    // Tente de se connecter à l’adresse IP et au port spécifiés.
    // Le cast est nécessaire car connect() attend une sockaddr générique. (comme pour le serveur)

    // Si la connexion échoue (ex : le serveur n’est pas lancé), connect() retournera -1 (voir 3.bis)

    // 3.bis Se connecter au serveur et printer un message erreur ->> si pas connecté c'est égal à -1 donc print l'erreur
        // if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        //     perror("Erreur lors de la connexion");
        //     exit(EXIT_FAILURE);  // Arrêter le programme si la connexion échoue
        // }


// 4. Envoyer un message au serveur

        char *message = "Bonjour serveur !";
        send(client_fd, message, strlen(message), 0);
        // Envoie le message via le socket connecté (client_fd)

        // 0 = flags , différents flags : 
            // Flag | Description
            // 0 | Aucun comportement spécial (envoi standard) 
            // MSG_DONTWAIT | Envoi non bloquant (ne bloque pas si la file est pleine)
            // MSG_NOSIGNAL | Ne déclenche pas de signal SIGPIPE si le serveur a fermé le socket (pratique sous Linux)
            // MSG_OOB | Envoie des données urgentes (rarement utilisé)


        //4.bis vérif, meme logique si send échoue retourn -1 :
            // if (send(client_fd, message, strlen(message), 0) < 0) {
            //   perror("Erreur lors de l'envoi");
            //     }


// 5. Lire la réponse du serveur

        char buffer[1024] = {0};
        read(client_fd, buffer, sizeof(buffer));
        printf("Réponse du serveur : %s\n", buffer);


        // tester l'erreur :
        // if (read(client_fd, buffer, sizeof(buffer)) < 0) {
        //     perror("Erreur lors de la lecture");
        // }
        

// 6. Fermer la connexion

        close(client_fd); // Ferme le socket client     


    return 0;
    }