CC = gcc
CFLAGS = -std=c17 -Wall -Wfatal-errors -I$(INCLUDE_DIR)
SRC_DIR = src
INCLUDE_DIR = include
TARGET = codec

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/traitementPGM.c $(SRC_DIR)/quadtree.c $(SRC_DIR)/compress.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f $(OBJS) $(TARGET)
