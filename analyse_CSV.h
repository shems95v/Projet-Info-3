#ifndef ANALYSE_H
#define ANALYSE_H

typedef struct {
    char nom[100];      // Nom de l'usine
    long volume_reel;   // Volume réel traité
} Usine;

typedef struct Arbre {
    Usine u;               // Données de l'usine
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
