#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 8080

int client_fd;

// Fermer proprement à Ctrl+C
void handle_sigint(int sig) {
    printf("\n[Ctrl+C] Fermeture du client...\n");
    if (client_fd != -1) {
        close(client_fd);
    }
    exit(0);
}

// Thread pour recevoir les messages du serveur
void *receive_messages(void *arg) {
    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0) {
            printf("\nConnexion fermée par le serveur.\n");
            exit(0);
        }

        printf("\r📩 %s", buffer);
        printf("📝 Vous > "); fflush(stdout); // Réafficher l’invite après réception
    }

    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    signal(SIGINT, handle_sigint);

    // 1. Créer le socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    // 2. Définir adresse du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 3. Connexion
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur connexion");
        exit(EXIT_FAILURE);
    }

    printf("✅ Connecté au serveur sur le port %d\n", PORT);

    // 4. Lancer le thread de réception
    pthread_t recv_thread;
    if (pthread_create(&recv_thread, NULL, receive_messages, NULL) != 0) {
        perror("Erreur thread réception");
        close(client_fd);
        return 1;
    }

    // 5. Envoyer des messages depuis stdin
    char message[1024];
    while (1) {
        printf("📝 Vous > ");
        fgets(message, sizeof(message), stdin);

        message[strcspn(message, "\n")] = 0; // Enlever \n
        if (strcmp(message, "exit") == 0) {
            printf("Déconnexion...\n");
            break;
        }

        send(client_fd, message, strlen(message), 0);
    }

    close(client_fd);
    return 0;
}
