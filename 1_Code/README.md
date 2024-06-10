# Protocole de communication client/serveur

Tous les messages envoyés entre le serveur et les clients sont composés de la façon suivante : 

- type de message : 

C -> Connexion

R -> Réponse

G -> Grille

S -> SLAM

I -> Information

- argument : string

Exemple : 

Pour la connection avec le nom de l'utilisateur, on utilise la ligne suivante : 

C_Basile
