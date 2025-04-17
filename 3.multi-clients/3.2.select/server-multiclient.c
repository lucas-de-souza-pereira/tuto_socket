#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>             // close()
#include <sys/socket.h>         // socket(), bind(), listen(), accept()
#include <netinet/in.h>         // struct sockaddr_in, htons()
#include <arpa/inet.h>          // inet_ntoa()
#include <sys/select.h>


#define PORT 8080 // port d'écoute du serveur
#define MAX_CLIENTS 10 // nom de connexion max en meme temps 

int main(){

    int server_fd;
    struct sockaddr_in server_addr;
    
    // 1. Créer le socket serveur (TCP/IP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }
    
    // 2. Préparer la structure de l'adresse du serveur
    server_addr.sin_family = AF_INET;              // Famille d'adresses : IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;      // Écouter sur toutes les interfaces
    server_addr.sin_port = htons(PORT);            // Port d'écoute, converti au format réseau
    
    // 3. Lier le socket à l'adresse
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur bind");
        exit(EXIT_FAILURE);
    }
    
    // 4. Mettre le socket en écoute
    if (listen(server_fd, 5) < 0) {
        perror("Erreur listen");
        exit(EXIT_FAILURE);
    }
    // ATTENTION ne pas confondre MAX_CLIENTS et listen
    // Max_client : nombre de personne qui peuvent ouvrir l'application en meme temps
    // listen : c’est le nombre de clients qui peuvent attendre accept()
    

            // Exemple en situation :
            // Ton serveur est lancé
            // Tu as déjà 10 clients connectés (donc ton tableau est plein)
            // Un 11e client essaie de se connecter :
                // Il est mis en file d’attente (dans la "queue" de listen())
            // Si 5 autres arrivent aussi, le système les garde dans la queue aussi
            // Mais dès que tu dépasses la valeur de listen(), les nouveaux clients :
            // Soit reçoivent une erreur ECONNREFUSED
            // Soit sont ignorés silencieusement par l’OS (dépend du système)

    printf("Serveur en écoute sur le port %d...\n", PORT);
    
    int client_sockets[MAX_CLIENTS]; // tableau de sockets clients
    fd_set read_fds; // ensemble des sockets à surveiller 
    int max_fd;     // valeur max à donner à select()

     // Initialiser tous les sockets clients à -1 (vide) / les 'reset tous'
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = -1;
     }

     //boucle principale du serveur 

     while(1){
        // 1. Réinitialiser la liste des sockets à surveiller
        FD_ZERO(&read_fds);
        // vide (réinitialise) la "liste de sockets" qu’on va donner à select() venant de fd_set read_fds; 


        // 2. Ajouter le socket serveur (pour détecter les nouvelles connexions)
        FD_SET(server_fd, &read_fds); // Ajoute le socket serveur à la liste des sockets à surveiller
        max_fd = server_fd;           // On note la plus grande valeur de descripteur

        // 3. Ajouter les sockets clients actifs
        for (int i = 0; i < MAX_CLIENTS; i++){
            int fd = client_sockets[i];
            if (fd > 0){
                FD_SET(fd, &read_fds);
                if (fd > max_fd){
                    max_fd = fd; // pour donner la bonne valeur à select()
                }
            }
        }

//         “Je vais parcourir tous les emplacements dans mon tableau client_sockets[],
//            et pour chaque socket actif (≠ -1),
//          → je l’ajoute à la liste de sockets que je veux que select() surveille.”
//               Et en même temps :
//         “Je garde en mémoire le plus grand numéro de socket rencontré (pour select())”


            //  Étape par étape

        // for (int i = 0; i < MAX_CLIENTS; i++)
        // → On parcourt toutes les cases du tableau client_sockets[].

        // int fd = client_sockets[i];
        // → On récupère la valeur du socket stocké dans cette case.

        // if (fd > 0)
        // → Si c’est un socket valide (par convention, on a mis -1 pour "vide").

        // FD_SET(fd, &read_fds);
        // → On dit à select() :
        // “Surveille ce client ! Préviens-moi quand il envoie un message.”

        // if (fd > max_fd) {
        //     max_fd = fd;
        // }
        // → select() a besoin qu’on lui dise la plage max de sockets à regarder.
        // Si ce client a un descripteur plus élevé que les autres → on met à jour max_fd.


        // Situation du tableau

        // client_sockets[] = {4, -1, 6, -1, 8}

        // Cela veut dire :
        // Tu as 3 clients connectés : leurs sockets sont 4, 6 et 8
        
        // Les cases avec -1 sont vides (pas de client ici)
        
        // Boucle for illustrée

        // Étapes de la boucle :
        
        // i = 0 → fd = 4 → ✅ FD_SET(4), max_fd = 4
        // i = 1 → fd = -1 → ⛔ rien (socket vide)
        // i = 2 → fd = 6 → ✅ FD_SET(6), max_fd = 6
        // i = 3 → fd = -1 → ⛔ rien
        // i = 4 → fd = 8 → ✅ FD_SET(8), max_fd = 8

        // À la fin de cette boucle, on a :
        
        // read_fds = {4, 6, 8}     // sockets surveillés par select()
        // max_fd = 8               // valeur max à passer à select()

        // Ce qu’on donne à select()

        // select(max_fd + 1, &read_fds, NULL, NULL, NULL);
            //       ↑
            //     9 ici
        // Parce que select() vérifie tous les file descriptors de 0 à 8 inclus
        // (oui, même si 0, 1, 2, 3 ne servent à rien ici — c’est normal)
        
        // Résumé visuel

        // +--------------------+
        // | client_sockets[]   |
        // +----+-----+----+----+----+
        // | 4  | -1  | 6  | -1 | 8  |
        // +----+-----+----+----+----+
        
        // → Sockets actifs : 4, 6, 8
        // → max_fd = 8
        // → select(9, &read_fds, ...)



        // 4. Attendre un événement sur l'un des sockets
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("Erreur select");
            continue;
        }
        // int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

        // Paramètre	Rôle
        // nfds	        Le plus grand numéro de socket + 1
        // readfds	    Liste des sockets à surveiller pour lecture
        // writefds	    Liste des sockets à surveiller pour écriture
        // exceptfds	Liste pour les erreurs (rarement utilisé)
        // timeout	    Combien de temps attendre (NULL = bloquant infini)

//         Pourquoi max_fd + 1 ?
        // Le premier paramètre nfds ne dit pas combien de sockets tu surveilles,
        // mais jusqu'à quel numéro de socket le système doit balayer.

        //   Le système regarde les sockets de 0 à max_fd inclus,
        // donc il faut donner max_fd + 1 pour qu’il aille jusque-là.

        //  Pourquoi NULL, NULL, NULL ?
        // select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        // Champ	    Explication
        // &read_fds	Tu veux écouter les sockets quand ils ont des données à lire
        // NULL	        Tu ne veux pas surveiller l'écriture pour le moment
        // NULL	        Tu ne veux pas surveiller les erreurs réseau
        // NULL	        Tu ne veux pas de timeout → tu bloques indéfiniment jusqu'à ce qu'un socket soit prêt

     }

}