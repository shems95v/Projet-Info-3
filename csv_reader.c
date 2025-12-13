#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"

#define MAX_LINE 1024

void traiter_fichier_histogramme(const char *fichier, AVLNode **racine) {
    FILE *fp = fopen(fichier, "r");
    if (!fp) {
        perror("Erreur ouverture fichier");
        exit(1);
    }

    char ligne[MAX_LINE];

    while (fgets(ligne, sizeof(ligne), fp)) {
        char *c1 = strtok(ligne, ";");
        char *c2 = strtok(NULL, ";");
        char *c3 = strtok(NULL, ";");
        char *c4 = strtok(NULL, ";");
        char *c5 = strtok(NULL, ";\n");

        if (!c2 || strcmp(c2, "-") == 0)
            continue;

        AVLNode *node = NULL;
        *racine = avl_insert(*racine, c2, &node);
        Usine *u = node->usine;

        /* Ligne usine */
        if (c3 && strcmp(c3, "-") == 0 && c4 && strcmp(c4, "-") != 0)
            u->vol_max = atof(c4);

        /* Ligne source -> usine */
        if (c3 && strstr(c3, "Facility") && c4 && strcmp(c4, "-") != 0) {
            double vol = atof(c4);
            double fuite = c5 ? atof(c5) / 100.0 : 0.0;

            u->vol_src += vol;
            u->vol_real += vol * (1.0 - fuite);
        }
    }

    fclose(fp);
}
