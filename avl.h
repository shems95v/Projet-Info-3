#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include "usine.h"

typedef struct noeud_avl {
    Usine *usine;              // Pointeur vers l'usine
    int hauteur;               
    struct noeud_avl *gauche;  
    struct noeud_avl *droite;  
} NoeudAVL;

NoeudAVL* avl_inserer(NoeudAVL *noeud, const char *identifiant, NoeudAVL **noeud_trouve);


void avl_parcours_inverse(NoeudAVL *noeud, FILE *sortie, int mode);


void avl_parcours_inverse_all(NoeudAVL *noeud, FILE *sortie);


void avl_liberer(NoeudAVL *noeud);

#endif