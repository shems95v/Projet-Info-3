CC = GCC

SRC = main.c lireCSV.c gestion_usine.c calcul_histo.c

BIN = projet.sh

all: $(BIN)
$(BIN) = $(SRC)
$(CC) $(SRC) -o $(BIN)

clean:
rm-f $(BIN)
