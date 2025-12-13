# ==============================================================================
# SCRIPT GNUPLOT POUR L'HISTOGRAMME WILDWATER
# Ce script lit histo.dat (format CSV) et génère une image.
# ==============================================================================

# Variables passées par le script Shell (myScript.sh) :
# INPUT_FILE est 'histo.dat'
# OUTPUT_FILE est 'histo_max.png' (ou _src, _real)
# TITLE est le titre du graphique

# 1. Configuration de la sortie et du terminal (Type d'image)
# Le 'png' est couramment utilisé, mais vous pouvez le changer pour 'pdf'
set terminal pngcairo font "arial,10" size 1200, 700
set output OUTPUT_FILE

# 2. Configuration du graphique
set datafile separator ";"
# set datafile noheader
set title TITLE

# 3. Axes X (Labels)
set xtics rotate by -45 font "arial,7"

# 4. Axes Y (Valeurs)
set ylabel "Volume (M.m³ / an)" font "arial,10"
set yrange [0:*]                # Commence l'axe Y à 0 et auto-ajuste le max
set grid y                      # Ajoute des lignes de grille horizontales

# 5. Dessin du graphique (Plot)
# using 2:xticlabels(1) :
#   - 2 : utilise la colonne 2 (les volumes) pour la hauteur de la barre.
#   - xticlabels(1) : utilise la colonne 1 (les identifiants des usines) comme labels sous les barres.
plot INPUT_FILE using 2:xticlabels(1) title "" lc rgb "#006400"