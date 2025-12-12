#!/bin/bash

START=$(date +%s)

if [$# -lt 2 ]; then
echo "Erreur : Pas assez d'arguments"
exit (1)
fi
 
arg1="$1"   (nom fichier CSV/DATA) 
arg2="$2"
arg3="$3"

 #Verification de lexistence du fichier
 
 if [! -f "$arg1" ] then
 echo " Fichier $arg1 introuvable "
 exit (1)
 fi
 

 

