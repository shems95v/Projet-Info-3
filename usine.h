#ifndef USINE_H
#define USINE_H

// Structure représentant une usine de traitement d'eau
typedef struct {
    char *identifiant;      // Identifiant de l'usine
    double volume_max;      // Capacité maximale (k.m3/an)
    double volume_source;   // Volume capté depuis les sources (k.m3/an)
    double volume_reel;     // Volume réellement traité (k.m3/an)
} Usine;

Usine* creer_usine(const char *identifiant);

void liberer_usine(Usine *u);

#endif