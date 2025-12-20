# Variables de configuration
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
EXECUTABLE = histo
SOURCES = main.c usine.c csv_reader.c avl.c tree.c
OBJETS = $(SOURCES:.c=.o)

# Cibles fictives (n'existent pas en tant que fichiers)
.PHONY: all clean

# 1. Cible par défaut : construit tout le projet
all: $(EXECUTABLE)

# 2. Règle pour l'exécutable

$(EXECUTABLE): $(OBJETS)
	$(CC) $(CFLAGS) $(OBJETS) -o $(EXECUTABLE)

# 3. Règle implicite pour la compilation des fichiers objets (.o)

main.o: csv_reader.h avl.h tree.h
csv_reader.o: csv_reader.h avl.h usine.h
usine.o: usine.h
avl.o: avl.h usine.h
tree.o: tree.h

# 4. Règle pour nettoyer le répertoire
clean:
	rm -f $(OBJETS) $(EXECUTABLE) histo.dat