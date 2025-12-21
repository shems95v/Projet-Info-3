#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include "avl.h"
#include "tree.h"

int main(int argc, char **argv) {
    // Vérification du nombre d'arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <fichier> <commande> [argument]\n", argv[0]);
        fprintf(stderr, "  Commandes:\n");
        fprintf(stderr, "    histo <max|src|real|all>\n");
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
            fprintf(stderr, "Usage: %s <fichier> histo <max|src|real|all>\n", argv[0]);
            return 1;
        }

        // Détermination du mode selon l'argument
        int mode;
        int mode_all = 0; 
        
        if (strcmp(argv[3], "max") == 0) {
            mode = 0;
        } else if (strcmp(argv[3], "src") == 0) {
            mode = 1;
        } else if (strcmp(argv[3], "real") == 0) {
            mode = 2;
        } else if (strcmp(argv[3], "all") == 0) {
            mode_all = 1; // Activer le mode "all"
        } else {
            fprintf(stderr, "Erreur: mode doit être 'max', 'src', 'real' ou 'all'\n");
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
        if (mode_all == 1) {
            // En-tête pour le mode "all" 
            fprintf(sortie, "identifier;max volume (M.m3.year-1);source volume (M.m3.year-1);real volume (M.m3.year-1)\n");
            // Parcours avec la fonction spéciale pour "all"
            avl_parcours_inverse_all(racine, sortie);
        } else {
            // En-têtes pour les modes simples
            if (mode == 0) {
                fprintf(sortie, "identifier;max volume (M.m3.year-1)\n");
            } else if (mode == 1) {
                fprintf(sortie, "identifier;source volume (M.m3.year-1)\n");
            } else {
                fprintf(sortie, "identifier;real volume (M.m3.year-1)\n");
            }
        
            avl_parcours_inverse(racine, sortie, mode);
        }
        
    
        fclose(sortie);

        avl_liberer(racine);
        return 0;
    }
    
    //  Commande leaks 
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
        PireTroncon pire_troncon;
        
        // Vérification si l'usine a été trouvée
        if (racine == NULL) {
            // Usine non trouvée
            fuites_totales = -1.0;
            pire_troncon.identifiant_amont = NULL;
            pire_troncon.identifiant_aval = NULL;
            pire_troncon.volume_fuite = -1.0;
        } else {
            // Calcul des fuites totales et identification du pire tronçon
            fuites_totales = calculer_fuites_avec_pire_troncon(racine, &pire_troncon);
            // Conversion en M.m3
            fuites_totales = fuites_totales / 1000.0;
        }
        
        
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
            fprintf(sortie, "identifiant;volume fuites (M.m3.an-1);troncon pire amont;troncon pire aval;volume fuite pire troncon (M.m3.an-1)\n");
        }
        
        // Écriture du résultat avec les informations du pire tronçon 
        if (pire_troncon.identifiant_amont != NULL && pire_troncon.identifiant_aval != NULL) {
            fprintf(sortie, "%s;%.6f;%s;%s;%.6f\n", 
                    identifiant_usine, 
                    fuites_totales, 
                    pire_troncon.identifiant_amont, 
                    pire_troncon.identifiant_aval, 
                    pire_troncon.volume_fuite);
        } else {
            
            fprintf(sortie, "%s;%.6f;-;-;-\n", identifiant_usine, fuites_totales);
        }
        
        fclose(sortie);
        
     
        printf("%.6f\n", fuites_totales);
        
        // Affichage du pire tronçon 
        if (pire_troncon.identifiant_amont != NULL && pire_troncon.identifiant_aval != NULL) {
            printf("Pire tronçon: %s -> %s (%.6f M.m3/an)\n", 
                   pire_troncon.identifiant_amont, 
                   pire_troncon.identifiant_aval, 
                   pire_troncon.volume_fuite);
        }
        
        // Libération de la mémoire du pire tronçon
        if (pire_troncon.identifiant_amont != NULL) {
            free(pire_troncon.identifiant_amont);
        }
        if (pire_troncon.identifiant_aval != NULL) {
            free(pire_troncon.identifiant_aval);
        }
        
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