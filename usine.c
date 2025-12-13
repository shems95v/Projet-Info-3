#include <stdlib.h>
#include <string.h>
#include "usine.h"

Usine* creer_usine(const char *id) {
    Usine *u = malloc(sizeof(Usine));
    if (!u) return NULL;

    u->id = strdup(id);
    u->vol_max = 0.0;
    u->vol_src = 0.0;
    u->vol_real = 0.0;

    return u;
}

void liberer_usine(Usine *u) {
    if (!u) return;
    free(u->id);
    free(u);
}
