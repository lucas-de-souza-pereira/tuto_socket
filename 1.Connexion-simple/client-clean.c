#include <stdio.h>              // printf, perror...
#include <stdlib.h>             // exit
#include <string.h>             // memset, strcpy, strlen...
#include <unistd.h>             // close()
#include <arpa/inet.h>          // inet_pton, htons, sockaddr_in
#include <sys/socket.h>         // socket, connect, send, recv...

// coté client : 
// 1. Créer le socket du client
// 2. Définir l'addresse du serveur à contacter 
// 3. Se connecter au serveur
// 4. Envoyer un message au serveur
// 5. Lire la réponse du serveur
// 6. Fermer la connexion

int main(){

    // 1. Créer le socket du client
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    //2. Définir l'addresse du serveur à contacter 

    struct sockaddr_in server_addr; 
    server_addr.sin_family = AF_INET; // Ipv4
    server_addr.sin_port = htons(8080); // port du serveur 
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); // Ip du serveur (local host)


// 3. Se connecter au serveur

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

// 4. Envoyer un message au serveur

        char *message = "Bonjour serveur !";
        send(client_fd, message, strlen(message), 0);

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