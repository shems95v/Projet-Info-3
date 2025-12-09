/* analyse.c
   Version simple (niveau bac+2) adaptée au fichier WildWater
   On détecte les lignes "USINE seule" et on récupère :
     - nom de l'usine = colonne 2
     - volume_reel   = colonne 4 (converti en long)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyse.h"

/*
  analyseLigne :
  - attend une ligne CSV avec 5 colonnes séparées par ';'
  - si la ligne correspond au type "USINE" (col1='-' , col2 non '-' , col3='-' , col4 != '-' )
    alors on remplit out->nom = colonne2 et out->volume_reel = colonne4
  - retourne :
    0 : succès (ligne usine remplie)
    1 : paramètre invalide
    2 : ligne mal formée / colonnes manquantes
    3 : ligne non-usine (aucune extraction faite)
*/
int analyseLigne(const char *line, Usine *out)
{
    if (line == NULL || out == NULL)
        return 1;

    char buffer[2048];
    char *c1, *c2, *c3, *c4, *c5;

    /* copie la ligne car strtok modifie la chaîne */
    strncpy(buffer, line, sizeof(buffer)-1);
    buffer[sizeof(buffer)-1] = '\0';

    c1 = strtok(buffer, ";\n");
    c2 = strtok(NULL, ";\n");
    c3 = strtok(NULL, ";\n");
    c4 = strtok(NULL, ";\n");
    c5 = strtok(NULL, ";\n");

    if (!c1 || !c2 || !c3 || !c4 || !c5) {
        return 2; /* ligne mal formée */
    }

    /* Détection d'une ligne "USINE seule" selon le sujet :
       col1 = '-' , col2 = identifiant usine , col3 = '-' , col4 = capacité , col5 = '-'
       On considère "USINE" si col2 != "-" et col3 == "-" et col4 != "-"
    */
    if (strcmp(c1, "-") == 0 && strcmp(c2, "-") != 0 &&
        strcmp(c3, "-") == 0 && strcmp(c4, "-") != 0) {

        /* remplir la structure Usine */
        strncpy(out->nom, c2, sizeof(out->nom)-1);
        out->nom[sizeof(out->nom)-1] = '\0';

        /* conversion volume : colonne 4 donnée en milliers de m3 (k.m3)
           on garde en 'long' la valeur telle quelle (ex : 4749292)
        */
        out->volume_reel = atol(c4);

        return 0; /* succès, ligne usine */
    }

    /* sinon : on ne traite pas cette ligne (pas une ligne "USINE seule") */
    return 3;
}

/* -------------------------------------------------
   Fonction d'insertion simple dans l'arbre binaire
   Tri alphabétique sur le nom
   ------------------------------------------------- */
Arbre* insererUsine(Arbre *root, Usine u)
{
    if (root == NULL) {
        Arbre *n = malloc(sizeof(Arbre));
        if (!n) {
            fprintf(stderr, "Erreur allocation memoire\n");
            exit(1);
        }
        n->u = u;
        n->fg = NULL;
        n->fd = NULL;
        return n;
    }

    if (strcmp(u.nom, root->u.nom) < 0) {
        root->fg = insererUsine(root->fg, u);
    } else {
        root->fd = insererUsine(root->fd, u);
    }
    return root;
}

/* -------------------------------------------------
   Affichage infixe de l'arbre
   ------------------------------------------------- */
void afficherArbre(Arbre *root)
{
    if (!root) return;
    afficherArbre(root->fg);
    printf("%s ; %ld\n", root->u.nom, root->u.volume_reel);
    afficherArbre(root->fd);
}

/* -------------------------------------------------
   Libération mémoire
   ------------------------------------------------- */
void freeArbre(Arbre *root)
{
    if (!root) return;
    freeArbre(root->fg);
    freeArbre(root->fd);
    free(root);
}

