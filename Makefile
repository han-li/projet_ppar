CC = mpicc
CC2 = gcc
CFLAGS = -Wall -fopenmp
HOSTNPROC = -n 12 -hostfile hostfile

BIN = bin
INC = include
LIB = library
OBJ = object
SRC = src
MAIN = main
RUN = mpirun
TEST = main2

all: $(OBJ)/$(MAIN).o $(OBJ)/function.o 
	$(CC) $(CFLAGS) -o $(BIN)/$(MAIN) $^

$(OBJ)/$(MAIN).o: $(SRC)/$(MAIN).c $(INC)/function.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(OBJ)/function.o: $(SRC)/function.c $(INC)/function.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(OBJ)/* $(BIN)/* 

exec:
	$(RUN) $(HOSTNPROC) $(BIN)/$(MAIN)

test:
	$(CC2) $(CFLAGS) -o $(BIN)/$(TEST) $(SRC)/function.c $(SRC)/$(TEST).c -pthread -lm
	$(BIN)/$(TEST)

%:
	$(RUN) $(HOSTNPROC) $(BIN)/$(MAIN) $@

