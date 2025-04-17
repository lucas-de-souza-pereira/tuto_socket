#include <stdio.h>              // printf(), perror() → affichage, messages d’erreur
#include <stdlib.h>             // exit(), malloc() → fonctions système, allocation
#include <string.h>             // memset(), strlen(), strcpy(), strcmp() → gestion des chaînes
#include <unistd.h>             // close(), read(), write() → appels système de bas niveau
#include <sys/socket.h>         // socket(), bind(), listen(), accept(), send(), recv() → fonctions de sockets
#include <netinet/in.h>         // struct sockaddr_in, htons() → adresses réseau et conversions
#include <arpa/inet.h>          // inet_ntoa(), ntohs() → conversion IP et port
#include <signal.h>             // signal(), SIGINT → gestion des signaux système (ex : Ctrl+C)


int server_fd;
int client_fd;

void handle_sigint(int sig){
    printf("\n[Ctrl+C] Arrêt du serveur...\n");

    if (client_fd != -1){
        close(client_fd);
        printf("Connexion client fermée.\n");
    }

    if (server_fd != -1){
        close(server_fd);
        printf("Socket serveur fermé.\n");
    }


    exit(0);
}

int main(){

    signal(SIGINT, handle_sigint);

    server_fd = socket(AF_INET,SOCK_STREAM,0);

    if (server_fd == -1) {
        perror("Erreur socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(8080);


    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur bind");
        exit(1);
    }
    

    listen(server_fd,5);
    printf("Serveur en écoute sur le port 8080...\n");

    while(1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        client_fd= accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == -1){
            perror("Erreur lors de accept");
            continue;
        }

        pid_t pid = fork();


        if (pid == 0){
       
            close(server_fd); //


        // Affichage de l’IP et port du client
        char *ip = inet_ntoa(client_addr.sin_addr);
        int port = ntohs(client_addr.sin_port);
        printf("[Fil %d] Nouveau client connecté depuis %s:%d\n", getpid(), ip, port);


        char buffer[1024];

        while (1)
            {
            memset(buffer,0,sizeof(buffer));
            int bytes_read = read(client_fd, buffer, sizeof(buffer));

            if (bytes_read <=0){
                printf("[Fils %d] Client déconnecté.\n", getpid());
                break;
            }
            
            
            printf("[Fils %d] Reçu : %s\n", getpid(), buffer);

            if (strcmp(buffer, "ping") == 0) {
                send(client_fd, "pong", 4, 0);
                } else {
                    send(client_fd, "Bien reçu !", strlen("Bien reçu !"), 0);
                }
            }

        close(client_fd); 
        exit(0);          
    }
            
        close(client_fd); 
    }
}
    