# 🔀 Serveur multi-clients en C avec `fork()`

Ce projet montre comment créer un serveur TCP **capable de gérer plusieurs clients simultanément** grâce à la création de **processus fils** via `fork()`.

---

## 📦 Objectifs

- Créer un **serveur TCP** basique
- Gérer plusieurs clients **en parallèle** via `fork()`
- Gérer la **fermeture propre** avec `Ctrl+C` (`SIGINT`)
- Réagir à des commandes simples (`ping`, `exit`, etc.)

---

## ⚙️ Fonctionnement

À chaque nouvelle connexion client :

1. Le serveur appelle `accept()`
2. Il lance un nouveau **processus enfant** avec `fork()`
3. Le processus enfant **gère la communication avec le client**
4. Le parent retourne dans la boucle `accept()` pour écouter d'autres clients

---

## 🧠 Pourquoi `fork()` ?

| Avantage                            | Description |
|-------------------------------------|-------------|
| 🌱 Chaque client est isolé          | Un processus dédié gère un seul client |
| 🧩 Simplicité logique               | Pas besoin de gérer une boucle complexe |
| 🔧 Gère les connexions en parallèle | Les clients peuvent parler en même temps |

---

## 🔁 Gestion des messages

Le serveur :

- Répond **`pong`** si le client envoie `ping`
- Répond **`Bien reçu !`** pour tout autre message
- Ferme proprement la connexion si le client se déconnecte

---

## 🛑 Ctrl+C (SIGINT)

Le serveur intercepte `Ctrl+C` :

```c
signal(SIGINT, handle_sigint);
