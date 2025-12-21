#include <stdlib.h>
#include <string.h>
#include "usine.h"

// Fonction pour créer une nouvelle usine
Usine* creer_usine(const char *identifiant) {
    Usine *u = malloc(sizeof(Usine));
    if (u == NULL) {
        return NULL;
    }

    // Allocation de la mémoire pour copier l'identifiant de l'usine
    u->identifiant = malloc(strlen(identifiant) + 1);
    if (u->identifiant == NULL) {
        // Échec d'allocation mémoire
        free(u);
        return NULL;
    }
    
    // Copie de la chaîne d'identifiant 
    strcpy(u->identifiant, identifiant);
    
    // Initialisation des volumes max/src puis réel
    u->volume_max = 0.0;
    u->volume_source = 0.0;
    u->volume_reel = 0.0;

    return u;
}

// Fonction pour libérer la mémoire d'une usine
void liberer_usine(Usine *u) {
    if (u == NULL) {
        return;
    }
    
    // Libération de la mémoire pour l'identifiant puis la structure de l'usine
    free(u->identifiant);
    free(u);
}
