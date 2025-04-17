#include <stdio.h>              // printf(), perror() → affichage, messages d’erreur
#include <stdlib.h>             // exit(), malloc() → fonctions système et mémoire
#include <string.h>             // memset(), strlen(), strcpy(), strcmp(), strcspn()
#include <unistd.h>             // close(), read(), write() → gestion des fichiers/sockets
#include <arpa/inet.h>          // inet_pton(), htons(), sockaddr_in → IP / ports
#include <sys/socket.h>         // socket(), connect(), bind(), listen(), accept(), send(), recv()
#include <signal.h>             // signal(), SIGINT → gestion des signaux système (Ctrl+C, interruption)


int client_fd;

// fonction pour fermer le client proprement lors d'un ctrl+C
void handle_sigint(int sig) {
    printf("\n[Ctrl+C] Interruption détectée, fermeture du client...\n");
    close(client_fd);
    exit(0);
}


int main(){

    client_fd = socket(AF_INET, SOCK_STREAM,0);
    signal(SIGINT, handle_sigint);  // Gérer Ctrl+C
    signal(SIGPIPE, SIG_IGN); // Ignore les plantages si serveur fermé


    struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(8080);
        inet_pton(AF_INET,"127.0.0.1", &server_addr.sin_addr);

    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));



    char message[1024];
    char buffer[1024];

while (1) {
    // 1. Saisie utilisateur
    printf("Message à envoyer au serveur > ");
    fgets(message, sizeof(message), stdin);
    // stdin = équivalent de input, c'est ce qui permet de rentrer une donnée au clavier

    // Élément	Signification	Utilisé pour...
    // stdin	Standard input	Lire les entrées clavier (fgets, scanf)
    // stdout	Standard output	Afficher du texte (printf)
    // stderr	Standard error	Afficher les erreurs (perror)

    message[strcspn(message, "\n")] = 0; 
    // sert à supprimer le \n que fgets() ajoute automatiquement à la fin du texte lu au clavier.
    // Elle retourne l’index du premier caractère trouvé dans la chaîne message qui correspond à l’un des caractères de "\n"
    // elle remplaces le \n par \0, donc coupe proprement la chaîne 

    // 2. Condition de sortie
    // strcmp(..., "exit")	On vérifie si c’est “exit” pour quitter
    if (strcmp(message, "exit") == 0) {
        printf("Déconnexion.\n");
        break;
    }

    // 3. Envoi du message (avec vérif si le serveur est fermé)
    if (send(client_fd, message, strlen(message), 0) < 0) {
        perror("Erreur lors de l'envoi (serveur peut-être déconnecté)");
        printf("Déconnexion du client...\n");
        break;
    }
    // 4. Lecture de la réponse simple
        // memset(buffer, 0, sizeof(buffer)); // vider le buffer avant lecture
        // read(client_fd, buffer, sizeof(buffer));
        // printf("Serveur > %s\n", buffer);


    // 4.bis Lecture de la réponse (si le serveur répond encore)
    memset(buffer, 0, sizeof(buffer));
    int bytes_read = read(client_fd, buffer, sizeof(buffer));

    if (bytes_read <= 0) {
        printf("❌ Le serveur a fermé la connexion.\n");
        break;
    }

    printf("Serveur > %s\n", buffer);
}


    close(client_fd);

    return 0;
}
