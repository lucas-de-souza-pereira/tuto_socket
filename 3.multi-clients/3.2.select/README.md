# 💻 Serveur Multi-client avec `select()` (en C)

Ce projet montre comment créer un **serveur TCP capable de gérer plusieurs clients simultanément** sans threads, en utilisant la fonction système `select()`.

---

## 📦 Objectif

Apprendre pas à pas à :

- Créer un serveur TCP
- Gérer plusieurs connexions clients en même temps
- Utiliser `select()` pour surveiller plusieurs sockets
- Réagir à des messages simples (`ping`, `exit`, autres)

---

## 🚀 Fonctionnement

Le serveur :

- Accepte jusqu’à **10 clients** (`MAX_CLIENTS`)
- Répond à chaque message :
  - `ping` → répond `pong`
  - `exit` → ferme proprement la connexion
  - tout autre message → répond `Bien reçu !`
- Gère `Ctrl+C` (signal `SIGINT`) pour fermer tout proprement

---

## 🧠 Étapes de fonctionnement

1. Créer un socket serveur
2. Associer une adresse IP et un port (`bind`)
3. Mettre en écoute (`listen`)
4. Boucle infinie :
   - `select()` surveille tous les sockets
   - Nouvelle connexion ? → `accept()`
   - Message d’un client ? → `read()` + `send()`
5. Déconnexion automatique sur `exit` ou erreur

---
