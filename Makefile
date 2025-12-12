CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC = main.c analyse.c analyse_CSV.c
OBJ = $(SRC:.c=.o)
EXEC = wildwater

all: $(EXEC)

$(EXEC): $(OBJ)  
  $(CC) $(CFLAGS) -o $(EXEC) $(OBJ)
%.o: %.c
  $(CC) $(CFLAGS) -c $< -o $@
clean:
  rm -f $(OBJ) $(EXEC)
