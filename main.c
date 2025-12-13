#include <stdio.h>
#include <string.h>
#include "csv_reader.h"
#include "avl.h"
#include <locale.h>  // 1. Ajoutez l'inclusion

int main(int argc, char **argv) {
    setlocale(LC_NUMERIC, "C");
    if (argc != 4 || strcmp(argv[2], "histo") != 0) {
        fprintf(stderr, "Usage: %s <fichier> histo <max|src|real>\n", argv[0]);
        return 1;
    }

    int mode;
    if (strcmp(argv[3], "max") == 0) mode = 0;
    else if (strcmp(argv[3], "src") == 0) mode = 1;
    else if (strcmp(argv[3], "real") == 0) mode = 2;
    else return 2;

    AVLNode *racine = NULL;
    traiter_fichier_histogramme(argv[1], &racine);

    FILE *out = fopen("histo.dat", "w");
    if (!out) return 3;

    fprintf(out, "identifier;value(M.m3)\n");
    avl_inorder_reverse(racine, out, mode);
    fclose(out);

    avl_free(racine);
    return 0;
}
