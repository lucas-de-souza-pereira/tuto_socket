#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>      // inet_ntoa(), htons()
#include <pthread.h>        // threads POSIX
#include <signal.h>         // gestion Ctrl+C
#include <netinet/in.h>     // struct sockaddr_in

#define PORT 8080
#define MAX_CLIENTS 100

int server_fd;


void handle_sigint(int sig) {
    printf("\nArrêt du serveur (Ctrl+C)\n");
    if (server_fd != -1) {
        close(server_fd);
        printf("Socket serveur fermé.\n");
    }
    exit(0);
}


// Fonction qui gère la communication avec un client
void *handle_client(void *arg) {
    int client_fd = *(int *)arg;   // Récupérer la valeur passée (le socket du client)
    free(arg);                     // Libérer la mémoire allouée dans main()

    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        int bytes_read = read(client_fd, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            printf("Client %d déconnecté.\n", client_fd);
            break;
        }

        printf("Message reçu de %d : %s\n", client_fd, buffer);

        // Gestion des messages
        if (strcmp(buffer, "exit") == 0) {
            send(client_fd, "Déconnexion.\n", strlen("Déconnexion.\n"), 0);
            break;
        } else if (strcmp(buffer, "ping") == 0) {
            send(client_fd, "pong\n", strlen("pong\n"), 0);
        } else {
            send(client_fd, "Bien reçu !\n", strlen("Bien reçu !\n"), 0);
        }
    }

    close(client_fd);  // Fermer la connexion avec ce client
    return NULL;
}


int main() {
    struct sockaddr_in server_addr;

    signal(SIGINT, handle_sigint);

    // 1. Créer le socket serveur (TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    
    // 2. Définir les infos du serveur
    server_addr.sin_family = AF_INET;            // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;    // Toutes les interfaces
    server_addr.sin_port = htons(PORT);          // Port d'écoute

    // 3. Lier le socket à l’adresse
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur bind");
        exit(EXIT_FAILURE);
    }

    // 4. Mettre le socket en écoute
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Erreur listen");
        exit(EXIT_FAILURE);
    }

    printf("Serveur lancé sur le port %d. En attente de connexions...\n", PORT);

    //boucle principale
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        // 1. Accepter une connexion entrante
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Erreur accept");
            continue;
        }

        // 2. Afficher IP + port du client
        char *ip = inet_ntoa(client_addr.sin_addr);
        int port = ntohs(client_addr.sin_port);
        printf("🆕 Nouveau client connecté depuis %s:%d\n", ip, port);


        // 3. Allouer dynamiquement le socket pour le thread
        int *pclient = malloc(sizeof(int)); 
        if (pclient == NULL) {
            perror("Erreur malloc");
            close(client_fd);
            continue;
        }
        *pclient = client_fd;
        
        // int *pclient = malloc(sizeof(int)); demande dynamiquement de la mémoire.
        // En cas de surcharge mémoire (rare, mais possible), malloc() peut échouer et retourner NULL.
        
        // Si tu fais *pclient = client_fd; sans vérifier que pclient != NULL,
        // tu accèdes à une adresse invalide → segmentation fault 

        // 4. Créer un thread pour ce client
        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, pclient) != 0) { //handle_client = fonction pour le thread
            perror("Erreur création thread");
            close(client_fd);
            free(pclient);
        }

        // (optionnel) Détacher le thread pour éviter les fuites mémoire
        pthread_detach(tid);

        // Quand on crées un thread avec pthread_create(), il reste "joignable" par défaut :
        // Ce qui signifie on pourrait faire un pthread_join() plus tard pour attendre sa fin.
        // Sauf que s'il n'y a pas de pthread_join() ni pthread_detach(), le système garde ses ressources en mémoire → fuite mémoire !

            // En résumé :
            // Que devient le thread ?
      // Cas                      |  Que devient le thread ?
            // pthread_join(thread)     |  Tu attends qu’il finisse (et libères)
            // pthread_detach(thread)   |  Il se "libère" tout seul à sa fin
            // Aucun des deux           |  Fuite mémoire (le thread reste zombie)

            // Dans notre cas, on ne veut pas attendre chaque thread (sinon le serveur bloquerait)
            // → donc pthread_detach(tid); est la meilleure solution ici

    }

    close(server_fd);
    return 0;
}


