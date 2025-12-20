#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"

#define TAILLE_MAX_LIGNE 1024

// Fonction pour traiter le fichier CSV et construire l'histogramme
void traiter_fichier_histogramme(const char *fichier, NoeudAVL **racine) {
    // Ouverture du fichier en lecture
    FILE *fp = fopen(fichier, "r");
    if (fp == NULL) {
        perror("Erreur ouverture fichier");
        exit(1);
    }

    char ligne[TAILLE_MAX_LIGNE];

    // Lecture ligne par ligne
    while (fgets(ligne, sizeof(ligne), fp)) {
        // Découpage de la ligne en colonnes
        (void)strtok(ligne, ";");   // Pour ignorer la première colonne du CSV
        char *colonne2 = strtok(NULL, ";");
        char *colonne3 = strtok(NULL, ";");
        char *colonne4 = strtok(NULL, ";");
        char *colonne5 = strtok(NULL, ";\n");

        // Si la colonne 2 est vide ou contient "-", on passe à la ligne suivante
        if (colonne2 == NULL || strcmp(colonne2, "-") == 0) {
            continue;
        }

        // Cas 1 : Ligne usine (format: -;Usine;-;capacité;-)
        if (colonne3 != NULL && strcmp(colonne3, "-") == 0 && colonne4 != NULL && strcmp(colonne4, "-") != 0) {
            // Insertion de l'usine dans l'arbre AVL
            NoeudAVL *noeud = NULL;
            *racine = avl_inserer(*racine, colonne2, &noeud);
            
            // Récupération de l'usine et mise à jour de sa capacité
            Usine *u = noeud->usine;
            u->volume_max = atof(colonne4);
        }

        // Cas 2 : Ligne source vers usine (format: -;Source;Usine;volume;fuite)
        // On ajoute le volume à l'usine (colonne3), pas à la source (colonne2)
        if (colonne3 != NULL && strcmp(colonne3, "-") != 0 && colonne4 != NULL && strcmp(colonne4, "-") != 0) {
            // Conversion du volume et du taux de fuite
            double volume = atof(colonne4);
            double fuite = 0.0;
            if (colonne5 != NULL) {
                fuite = atof(colonne5) / 100.0;
            }

            // Insertion ou récupération de l'usine dans l'arbre
            NoeudAVL *noeud_usine = NULL;
            *racine = avl_inserer(*racine, colonne3, &noeud_usine);
            Usine *usine = noeud_usine->usine;

            // Mise à jour des volumes de l'usine
            usine->volume_source += volume;
            usine->volume_reel += volume * (1.0 - fuite);
        }
    }
    fclose(fp);
}