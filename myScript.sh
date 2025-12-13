#!/bin/bash

# ==============================================================================
# SCRIPT SHELL POUR PROJET WILDWATER (Compilation C, Tri, GnuPlot)
# ==============================================================================

START_TIME=$(date +%s)

# 1. Vérification des arguments d'entrée
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <fichier_csv_donnees> <mode_histo>"
    echo "Modes disponibles: max, src, real"
    exit 1
fi

FICHIER_CSV="$1"
MODE_HISTO="$2"

# 2. Vérification de l'existence de l'exécutable
if [ ! -f ./histo ]; then
    echo "Erreur: L'exécutable 'histo' n'existe pas. Veuillez lancer 'make' d'abord."
    exit 1
fi

# 3. Vérification de l'existence du fichier GnuPlot
if [ ! -f ./histo.gp ]; then
    echo "Erreur: Le script GnuPlot 'histo.gp' est manquant."
    exit 1
fi

# -------------------------------------------------------------
# 4. Préparation et Exécution du programme C
# -------------------------------------------------------------

# Définition du nom du fichier de données de sortie (selon l'énoncé)
case $MODE_HISTO in
    "max")
        DATA_FILE="vol_max.dat"
        TITLE="Histogramme par Capacité Maximale"
        ;;
    "src")
        DATA_FILE="vol_captation.dat"
        TITLE="Histogramme par Volume Capté (Source)"
        ;;
    "real")
        DATA_FILE="vol_traitement.dat"
        TITLE="Histogramme par Volume Réel Traité"
        ;;
    *)
        echo "Erreur: Mode non reconnu. Utilisez max, src, ou real."
        exit 1
        ;;
esac

echo "Phase 1/4: Exécution du programme C..."

# Correction de l'erreur Code 2 : Nettoyer la variable du mode 
# (supprime les retours à la ligne ou espaces qui causent un strcmp incorrect en C)
MODE_CLEANED=$(echo "$MODE_HISTO" | tr -d '\r\n[:space:]')

# Appel du programme C
# NOTE: Le programme C doit écrire sa sortie dans un fichier nommé "histo.dat"
./histo "$FICHIER_CSV" histo "$MODE_CLEANED"

CODE_RETOUR=$?
if [ $CODE_RETOUR -ne 0 ]; then
    echo "--- ÉCHEC DU PROGRAMME C avec code $CODE_RETOUR ---"
    echo "Vérifiez que le 3ème argument dans main.c est bien max, src, ou real (minuscules)."
    exit $CODE_RETOUR
fi
echo "Programme C terminé avec succès. Fichier 'histo.dat' généré."

# Renommer la sortie pour respecter l'énoncé
mv histo.dat "$DATA_FILE"
if [ $? -ne 0 ]; then
    echo "Erreur lors du renommage de histo.dat en $DATA_FILE."
    exit 5
fi

# -------------------------------------------------------------
# 5. Préparation des données pour l'extraction Top 10 / Bottom 50
# -------------------------------------------------------------
echo "Phase 2/4: Tri des données pour extraction Top 10 / Bottom 50."

# Fichiers temporaires pour les extraits
TEMP_SORTED_FILE="sorted_by_value.dat"
TOP10_FILE="temp_top10.dat"
BOTTOM50_FILE="temp_bottom50.dat"

# Force la locale C pour s'assurer que sort interprète le point '.' comme séparateur décimal
LC_ALL=C grep -v identifier "$DATA_FILE" | sort -t ';' -k 2,2nr > sorted_by_value.dat

# Extraction des 10 plus grandes (Top 10)
# 'head -n 10' prend les 10 premières lignes du fichier trié
head -n 10 "$TEMP_SORTED_FILE" > "$TOP10_FILE"
echo "Extraction Top 10 usines réussie."

# Extraction des 50 plus petites (Bottom 50)
# 'tail -n 50' prend les 50 dernières lignes du fichier trié
tail -n 50 "$TEMP_SORTED_FILE" > "$BOTTOM50_FILE"
echo "Extraction Bottom 50 usines réussie."

# -------------------------------------------------------------
# 6. Appel de GnuPlot (Génération des deux images)
# -------------------------------------------------------------
echo "Phase 3/4: Génération des images PNG (Top 10 et Bottom 50)..."

# A. GÉNÉRATION DE L'IMAGE DES 10 PLUS GRANDES
OUTPUT_FILE_1="top10_${MODE_CLEANED}.png"
# Appel de GnuPlot avec les variables en environnement
gnuplot -e "INPUT_FILE='${TOP10_FILE}'; OUTPUT_FILE='${OUTPUT_FILE_1}'; TITLE='${TITLE} (Top 10)'" histo.gp
if [ $? -ne 0 ]; then echo "Erreur GnuPlot Top 10." ; exit 4; fi
echo "Image Top 10 générée : ${OUTPUT_FILE_1}"

# B. GÉNÉRATION DE L'IMAGE DES 50 PLUS PETITES
OUTPUT_FILE_2="bottom50_${MODE_CLEANED}.png"
gnuplot -e "INPUT_FILE='${BOTTOM50_FILE}'; OUTPUT_FILE='${OUTPUT_FILE_2}'; TITLE='${TITLE} (Bottom 50)'" histo.gp
if [ $? -ne 0 ]; then echo "Erreur GnuPlot Bottom 50." ; exit 4; fi
echo "Image Bottom 50 générée : ${OUTPUT_FILE_2}"

# -------------------------------------------------------------
# 7. Nettoyage et Fin
# -------------------------------------------------------------
echo "Phase 4/4: Nettoyage des fichiers temporaires."

# Suppression des fichiers temporaires de tri
rm "$TEMP_SORTED_FILE" "$TOP10_FILE" "$BOTTOM50_FILE"

echo "----------------------------------------"
END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))
echo "Traitement terminé avec succès."
echo "Durée totale de l'exécution : ${DURATION} secondes."
exit 0