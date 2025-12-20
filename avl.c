#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

// Fonction pour calculer le maximum entre deux nombres
int maximum(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Fonction pour obtenir la hauteur d'un noeud
int obtenir_hauteur(NoeudAVL *n) {
    if (n == NULL) {
        return 0;
    }
    return n->hauteur;
}

// Rotation à droite
NoeudAVL* rotation_droite(NoeudAVL *y) {
    NoeudAVL *x = y->gauche;
    NoeudAVL *t2 = x->droite;

    x->droite = y;
    y->gauche = t2;

    // Mise à jour des hauteurs
    y->hauteur = maximum(obtenir_hauteur(y->gauche), obtenir_hauteur(y->droite)) + 1;
    x->hauteur = maximum(obtenir_hauteur(x->gauche), obtenir_hauteur(x->droite)) + 1;

    return x;
}

// Rotation à gauche
NoeudAVL* rotation_gauche(NoeudAVL *x) {
    NoeudAVL *y = x->droite;
    NoeudAVL *t2 = y->gauche;

    y->gauche = x;
    x->droite = t2;

    // Mise à jour des hauteurs
    x->hauteur = maximum(obtenir_hauteur(x->gauche), obtenir_hauteur(x->droite)) + 1;
    y->hauteur = maximum(obtenir_hauteur(y->gauche), obtenir_hauteur(y->droite)) + 1;

    return y;
}

// Calculer le facteur d'équilibre
int calculer_equilibre(NoeudAVL *n) {
    if (n == NULL) {
        return 0;
    }
    return obtenir_hauteur(n->gauche) - obtenir_hauteur(n->droite);
}

// Insertion dans l'arbre AVL
NoeudAVL* avl_inserer(NoeudAVL *noeud, const char *identifiant, NoeudAVL **noeud_trouve) {

    if (noeud == NULL) {
        Usine *u = creer_usine(identifiant);
        *noeud_trouve = malloc(sizeof(NoeudAVL));
        (*noeud_trouve)->usine = u;
        (*noeud_trouve)->gauche = NULL;
        (*noeud_trouve)->droite = NULL;
        (*noeud_trouve)->hauteur = 1;
        return *noeud_trouve;
    }

    // Comparaison des identifiants
    int comparaison = strcmp(identifiant, noeud->usine->identifiant);

    // Si l'identifiant existe déjà
    if (comparaison == 0) {
        *noeud_trouve = noeud;
        return noeud;
    }

    else if (comparaison < 0) {
        noeud->gauche = avl_inserer(noeud->gauche, identifiant, noeud_trouve);
    }
  
    else {
        noeud->droite = avl_inserer(noeud->droite, identifiant, noeud_trouve);
    }

    
    noeud->hauteur = 1 + maximum(obtenir_hauteur(noeud->gauche), obtenir_hauteur(noeud->droite));

    int equilibre = calculer_equilibre(noeud);

    // Cas 1 : Rotation droite simple
    if (equilibre > 1 && strcmp(identifiant, noeud->gauche->usine->identifiant) < 0) {
        return rotation_droite(noeud);
    }

    // Cas 2 : Rotation gauche simple
    if (equilibre < -1 && strcmp(identifiant, noeud->droite->usine->identifiant) > 0) {
        return rotation_gauche(noeud);
    }

    // Cas 3 : Rotation gauche-droite
    if (equilibre > 1 && strcmp(identifiant, noeud->gauche->usine->identifiant) > 0) {
        noeud->gauche = rotation_gauche(noeud->gauche);
        return rotation_droite(noeud);
    }

    // Cas 4 : Rotation droite-gauche
    if (equilibre < -1 && strcmp(identifiant, noeud->droite->usine->identifiant) < 0) {
        noeud->droite = rotation_droite(noeud->droite);
        return rotation_gauche(noeud);
    }

    return noeud;
}

// Parcours infixe inversé (ordre décroissant)
void avl_parcours_inverse(NoeudAVL *noeud, FILE *sortie, int mode) {
    if (noeud == NULL) {
        return;
    }

   
    avl_parcours_inverse(noeud->droite, sortie, mode);

    Usine *u = noeud->usine;
    
    // Conversion de k.m3 en M.m3 (division par 1000)
    if (mode == 0) {
        fprintf(sortie, "%s;%.6f\n", u->identifiant, u->volume_max / 1000.0);
    } else if (mode == 1) {
        fprintf(sortie, "%s;%.6f\n", u->identifiant, u->volume_source / 1000.0);
    } else {
        fprintf(sortie, "%s;%.6f\n", u->identifiant, u->volume_reel / 1000.0);
    }

    // Parcours du sous-arbre gauche
    avl_parcours_inverse(noeud->gauche, sortie, mode);
}


void avl_liberer(NoeudAVL *noeud) {
    if (noeud == NULL) {
        return;
    }
    
    avl_liberer(noeud->gauche);
    avl_liberer(noeud->droite);
    liberer_usine(noeud->usine);
    free(noeud);
}