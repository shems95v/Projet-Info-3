#!/bin/bash

START=$(date +%s)

#Verification du nombre darguments

if [$# -ne 3 ]; then
 echo "Erreur : nombre darguments incorrect"
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

#Calcul de la duree totale en millisecondes

END=$(date +%s)
DURATION=$(( (END - START) * 1000 ))
echo "Duree totale : ${DURATION} ms"

