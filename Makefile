CC = gcc

SRC = main.c gestion_usine.c calcul_histo.c analyse.c

BIN = projet_wildwater

all: $(BIN)

$(BIN) : $(SRC)  
  $(CC) $(SRC) -o $(BIN)
clean:
  rm -f $(BIN)
