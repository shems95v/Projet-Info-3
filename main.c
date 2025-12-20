#include <stdio.h>
#include <string.h>
#include "csv_reader.h"
#include "avl.h"
#include "tree.h"

int main(int argc, char **argv) {
    // Vérification du nombre d'arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <fichier> <commande> [argument]\n", argv[0]);
        fprintf(stderr, "  Commandes:\n");
        fprintf(stderr, "    histo <max|src|real>\n");
        fprintf(stderr, "    leaks <identifiant_usine>\n");
        return 1;
    }

    // Récupération des arguments
    const char *fichier = argv[1];
    const char *commande = argv[2];

    //Commande histo
    if (strcmp(commande, "histo") == 0) {
        // Vérification du nombre d'arguments pour histo
        if (argc != 4) {
            fprintf(stderr, "Usage: %s <fichier> histo <max|src|real>\n", argv[0]);
            return 1;
        }

        // Détermination du mode selon l'argument
        int mode;
        if (strcmp(argv[3], "max") == 0) {
            mode = 0;
        } else if (strcmp(argv[3], "src") == 0) {
            mode = 1;
        } else if (strcmp(argv[3], "real") == 0) {
            mode = 2;
        } else {
            fprintf(stderr, "Erreur: mode doit être 'max', 'src' ou 'real'\n");
            return 2;
        }

        // Construction de l'arbre AVL à partir du fichier
        NoeudAVL *racine = NULL;
        traiter_fichier_histogramme(fichier, &racine);

        // Ouverture du fichier de sortie
        FILE *sortie = fopen("histo.dat", "w");
        if (sortie == NULL) {
            return 3;
        }

        // Écriture de l'en-tête selon le mode
        if (mode == 0) {
            fprintf(sortie, "identifier;max volume (M.m3.year-1)\n");
        } else if (mode == 1) {
            fprintf(sortie, "identifier;source volume (M.m3.year-1)\n");
        } else {
            fprintf(sortie, "identifier;real volume (M.m3.year-1)\n");
        }
        
        // Parcours de l'arbre et écriture des données
        avl_parcours_inverse(racine, sortie, mode);
        
    
        fclose(sortie);

        avl_liberer(racine);
        return 0;
    }
    
    //  Commande Leaks 
    else if (strcmp(commande, "leaks") == 0) {
        // Vérification du nombre d'arguments pour leaks
        if (argc != 4) {
            fprintf(stderr, "Usage: %s <fichier> leaks <identifiant_usine>\n", argv[0]);
            return 1;
        }

        const char *identifiant_usine = argv[3];
        
        // Construction de l'arbre de distribution
        NoeudAVLArbre *index_avl = NULL;
        NoeudArbre *racine = construire_arbre_distribution(fichier, identifiant_usine, &index_avl);
        
        double fuites_totales;
        
        // Vérification si l'usine a été trouvée
        if (racine == NULL) {
            // Usine non trouvée
            fuites_totales = -1.0;
        } else {
            // Calcul des fuites totales (en k.m3)
            fuites_totales = calculer_fuites_totales(racine);
            // Conversion en M.m3
            fuites_totales = fuites_totales / 1000.0;
        }
        
        // Ouverture du fichier de sortie en mode ajout
        FILE *sortie = fopen("leaks.dat", "a");
        if (sortie == NULL) {
            if (racine != NULL) {
                liberer_arbre(racine);
            }
            if (index_avl != NULL) {
                arbre_avl_liberer(index_avl);
            }
            return 3;
        }
        
        // Si le fichier est vide, ajouter l'en-tête
        fseek(sortie, 0, SEEK_END);
        long taille = ftell(sortie);
        if (taille == 0) {
            fprintf(sortie, "identifier;Leak volume (M.m3.year-1)\n");
        }
        
    
        fprintf(sortie, "%s;%.6f\n", identifiant_usine, fuites_totales);
        fclose(sortie);
        
        // Affichage du résultat sur stdout pour le script shell
        printf("%.6f\n", fuites_totales);
        
        if (racine != NULL) {
            liberer_arbre(racine);
        }
        if (index_avl != NULL) {
            arbre_avl_liberer(index_avl);
        }
        
        // Retour d'erreur si l'usine n'a pas été trouvée
        if (fuites_totales < 0) {
            return 4;
        } else {
            return 0;
        }
    }
    
    // Commande inconnue
    else {
        fprintf(stderr, "Erreur: commande inconnue '%s'\n", commande);
        fprintf(stderr, "Commandes disponibles: histo, leaks\n");
        return 2;
    }
}