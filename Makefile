# Variables de configuration
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = histo
SRCS = main.c usine.c csv_reader.c avl.c
OBJS = $(SRCS:.c=.o)

# Cibles fictives (n'existent pas en tant que fichiers)
.PHONY: all clean

# 1. Cible par défaut : construit tout le projet
all: $(TARGET)

# 2. Règle pour l'exécutable
# Dépend de tous les fichiers objets (.o)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# 3. Règle implicite pour la compilation des fichiers objets (.o)
# Utilise la règle implicite de make pour compiler un .c en .o
# Indique seulement les dépendances nécessaires aux en-têtes (headers)
main.o: csv_reader.h avl.h
csv_reader.o: csv_reader.h avl.h usine.h
usine.o: usine.h
avl.o: avl.h usine.h

# 4. Règle pour nettoyer le répertoire
clean:
	rm -f $(OBJS) $(TARGET) histo.dat