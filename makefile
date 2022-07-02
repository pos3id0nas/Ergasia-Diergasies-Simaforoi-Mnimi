#This is the makefile
#Constants Metavlites
#PHGH : https://www.youtube.com/watch?v=yA8uXpZL-QM
CC=gcc
CFLAGS=-Wall -g
OBJ = ergasia2

all: Pm Pc

Pm: Pm.c
	$(CC) $(CFLAGS) -o $(OBJ) Pm.c

Pc: Pc.c
	$(CC) $(CFLAGS) -o Pc Pc.c -lm

#make clean
clean:
	rm -rf $(OBJ) Pc
