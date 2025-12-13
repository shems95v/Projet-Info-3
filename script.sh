#!/bin/bash

START=$(date +%s)

if [$# -ne 3 ]; then
echo "Erreur : Pas assez darguments"
exit 1
fi

DATAFILE="$1"  # chemin du fichier de donnees 
CHOIX="$2"     # histo ou leaks
OPTION="$3"    # max/src/real ou id usine


 #Verification de lexistence du fichier
 
 if [! -f "$DATAFILE" ] then
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
if [ -z "$OPTION" ]; then
echo "Erreur : id dusine manquant"
exit 1
fi

else
echo "Erreur : commande inconnue"
exit 1
fi





END=$(date +%s)
DURATION=$(( (END - START) * 1000 ))
echo "Durée totale : ${DURATION} ms"

