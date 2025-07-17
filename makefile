CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Iinclude

SRC = src
OBJ = obj
BIN = scanner

OBJS = $(SRC)/scanner.o \
        $(SRC)/banner.o \
        $(SRC)/parallel_scan.o \
        $(SRC)/port_utils.o \
        $(SRC)/services.o

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

clean:
	rm -f $(SRC)/*.o $(BIN)
