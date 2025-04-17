# ⚙️ Comparaison : `select()` vs `threads` vs `fork()` en C

Quand on veut gérer plusieurs clients dans un serveur réseau, on peut utiliser plusieurs approches :  
- `select()` pour surveiller plusieurs connexions dans une boucle
- `threads` pour créer un flux d'exécution par client
- `fork()` pour créer un processus séparé pour chaque client

Voici un tableau comparatif des 3 solutions :

| 🧩 Critère                          | `select()` 🧲                        | `threads` 🧵                        | `fork()` 🍴                           |
|------------------------------------|-------------------------------------|-------------------------------------|----------------------------------------|
| 📚 Fonctionnement                  | Boucle unique qui surveille tout    | Un thread par client                | Un processus par client                |
| 🧵 Parallélisme                    | ❌ Non (séquentiel dans une boucle) | ✅ Oui                              | ✅ Oui                                 |
| 🔁 Ressources système              | Très léger                          | Moyen (pile par thread)            | 🐘 Lourd (processus complet)           |
| 🧠 Complexité du code              | ✅ Simple (pas de synchronisation)  | ⚠️ Synchronisation nécessaire (mutex) | ❌ Impossible de partager des variables |
| 📦 Partage de données              | ✅ Facile (même espace mémoire)     | ✅ Possible                         | ❌ Non (mémoire séparée)               |
| 💥 Risques                        | Une erreur peut bloquer la boucle   | Deadlocks, race conditions          | Moins de bugs internes, mais plus lourd |
| 📈 Scalabilité                    | ✅ Très bon jusqu’à 1000+ clients   | ⚠️ Limité à quelques centaines      | ❌ Moins bon au-delà de 50 clients     |
| 💬 Idéal pour...                  | Serveur de chat, socket léger       | Jeux, tâches lourdes séparées      | Apprentissage, exos simples, mini-serveur |

---

## 🧠 Résumé simple

| Tu veux faire...                                    | Choix recommandé     |
|------------------------------------------------------|-----------------------|
| Un serveur de chat                                  | `select()` ✅         |
| Du calcul parallèle, des IA ou jeux                 | `threads` ✅          |
| Apprendre la base des sockets, gérer 1 client à la fois | `fork()` ✅           |
| Gérer un grand nombre de connexions TCP             | `select()` ou `poll()` 🔥 |

---

## 🔍 Exemple visuel

```text
Avec `select()` :
[Serveur]
    ↳ Surveille tous les clients dans une seule boucle

Avec `threads` :
[Thread 1] ↔ client 1
[Thread 2] ↔ client 2
[Thread 3] ↔ client 3

Avec `fork()` :
[Processus fils 1] ↔ client 1
[Processus fils 2] ↔ client 2
[Processus fils 3] ↔ client 3
