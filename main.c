/* main.c : test de analyse.c sur le fichier c-wildwater_v0(2).dat
   Compilation :
     gcc main.c analyse.c -o test_wild
   Usage :
     ./test_wild c-wildwater_v0(2).dat
   Si aucun argument fourni, le programme tente d'ouvrir "c-wildwater_v0(2).dat" dans le dossier courant.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyse.h"

int main(int argc, char *argv[])
{
    const char *filename = "c-wildwater_v0.dat";
    if (argc >= 2) filename = argv[1];

    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return 1;
    }

    char ligne[2048];
    Usine u;
    Arbre *racine = NULL;
    long compte_usines = 0;
    long lignes_tot = 0;

    while (fgets(ligne, sizeof(ligne), f)) {
        lignes_tot++;
        int r = analyseLigne(ligne, &u);
        if (r == 0) {
            /* c'est une ligne usine, insertion dans l'arbre */
            racine = insererUsine(racine, u);
            compte_usines++;
        } else if (r == 2) {
            fprintf(stderr, "Avertissement : ligne mal formée à la ligne %ld\n", lignes_tot);
        }
        /* r == 3 -> ligne non-usine : on ignore */
    }

    fclose(f);

    printf("\n--- Résumé ---\n");
    printf("Fichier : %s\n", filename);
    printf("Lignes lues : %ld\n", lignes_tot);
    printf("Usines extraites : %ld\n\n", compte_usines);

    printf("--- Liste des usines (tri alphabétique) ---\n");
    afficherArbre(racine);

    freeArbre(racine);
    return 0;
}

