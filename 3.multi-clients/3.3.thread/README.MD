# 🧵 Serveur multi-clients avec Threads en C

Ce projet montre comment créer un serveur TCP multi-clients en C, capable de gérer plusieurs connexions en parallèle via les **threads POSIX (`pthread`)**.

Chaque client connecté est géré dans un **thread indépendant**, ce qui permet des communications simultanées.

---

## 🛠️ Fonctionnalités

- 🔄 Accepte plusieurs connexions clients
- 🧵 Crée un thread par client (`pthread_create`)
- 📡 Répond à certains messages :
  - `ping` → `pong`
  - `exit` → déconnexion propre
  - autres → "Bien reçu !"
- ✅ Gestion propre des signaux (`Ctrl+C`)
- 🧼 Pas de fuites mémoire grâce à `pthread_detach()`

---

