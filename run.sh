#!/bin/bash


#Récupération des arguments
DATA_FILE=$1
COMMAND=$2
EXECUTABLE="./histo"
MAKEFILE="Makefile"

# Fonction pour afficher le temps écoulé
afficher_temps() {
    DUREE=$((SECONDS * 1000))
    echo "Durée totale du script: ${DUREE} ms"
}

# Vérification des arguments 
if [ "$#" -lt 3 ]; then
    echo "Erreur: Usage: $0 <fichier_donnees> <commande> [argument]" >&2
    echo "  Commandes:" >&2
    echo "    histo <max|src|real>" >&2
    echo "    leaks <identifiant_usine>" >&2
    afficher_temps
    exit 1
fi

# Compilation du programme C si nécessaire
if [ ! -f "$EXECUTABLE" ]; then
    echo "L'exécutable '$EXECUTABLE' n'existe pas. Compilation en cours..."
    if ! make -f "$MAKEFILE" all; then
        echo "Erreur: Échec de la compilation du programme C." >&2
        afficher_temps
        exit 1
    fi
fi

#Traitement selon la commande

if [ "$COMMAND" = "histo" ]; then

    # Commande histo
    
    MODE=$3
    
    # Vérification du nombre d'arguments
    if [ "$#" -ne 3 ]; then
        echo "Erreur: Usage: $0 <fichier_donnees> histo <max|src|real>" >&2
        afficher_temps
        exit 1
    fi

    # Vérification du mode
    if [ "$MODE" != "max" ] && [ "$MODE" != "src" ] && [ "$MODE" != "real" ]; then
        echo "Erreur: Le mode doit être 'max', 'src' ou 'real'." >&2
        afficher_temps
        exit 1
    fi

    # Conversion du mode en majuscule
    UPPER_MODE=$(echo "$MODE" | tr '[:lower:]' '[:upper:]')
    
    # Définition du nom de fichier de sortie
    OUTPUT_FILE="${MODE}_volume.dat"
    TITLE_REF=""

    # Définition du titre selon le mode
    if [ "$MODE" = "max" ]; then
        TITLE_REF="Capacité Maximale de Traitement"
    elif [ "$MODE" = "src" ]; then
        TITLE_REF="Volume Total Capté depuis les Sources"
    elif [ "$MODE" = "real" ]; then
        TITLE_REF="Volume Total Réellement Traité"
    fi

    # Exécution du programme C pour générer histo.dat
    echo "Exécution du programme C pour générer histo.dat..."
    if ! "$EXECUTABLE" "$DATA_FILE" histo "$MODE"; then
        echo "Erreur: Le programme C a retourné un code d'erreur." >&2
        afficher_temps
        exit 3
    fi

    # Renommage du fichier généré
    mv histo.dat "$OUTPUT_FILE"
    echo "Fichier de données généré: $OUTPUT_FILE"

    # Génération du fichier de référence max si nécessaire
    if [ "$MODE" != "max" ]; then
        echo "Génération du fichier de référence max_volume.dat pour le tri..."
        "$EXECUTABLE" "$DATA_FILE" histo max
        mv histo.dat max_volume.dat
    fi

    # Préparation des fichiers pour les 10 plus grandes et 50 plus petites usines
    head -n 1 "$OUTPUT_FILE" > "${MODE}_10_plus_grandes_usines.dat"
    head -n 1 "$OUTPUT_FILE" > "${MODE}_50_plus_petites usines.dat"

    # Tri et filtrage des données
    if [ "$MODE" = "max" ]; then
        # Pour le mode max, tri direct par volume
        tail -n +2 "$OUTPUT_FILE" | sort -t ';' -k 2,2gr | head -n 10 >> "${MODE}_10_plus_grandes_usines.dat"
        tail -n +2 "$OUTPUT_FILE" | sort -t ';' -k 2,2g | head -n 50 >> "${MODE}_50_plus_petites usines.dat"
    else
        # Pour src et real, tri basé sur la capacité max
        tail -n +2 "$OUTPUT_FILE" | sort -t ';' -k 1,1 > temp_mode.txt
        tail -n +2 max_volume.dat | sort -t ';' -k 1,1 > temp_max.txt
        join -t ';' -1 1 -2 1 temp_mode.txt temp_max.txt > temp_joined.txt
        sort -t ';' -k 3,3gr temp_joined.txt | head -n 10 | cut -d ';' -f 1,2 >> "${MODE}_10_plus_grandes_usines.dat"
        sort -t ';' -k 3,3g temp_joined.txt | head -n 50 | cut -d ';' -f 1,2 >> "${MODE}_50_plus_petites usines.dat"
        rm -f temp_mode.txt temp_max.txt temp_joined.txt
    fi

    echo "Fichiers filtrés générés: ${MODE}_10_plus_grandes_usines.dat et ${MODE}_50_plus_petites usines.dat"

    # Fonction pour générer un script Gnuplot
    generate_gnuplot_script() {
        local DATA_IN=$1
        local PNG_OUT=$2
        local CHART_TITLE=$3
        local X_LABEL="Usine"
        local Y_LABEL="Volume (M.m³)"

        cat << EOF > "${PNG_OUT}.gp"
set terminal pngcairo size 1400,900 font "Arial,10"
set output '$PNG_OUT'
set title "$CHART_TITLE" font "Arial,14"
set xlabel "$X_LABEL" font "Arial,12"
set ylabel "$Y_LABEL" font "Arial,12"
set style data histogram
set style histogram clustered gap 1
set style fill solid 0.8 border -1
set boxwidth 0.9 relative
set xtics rotate by -45 scale 0 font "Arial,8"
set grid ytics
set datafile separator ";"
set key off
set yrange [0:*]

plot '$DATA_IN' using 2:xtic(1) with boxes linecolor rgb "#4472C4"
EOF
    }

    # Génération du graphique pour les 10 plus grandes usines
    TITLE_10="Les 10 Plus Grandes Usines (basé sur: Capacité Maximale) - ${TITLE_REF}"
    PNG_10="${MODE}_10_plus_grandes_usines.png"
    generate_gnuplot_script "${MODE}_10_plus_grandes_usines.dat" "$PNG_10" "$TITLE_10"
    if gnuplot "${PNG_10}.gp" 2>/dev/null; then
        echo "Image générée: $PNG_10"
    else
        echo "Avertissement: Échec de la génération de ${PNG_10}. Vérifiez que gnuplot est installé."
    fi

    # Génération du graphique pour les 50 plus petites usines
    TITLE_50="Les 50 Plus Petites Usines (basé sur: Capacité Maximale) - ${TITLE_REF}"
    PNG_50="${MODE}_50_plus_petites usines.png"
    generate_gnuplot_script "${MODE}_50_plus_petites usines.dat" "$PNG_50" "$TITLE_50"
    if gnuplot "${PNG_50}.gp" 2>/dev/null; then
        echo "Image générée: $PNG_50"
    else
        echo "Avertissement: Échec de la génération de ${PNG_50}. Vérifiez que gnuplot est installé."
    fi

