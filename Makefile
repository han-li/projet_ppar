CC = gcc
CFLAGS = -Wall

BIN = bin
INC = include
LIB = library
OBJ = object
SRC = src
MAIN = main

all: $(OBJ)/main.o $(OBJ)/function.o
	$(CC) $(CFLAGS) -o $(BIN)/$(MAIN) $^

$(OBJ)/main.o: $(SRC)/main.c $(INC)/function.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(OBJ)/function.o: $(SRC)/function.c $(INC)/function.h
	$(CC) $(CFLAGS) -c -o $@ $<

%: $(SRC)/%.c
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^

clean:
	rm $(OBJ)/* $(BIN)/$(MAIN)

exec:
	$(BIN)/$(MAIN)

