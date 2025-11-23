CC = gcc
CFLAGS = -I../utils -Wall -Wextra -O2

SRC_DIR = .
LIB_DIR = ../utils

SRC = $(SRC_DIR)/main.c
LIB_SRC = $(LIB_DIR)/socketUtils.c

LIB_OBJ = $(LIB_DIR)/socketUtils.o
LIB_STATIC = $(LIB_DIR)/libutils.a

TARGET = server

all: $(TARGET)

$(TARGET): $(SRC) $(LIB_STATIC)
	$(CC) $(SRC) -L$(LIB_DIR) -lutils -I$(LIB_DIR) -o $(TARGET)

$(LIB_STATIC): $(LIB_OBJ)
	ar rcs $(LIB_STATIC) $(LIB_OBJ)

$(LIB_OBJ): $(LIB_SRC)
	$(CC) -c $(LIB_SRC) -o $(LIB_OBJ)

clean:
	rm -f $(TARGET) $(LIB_OBJ) $(LIB_STATIC)

.PHONY: all clean
