#ifndef ARBRE_H
#define ARBRE_H

#include <stdio.h>

typedef struct noeud_arbre NoeudArbre;

// Structure pour une liste chaînée d'enfants
typedef struct liste_enfants {
    NoeudArbre *enfant;              // Pointeur vers le noeud enfant
    struct liste_enfants *suivant;   // Pointeur vers l'élément suivant de la liste
} ListeEnfants;

// Structure pour un noeud de l'arbre de distribution
struct noeud_arbre {
    char *identifiant;
    double volume_entrant;       
    double pourcentage_fuite;    
    int nombre_enfants;          
    ListeEnfants *enfants;       // Liste chaînée des enfants
};

// Structure pour un noeud de l'arbre AVL de recherche
typedef struct noeud_avl_arbre {
    char *identifiant;                      
    NoeudArbre *noeud_arbre;                // Pointeur vers le noeud de l'arbre de distribution
    int hauteur;                           
    struct noeud_avl_arbre *gauche;         
    struct noeud_avl_arbre *droite;        
} NoeudAVLArbre;

// Fonctions pour l'arbre de distribution 

NoeudArbre* creer_noeud_arbre(const char *identifiant);

void ajouter_enfant(NoeudArbre *parent, NoeudArbre *enfant, double fuite);

// Libérer la mémoire de l'arbre complet
void liberer_arbre(NoeudArbre *racine);

// Fonctions pour l'AVL de recherche

NoeudAVLArbre* arbre_avl_inserer(NoeudAVLArbre *noeud, const char *identifiant, NoeudArbre *noeud_arbre, NoeudAVLArbre **trouve);

// Rechercher un noeud dans l'arbre AVL par son identifiant
NoeudAVLArbre* arbre_avl_rechercher(NoeudAVLArbre *noeud, const char *identifiant);

void arbre_avl_liberer(NoeudAVLArbre *noeud);

// Fonctions de calcul

// Calculer le volume total de fuites dans l'arbre
double calculer_fuites_totales(NoeudArbre *racine);

// Construction de l'arbre

NoeudArbre* construire_arbre_distribution(const char *fichier, const char *identifiant_usine, NoeudAVLArbre **index_avl);

#endif