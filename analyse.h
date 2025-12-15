#ifndef ANALYSE_H
#define ANALYSE_H

typedef struct {
    char id[64];              // Identifiant de l'usine
    long capacite_max;        // Capacité maximale 
    long volume_source;       // Volume total capté depuis les sources
    long volume_reel;         // Volume réel traité 
} Usine;

// Arbre AVL pour stocker les usines
typedef struct Arbre {
    Usine u;               // Données de l'usine
    int hauteur;              // Hauteur du noeud (AVL)
    struct Arbre *fg;      // Fils gauche
    struct Arbre *fd;      // Fils droit
} Arbre;

// Fonction qui lit une ligne CSV et remplit une structure Usine
int analyseLigne(const char *line, Usine *out);

// Ajout d'une usine dans l'arbre binaire (ordre alphabétique)
Arbre* insererUsine(Arbre *root, Usine u);

// Affiche l'arbre (infixe)
void afficherArbre(Arbre *root);

// Libère toute la mémoire
void freeArbre(Arbre *root);

#endif
