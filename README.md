# 🧠 tuto_socket

Je n'ai pas trouvé de tutoriel clair qui explique **pas à pas** comment construire des sockets en C.  
J'ai donc décidé de le faire moi-même, en partageant ici **plusieurs mini-projets** commentés pour apprendre en pratiquant.

> 💡 Si tu débutes, je te recommande de **lire un peu de théorie avant de foncer dans le code**.  
> Voici deux excellents articles pour commencer :
> 
> - https://dev.to/sanjayrv/a-beginners-guide-to-socket-programming-in-c-5an5  
> - https://medium.com/developer-student-clubs-tiet/creating-your-own-chat-server-with-c-34f11c752ffb

---

## 🔧 Contenu du projet

Chaque étape correspond à une **version évoluée** du client/serveur :

1. 🖨️ Serveur + client simples (affichage de messages)
2. ⌨️ Saisie clavier côté client (input dynamique)
3. 👥 Serveur multi-clients (avec `fork()`)
4. 🗄️ Multi-clients + Connexion à une base de données PostgreSQL (en cours)

Chaque programme est **commenté en détail**, ligne par ligne, pour aider à comprendre la logique.

---

## 🙌 Pourquoi ce projet ?

- Pour apprendre à construire **pas à pas** une communication réseau en C.
- Pour bien séparer la logique **client / serveur**.
- Pour comprendre comment aller **de la théorie au projet réel**.

---

## ⭐ Contribuer

Tu vois des améliorations possibles ?  
Une faute ? Une optimisation ? Une nouvelle idée ?  
➡️ **PR bienvenue !**

Et si ce repo t’aide, pense à lâcher une petite ⭐ :)

---

## 📫 Contact

Si tu veux échanger sur le sujet ou poser une question, tu peux ouvrir une issue ou me contacter sur [GitHub](https://github.com/ton_pseudo).

---

## 🚀 À venir

- [ ] Version avec threads (`pthread`)
