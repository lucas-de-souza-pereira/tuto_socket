#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>             // close()
#include <sys/socket.h>         // socket(), bind(), listen(), accept()
#include <netinet/in.h>         // struct sockaddr_in, htons()
#include <arpa/inet.h>          // inet_ntoa()
#include <sys/select.h>
#include <signal.h>             // signal() et SIGINT

#define PORT 8080
#define MAX_CLIENTS 10

int server_fd;
int client_sockets[MAX_CLIENTS];

// Gestion du Ctrl+C
void handle_sigint(int sig) {
    printf("\nCtrl+C détecté. Fermeture du serveur...\n");

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != -1) {
            close(client_sockets[i]);
            printf("Client %d déconnecté.\n", client_sockets[i]);
        }
    }

    if (server_fd != -1) {
        close(server_fd);
        printf("Socket serveur fermé.\n");
    }

    exit(0);
}

int main() {
    struct sockaddr_in server_addr;

    // 1. Créer le socket serveur
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_sigint);

    // 2. Préparer l'adresse du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 3. Associer le socket à l'adresse
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur bind");
        exit(EXIT_FAILURE);
    }

    // 4. Mettre le socket en écoute
    if (listen(server_fd, 5) < 0) {
        perror("Erreur listen");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en écoute sur le port %d...\n", PORT);

    // 5. Initialiser les sockets clients
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = -1;
    }

    // Boucle principale
    while (1) {
        fd_set read_fds;
        int max_fd = server_fd;

        // 6. Réinitialiser l'ensemble à surveiller
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);

        // 7. Ajouter les sockets clients actifs
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int fd = client_sockets[i];
            if (fd != -1) {
                FD_SET(fd, &read_fds);
                if (fd > max_fd) max_fd = fd;
            }
        }

        // 8. Appel à select()
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("Erreur select");
            continue;
        }

        // 9. Nouvelle connexion entrante
        if (FD_ISSET(server_fd, &read_fds)) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

            if (new_socket < 0) {
                perror("Erreur accept");
                continue;
            }

            printf("Nouveau client connecté depuis %s:%d\n",
                   inet_ntoa(client_addr.sin_addr),
                   ntohs(client_addr.sin_port));

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == -1) {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        // Lire les messages des clients
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int fd = client_sockets[i];

            if (fd != -1 && FD_ISSET(fd, &read_fds)) {
                char buffer[1024] = {0};
                int bytes_read = read(fd, buffer, sizeof(buffer));

                if (bytes_read <= 0) {
                    printf("Client %d déconnecté.\n", fd);
                    close(fd);
                    client_sockets[i] = -1;
                } else {
                    printf("Message reçu de %d : %s\n", fd, buffer);

                    if (strcmp(buffer, "exit") == 0) {
                        printf("Le client %d a demandé la déconnexion.\n", fd);
                        send(fd, "Bye bye", strlen("Bye bye"), 0);
                        close(fd);
                        client_sockets[i] = -1;
                    } else if (strcmp(buffer, "ping") == 0) {
                        send(fd, "pong", 4, 0);
                    } else {
                        send(fd, "Bien reçu !", strlen("Bien reçu !"), 0);
                    }
                }
            }
        }
    }

    return 0;
}
