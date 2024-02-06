CC=gcc
INC=./inc/
SRC=./src/

CFLAGS=-g -Wall

#SRC=$(foreach D,$(SRC), $(wildcard $(D)/*.c))
OBJDIR=obj
SRCDIR=src
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
BIN=bin/main


all:$(BIN)

$(BIN):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

$(OBJ):$(SRC) 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj/* bin/*