elif [ "$COMMAND" = "leaks" ]; then
   
    #Commande leaks
    
    USINE_ID=$3
    
    # Vérification du nombre d'arguments
    if [ "$#" -ne 3 ]; then
        echo "Erreur: Usage: $0 <fichier_donnees> leaks <identifiant_usine>" >&2
        afficher_temps
        exit 1
    fi
    
    echo "Calcul des fuites pour l'usine: $USINE_ID"
    
    # Exécution du programme C
    RESULT=$("$EXECUTABLE" "$DATA_FILE" leaks "$USINE_ID")
    EXIT_CODE=$?
    
    # Vérification du code de retour
    if [ $EXIT_CODE -ne 0 ] && [ $EXIT_CODE -ne 4 ]; then
        echo "Erreur: Le programme C a retourné un code d'erreur $EXIT_CODE." >&2
        afficher_temps
        exit 3
    fi
    
    # Affichage du résultat
    if [ "$RESULT" = "-1.000000" ] || [ $EXIT_CODE -eq 4 ]; then
        echo "Avertissement: L'usine '$USINE_ID' n'a pas été trouvée dans le fichier."
        echo "Volume de fuites: -1.000000 M.m³/an (usine non trouvée)"
    else
        echo "Volume total de fuites: $RESULT M.m³/an"
    fi
    
    echo "Résultat enregistré dans: leaks.dat"

else
    # Commande inconnue
    echo "Erreur: Commande inconnue '$COMMAND'" >&2
    echo "Commandes disponibles: histo, leaks" >&2
    afficher_temps
    exit 2
fi

# Calcul et affichage de la durée totale
echo "Traitement terminé avec succès."
afficher_temps
