#include <stdio.h>              // printf(), perror()
#include <stdlib.h>             // exit()
#include <string.h>             // memset(), strlen(), strcpy()
#include <unistd.h>             // close(), read(), write()
#include <sys/socket.h>         // socket(), bind(), listen(), accept(), send(), recv()
#include <netinet/in.h>         // struct sockaddr_in, htons()
#include <arpa/inet.h>          // inet_ntoa(), ntohs()

int main() {
    int server_fd;

    // 1. Créer le socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Définir l’adresse du serveur
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // 3. Lier le socket à l’adresse
    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Mettre le socket en écoute
    listen(server_fd, 5);
    printf("Serveur en écoute sur le port 8080...\n");

    // 5. Accepter un client
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

    // 6. Afficher l’IP et le port du client
    char *ip_client = inet_ntoa(client_addr.sin_addr);
    int port_client = ntohs(client_addr.sin_port);
    printf("Client connecté depuis %s:%d\n", ip_client, port_client);

    // 7. Boucle de communication
    while (1) {
        char buffer[1024] = {0};

        int bytes_read = read(client_fd, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("Client déconnecté proprement.\n");
            } else {
                perror("Erreur lors de la lecture");
            }
            break;
        }

        printf("Message reçu : %s\n", buffer);

        // Réponse simple
        if (strcmp(buffer, "ping") == 0) {
            send(client_fd, "pong", 4, 0);
        } else {
            send(client_fd, "Bien reçu !", strlen("Bien reçu !"), 0);
        }        
    }

    // 8. Fermer les sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
