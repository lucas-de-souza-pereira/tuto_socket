# 🔗 Connexion simple : Serveur / Client TCP en C

Petit projet pédagogique pour apprendre à établir une **connexion TCP de base** entre un serveur et un client en C.

---

## 🎯 Objectif

- Créer un serveur TCP qui accepte une seule connexion
- Créer un client qui se connecte, envoie un message, et lit la réponse
- Comprendre les bases des sockets avec `socket()`, `bind()`, `accept()`, `connect()`, `send()`, `recv()`

---

## 🧱 Étapes du serveur

1. Créer un socket avec `socket()`
2. Attacher le socket à une adresse IP + port (`bind()`)
3. Se mettre en écoute (`listen()`)
4. Attendre une connexion (`accept()`)
5. Lire le message du client (`read()`)
6. Envoyer une réponse (`send()`)
7. Fermer les sockets (`close()`)

---

## 🧱 Étapes du client

1. Créer un socket (`socket()`)
2. Spécifier l’adresse du serveur (`inet_pton()`)
3. Se connecter (`connect()`)
4. Envoyer un message (`send()`)
5. Lire la réponse (`read()`)
6. Fermer la connexion (`close()`)

---

## ⚙️ Compilation

```bash
gcc -o serveur serveur.c
gcc -o client client.c
