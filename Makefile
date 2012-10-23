CC = mpicc
CFLAGS = -Wall
HOSTNPROC = -n 8 -hostfile hostfile

BIN = bin
INC = include
LIB = library
OBJ = object
SRC = src
MAIN = main
RUN = mpirun

all: $(OBJ)/$(MAIN).o $(OBJ)/function.o
	$(CC) $(CFLAGS) -o $(BIN)/$(MAIN) $^

$(OBJ)/$(MAIN).o: $(SRC)/$(MAIN).c $(INC)/function.h
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(OBJ)/function.o: $(SRC)/function.c $(INC)/function.h
	$(CC) $(CFLAGS) -c -o $@ $<

%: $(SRC)/%.c
	$(CC) $(CFLAGS) -o $(BIN)/$@ $^

clean:
	rm $(OBJ)/* $(BIN)/* 

exec:
	$(RUN) $(HOSTNPROC) $(BIN)/$(MAIN)

