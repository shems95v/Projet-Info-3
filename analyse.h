#ifndef ANALYSE_H
#define ANALYSE_H

//6 types de lignes possibles dans le fichier CSV

typedef enum {
    T_SRC_USINE,        // source → usine
    T_USINE_SEULE,      // ligne décrivant uniquement une usine
    T_USINE_STOCK,      // usine → stockage
    T_STOCK_JONCTION,   // stockage → jonction
    T_JONCTION_SERVICE, // jonction → raccordement
    T_SERVICE_USAGER,   // raccordement → usager
    T_INCONNU           // ligne invalide
} TypeTroncon;

// Structure représentant une ligne du fichier CSV

typedef struct {

    TypeTroncon type;

    char *id_usine;     // colonne 1 (ou "-")
    char *id_amont;     // colonne 2
    char *id_aval;      // colonne 3

    float volume;       // colonne 4 (ou "-")
    float fuite;        // colonne 5 (ou "-")
} Troncon;

// Fonction principale du module analyse
int analyseLigne(const char *line, Troncon *out);

// Libération des champs alloués dynamiquement
void freeTroncon(Troncon *t);

#endif
