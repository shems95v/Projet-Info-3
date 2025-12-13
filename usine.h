#ifndef USINE_H
#define USINE_H

typedef struct {
    char *id;          // Identifiant de l'usine
    double vol_max;    // Capacité max (k.m3/an)
    double vol_src;    // Volume capté (k.m3/an)
    double vol_real;   // Volume réellement traité
} Usine;

Usine* creer_usine(const char *id);
void liberer_usine(Usine *u);

#endif
