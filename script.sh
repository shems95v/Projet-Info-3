#!/bin/bash

START=$(date +%s)

#Verification du nombre darguments

if [ $# -ne 3 ]; then
 echo "Erreur : nombre darguments incorrect"
 exit 1
fi

DATAFILE="$1"  # chemin du fichier de donnees
CHOIX="$2"     # histo ou leaks
OPTION="$3"    # max/src/real ou id usine


 #Verification de lexistence du fichier
 
 if [ ! -f "$DATAFILE" ]; then
  echo " Erreur : fichier '$DATAFILE' introuvable "
  exit 1
 fi

#Verification des separateurs points virgule

 if ! grep -q ";" "$DATAFILE"; then
  echo "Erreur : fichier invalide (pas de separateur ';')"
  exit 1
 fi

#Verification des commandes
 
if [ "$CHOIX" = "histo" ]; then
 if [ "$OPTION" != "max" ] && [ "$OPTION" != "real" ] && [ "$OPTION" != "src" ]; then
  echo "Erreur : option histo incorrecte"
  exit 1
 fi

elif [ "$CHOIX" = "leaks" ]; then  
 if [ -z "$OPTION" ]; then              #Verification de lidentifiant de lusine pour leaks  
  echo "Erreur : id dusine manquant"
  exit 1
fi

else
 echo "Erreur : commande inconnue"
 exit 1
fi

#Compilation du programme

if [ ! -f "wildwater" ]; then  
make
 if [ $? -ne 0 ]; then
 echo "Erreur de compilation"
 exit 1
 fi
fi


#Execution du programme C

./wildwater "$DATAFILE" "$CHOIX" "$OPTION"
RET=$?

if [ $RET -ne 0 ]; then
 echo "Erreur: Le programme C a echoue"
 exit 1
fi

# Vérification que histo.dat existe
if [ ! -f "histo.dat" ]; then
    echo "Erreur : fichier histo.dat non généré"
    exit 1
fi

# Tri


SORTED="sorted_by_value.dat"
TOP10="top10.dat"
BOTTOM50="bottom50.dat"

# Tri decroissant par valeur
LC_ALL=C sort -t ';' -k 2,2nr histo.dat > "$SORTED"

# Top 10 plus grandes usines
head -n 10 "$SORTED" > "$TOP10"

# 50 plus petites usines
tail -n 50 "$SORTED" | sort -t ';' -k 2,2n > "$BOTTOM50"


#Calcul de la duree totale en millisecondes

END=$(date +%s)
DURATION=$(( (END - START) * 1000 ))
echo "Duree totale : ${DURATION} ms"


# Generation du graphique si mode histo
if [ "$CHOIX" = "histo" ]; then
  
        gnuplot -e "INPUT_FILE='$TOP10'; OUTPUT_FILE='top10_${OPTION}.png'; TITLE='Top 10 usines (${OPTION})'" histo.gp
        gnuplot -e "INPUT_FILE='$BOTTOM50'; OUTPUT_FILE='bottom50_${OPTION}.png'; TITLE='Bottom 50 usines (${OPTION})'" histo.gp

    if [ $? -ne 0 ]; then
        echo "Erreur lors de la generation du graphique"
        exit 1
    fi
    echo "Graphique genere : histo.png"
fi
