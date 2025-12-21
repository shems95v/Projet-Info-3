# 🌊 C-WildWater — Analyse de données industrielles en C

Nous avons deux branches sur Github: une qui se nomme **main** qui contient tous les fichiers du projet **sans le Bonus** et une autre qui se nomme **Bonus** qui contient tous les fichiers du projet **avec le Bonus**

## Présentation du projet

Ce projet a été réalisé en langage C dans le but d’analyser des données industrielles
contenues dans un fichier CSV. Les données concernent différentes usines et permettent
d’étudier des volumes, des valeurs maximales ainsi que des pourcentages de fuites.

Le programme repose sur une organisation modulaire du code et sur l’utilisation
de structures de données comme les arbres binaires et les arbres AVL afin d’optimiser
le stockage et le traitement des informations.

----------------------------------------------------------------

## Organisation des fichiers

Le projet est organisé de la manière suivante :

main.c              : fichier principal, point d’entrée du programme  
csv_reader.c / .h   : lecture et traitement du fichier CSV  
usine.c / .h        : définition de la structure Usine et fonctions associées  
tree.c / .h         : implémentation d’un arbre binaire de recherche  
avl.c / .h          : implémentation d’un arbre AVL (arbre équilibré)  
Makefile            : compilation automatique du projet  
run.sh              : script shell pour exécuter le programme  
c-wildwater_v3      : fichier de données utilisé en entrée  

----------------------------------------------------------------

## Compilation et exécution

1) Télécharger le projet au format ZIP  
2) Extraire l’archive puis ouvrir le dossier du projet  
3) Ouvrir un terminal dans ce dossier  

Compiler le projet :
make

Rendre le script shell exécutable :
chmod +x run.sh

Lancer le programme :
./run.sh c-wildwater_v3.dat **commande** **mode** 

Nettoyer le projet :
make clean

(Cette commande permet de supprimer les fichiers générés lors de la compilation.
Elle peut être utilisée avant de recompiler le projet ou pour libérer de l’espace)


----------------------------------------------------------------

## Commandes disponibles

COMMANDE : histo
----------------
Cette commande permet de générer des histogrammes à partir des données du fichier.

Modes possibles :
- max
- src
- real
- all (pour le bonus)

Résultats obtenus :
- un fichier .dat contenant les données intermédiaires
- deux images .png correspondant aux histogrammes générés

------------------------------------------------------------

COMMANDE : leaks
----------------
Cette commande permet d’analyser les fuites par usine.

Mode :
- "id_usine"

Résultat obtenu :
- un fichier .dat contenant pour chaque usine :
- son identifiant
- le pourcentage de fuites associé
- les identifiants amont et aval du tronçon qui perd le plus d’eau (pour le bonus)

----------------------------------------------------------------

## Principe général du programme

Le programme fonctionne selon les étapes suivantes :

1) Lecture du fichier CSV  
2) Création des structures représentant les usines  
3) Insertion des données dans un arbre (binaire ou AVL)  
4) Traitement des données selon la commande choisie  
5) Génération des fichiers de sortie (.dat et images)

----------------------------------------------------------------

## Outils et notions utilisés

- Langage C
- Structures et pointeurs
- Lecture et traitement de fichiers CSV
- Arbres binaires de recherche
- Arbres AVL
- Makefile
- Script shell

----------------------------------------------------------------

## 👥 Auteurs

Projet réalisé par :  
**Taissir RGUIG**, **Shems BOUDJEBINA**, **Sara BELGHAZI**

Dans le cadre du projet d'informatique 3 — année universitaire **2025–2026**.


