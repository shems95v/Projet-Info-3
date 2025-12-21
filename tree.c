#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

#define TAILLE_MAX_LIGNE 1024


int maximum_entier(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Fonction pour obtenir la hauteur d'un noeud AVL
int arbre_avl_hauteur(NoeudAVLArbre *n) {
    if (n == NULL) {
        return 0;
    }
    return n->hauteur;
}

// Fonction pour calculer le facteur d'équilibre
int arbre_avl_equilibre(NoeudAVLArbre *n) {
    if (n == NULL) {
        return 0;
    }
    return arbre_avl_hauteur(n->gauche) - arbre_avl_hauteur(n->droite);
}

// Rotation à droite
NoeudAVLArbre* arbre_avl_rotation_droite(NoeudAVLArbre *y) {
    NoeudAVLArbre *x = y->gauche;
    NoeudAVLArbre *t2 = x->droite;
    
   
    x->droite = y;
    y->gauche = t2;
    
    // Mise à jour des hauteurs
    y->hauteur = maximum_entier(arbre_avl_hauteur(y->gauche), arbre_avl_hauteur(y->droite)) + 1;
    x->hauteur = maximum_entier(arbre_avl_hauteur(x->gauche), arbre_avl_hauteur(x->droite)) + 1;
    
    return x;
}

// Rotation à gauche
NoeudAVLArbre* arbre_avl_rotation_gauche(NoeudAVLArbre *x) {
    NoeudAVLArbre *y = x->droite;
    NoeudAVLArbre *t2 = y->gauche;
    
  
    y->gauche = x;
    x->droite = t2;
    
    // Mise à jour des hauteurs
    x->hauteur = maximum_entier(arbre_avl_hauteur(x->gauche), arbre_avl_hauteur(x->droite)) + 1;
    y->hauteur = maximum_entier(arbre_avl_hauteur(y->gauche), arbre_avl_hauteur(y->droite)) + 1;
    
    return y;
}

// Fonctions pour l'arbre de distribution


NoeudArbre* creer_noeud_arbre(const char *identifiant) {
    NoeudArbre *noeud = malloc(sizeof(NoeudArbre));
    if (noeud == NULL) {
        return NULL;
    }
    
    noeud->identifiant = malloc(strlen(identifiant) + 1);
    if (noeud->identifiant == NULL) {
        free(noeud);
        return NULL;
    }
    strcpy(noeud->identifiant, identifiant);
    noeud->volume_entrant = 0.0;
    noeud->pourcentage_fuite = 0.0;
    noeud->nombre_enfants = 0;
    noeud->enfants = NULL;
    
    return noeud;
}

// Ajouter un enfant à un noeud parent
void ajouter_enfant(NoeudArbre *parent, NoeudArbre *enfant, double fuite) {
    ListeEnfants *nouvel_enfant = malloc(sizeof(ListeEnfants));
    if (nouvel_enfant == NULL) {
        return;
    }
    
    nouvel_enfant->enfant = enfant;
    nouvel_enfant->suivant = parent->enfants;
    parent->enfants = nouvel_enfant;
    parent->nombre_enfants++;
    
    enfant->pourcentage_fuite = fuite;
}

// Libérer la mémoire de l'arbre de distribution
void liberer_arbre(NoeudArbre *racine) {
    if (racine == NULL) {
        return;
    }
    
    // Libération récursive des enfants
    ListeEnfants *temp = racine->enfants;
    while (temp != NULL) {
        ListeEnfants *suivant = temp->suivant;
        liberer_arbre(temp->enfant);
        free(temp);
        temp = suivant;
    }
    
    free(racine->identifiant);
    free(racine);
}

// Insertion dans l'arbre AVL de recherche
NoeudAVLArbre* arbre_avl_inserer(NoeudAVLArbre *noeud, const char *identifiant, NoeudArbre *noeud_arbre, NoeudAVLArbre **noeud_trouve) {
    // Cas de base : arbre vide
    if (noeud == NULL) {
        NoeudAVLArbre *nouveau_noeud = malloc(sizeof(NoeudAVLArbre));
        nouveau_noeud->identifiant = malloc(strlen(identifiant) + 1);
        if (nouveau_noeud->identifiant == NULL) {
            free(nouveau_noeud);
            return NULL;
        }
        strcpy(nouveau_noeud->identifiant, identifiant);
        nouveau_noeud->noeud_arbre = noeud_arbre;
        nouveau_noeud->gauche = NULL;
        nouveau_noeud->droite = NULL;
        nouveau_noeud->hauteur = 1;
        *noeud_trouve = nouveau_noeud;
        return nouveau_noeud;
    }
    
    // Comparaison des identifiants
    int comparaison = strcmp(identifiant, noeud->identifiant);
    
    // Si l'identifiant existe déjà
    if (comparaison == 0) {
        *noeud_trouve = noeud;
        return noeud;
    }

    else if (comparaison < 0) {
        noeud->gauche = arbre_avl_inserer(noeud->gauche, identifiant, noeud_arbre, noeud_trouve);
    }
    else {
        noeud->droite = arbre_avl_inserer(noeud->droite, identifiant, noeud_arbre, noeud_trouve);
    }
    
    // Mise à jour de la hauteur
    noeud->hauteur = 1 + maximum_entier(arbre_avl_hauteur(noeud->gauche), arbre_avl_hauteur(noeud->droite));
    
    // Calcul du facteur d'équilibre
    int equilibre = arbre_avl_equilibre(noeud);
    
    // Cas 1 : Rotation droite simple
    if (equilibre > 1 && strcmp(identifiant, noeud->gauche->identifiant) < 0) {
        return arbre_avl_rotation_droite(noeud);
    }
    
    // Cas 2 : Rotation gauche simple
    if (equilibre < -1 && strcmp(identifiant, noeud->droite->identifiant) > 0) {
        return arbre_avl_rotation_gauche(noeud);
    }
    
    // Cas 3 : Rotation gauche-droite
    if (equilibre > 1 && strcmp(identifiant, noeud->gauche->identifiant) > 0) {
        noeud->gauche = arbre_avl_rotation_gauche(noeud->gauche);
        return arbre_avl_rotation_droite(noeud);
    }
    
    // Cas 4 : Rotation droite-gauche
    if (equilibre < -1 && strcmp(identifiant, noeud->droite->identifiant) < 0) {
        noeud->droite = arbre_avl_rotation_droite(noeud->droite);
        return arbre_avl_rotation_gauche(noeud);
    }
    
    return noeud;
}

// Recherche dans l'arbre AVL
NoeudAVLArbre* arbre_avl_rechercher(NoeudAVLArbre *noeud, const char *identifiant) {
    if (noeud == NULL) {
        return NULL;
    }
    
    int comparaison = strcmp(identifiant, noeud->identifiant);
    
    if (comparaison == 0) {
        return noeud;
    } else if (comparaison < 0) {
        return arbre_avl_rechercher(noeud->gauche, identifiant);
    } else {
        return arbre_avl_rechercher(noeud->droite, identifiant);
    }
}

void arbre_avl_liberer(NoeudAVLArbre *noeud) {
    if (noeud == NULL) {
        return;
    }
    
    arbre_avl_liberer(noeud->gauche);
    arbre_avl_liberer(noeud->droite);
    free(noeud->identifiant);
    free(noeud);
}

// Calcul des fuites

// Propager le volume de la racine vers les enfants
void propager_volume(NoeudArbre *noeud) {
    if (noeud == NULL || noeud->nombre_enfants == 0) {
        return;
    }
    
    // Volume réparti équitablement entre les enfants
    double volume_par_enfant = noeud->volume_entrant / noeud->nombre_enfants;
    
    // Propagation du volume à chaque enfant
    ListeEnfants *temp = noeud->enfants;
    while (temp != NULL) {
        temp->enfant->volume_entrant = volume_par_enfant;
        propager_volume(temp->enfant);
        temp = temp->suivant;
    }
}

// Calcul récursif des fuites
double calculer_fuites_recursif(NoeudArbre *noeud) {
    if (noeud == NULL) {
        return 0;
    }
    
    // Fuite sur ce tronçon = volume_entrant × pourcentage_fuite
    double fuite_locale = noeud->volume_entrant * (noeud->pourcentage_fuite / 100.0);
    
    // Somme des fuites des enfants
    double fuites_enfants = 0;
    ListeEnfants *temp = noeud->enfants;
    while (temp != NULL) {
        fuites_enfants += calculer_fuites_recursif(temp->enfant);
        temp = temp->suivant;
    }
    
    return fuite_locale + fuites_enfants;
}

// Calcul des fuites totales de l'arbre
double calculer_fuites_totales(NoeudArbre *racine) {
    if (racine == NULL) {
        return -1;
    }
    
    propager_volume(racine);
    
    return calculer_fuites_recursif(racine);
}

// Construction de l'arbre depuis le fichier CSV

NoeudArbre* construire_arbre_distribution(const char *fichier, const char *identifiant_usine, NoeudAVLArbre **index_avl) {

    FILE *fp = fopen(fichier, "r");
    if (fp == NULL) {
        perror("Erreur ouverture fichier");
        return NULL;
    }
    
    char ligne[TAILLE_MAX_LIGNE];
    NoeudArbre *racine = NULL;
    *index_avl = NULL;
    
    // chercher l'usine et créer la racine
    while (fgets(ligne, sizeof(ligne), fp)) {
        char copie_ligne[TAILLE_MAX_LIGNE];
        strncpy(copie_ligne, ligne, TAILLE_MAX_LIGNE - 1);
        copie_ligne[TAILLE_MAX_LIGNE - 1] = '\0';
        
        // Découpage de la ligne
        (void)strtok(copie_ligne, ";");
        char *colonne2 = strtok(NULL, ";");
        char *colonne3 = strtok(NULL, ";");
        char *colonne4 = strtok(NULL, ";");
        
        if (colonne2 == NULL || colonne3 == NULL || colonne4 == NULL) {
            continue;
        }
        
        // Trouver la ligne de l'usine: -;USINE;-;CAPACITE;-
        if (strcmp(colonne3, "-") == 0 && strcmp(colonne4, "-") != 0 && strcmp(colonne2, identifiant_usine) == 0) {
            racine = creer_noeud_arbre(identifiant_usine);
            racine->volume_entrant = atof(colonne4); // Capacité de l'usine
            
            NoeudAVLArbre *trouve = NULL;
            *index_avl = arbre_avl_inserer(*index_avl, identifiant_usine, racine, &trouve);
            break;
        }
    }
    
    // Si l'usine n'a pas été trouvée
    if (racine == NULL) {
        fclose(fp);
        return NULL;
    }
    
    // construire l'arbre complet
    rewind(fp); // Revenir au début du fichier 
    
    while (fgets(ligne, sizeof(ligne), fp)) {
        char copie_ligne[TAILLE_MAX_LIGNE];
        strncpy(copie_ligne, ligne, TAILLE_MAX_LIGNE - 1);
        copie_ligne[TAILLE_MAX_LIGNE - 1] = '\0';
        
        // Découpage de la ligne
        char *colonne1 = strtok(copie_ligne, ";");
        char *colonne2 = strtok(NULL, ";");
        char *colonne3 = strtok(NULL, ";");
        (void)strtok(NULL, ";");
        char *colonne5 = strtok(NULL, ";\n");
        
        if (colonne1 == NULL || colonne2 == NULL || colonne3 == NULL) {
            continue;
        }
        
        // Traiter les lignes qui concernent notre usine (identifiant en colonne1 ou colonne2)
        if (strcmp(colonne1, identifiant_usine) == 0 || (strcmp(colonne1, "-") == 0 && strcmp(colonne2, identifiant_usine) == 0)) {
            
            if (strcmp(colonne3, "-") == 0) {
                continue; // Ignorer les lignes USINE
            }
            
            // Récupération du taux de fuite
            double fuite = 0;
            if (colonne5 != NULL) {
                fuite = atof(colonne5);
            }
            
            // Chercher le nœud parent dans l'AVL
            NoeudAVLArbre *parent_avl = arbre_avl_rechercher(*index_avl, colonne2);
            
            if (parent_avl != NULL) {
                // Créer ou récupérer l'enfant
                NoeudAVLArbre *enfant_avl = arbre_avl_rechercher(*index_avl, colonne3);
                NoeudArbre *noeud_enfant;
                
                if (enfant_avl == NULL) {
                    // Créer un nouveau nœud enfant
                    noeud_enfant = creer_noeud_arbre(colonne3);
                    NoeudAVLArbre *trouve = NULL;
                    *index_avl = arbre_avl_inserer(*index_avl, colonne3, noeud_enfant, &trouve);
                } else {
                    // Récupérer le nœud existant
                    noeud_enfant = enfant_avl->noeud_arbre;
                }
                
                // Ajouter l'enfant au parent
                ajouter_enfant(parent_avl->noeud_arbre, noeud_enfant, fuite);
            }
        }
    }
    
    fclose(fp);
    return racine;
}
