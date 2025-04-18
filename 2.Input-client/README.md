# 💬 Serveur / Client TCP interactif en C

Ce projet montre comment créer une **communication bidirectionnelle simple** entre un client et un serveur en **C**, avec la possibilité d’entrer du texte côté client (via `stdin`) et de le traiter côté serveur.

---

## 🎯 Objectif

- Créer un serveur TCP
- Créer un client interactif avec `fgets()`
- Permettre des **échanges de messages texte**
- Répondre à des commandes (`ping`, `exit`)
- Gérer la fermeture propre (`Ctrl+C`, déconnexion)

---

## 🔧 Fonctionnement

- Le client saisit un message (comme un mini chat)
- Le message est envoyé au serveur
- Le serveur affiche, traite, puis répond
- Le client affiche la réponse

---

## ⚙️ Fonctionnalités du serveur

- Accepte **un seul client à la fois**
- Répond :
  - `ping` → `pong`
  - `exit` → ferme la connexion
  - autre → `Bien reçu !`
- Gère les erreurs et déconnexions
- Gère `Ctrl+C` pour une **fermeture propre**

---

## 📱 Fonctionnalités du client

- Saisie d’un message avec `fgets()` (clavier)
- Affiche la réponse du serveur
- Tapez `exit` pour quitter
- Gère les erreurs et la fermeture du serveur
- Ignore les crashs via `SIGPIPE`

---

## 🛠 Compilation

```bash
gcc -o serveur server.c
gcc -o client client.c
