CC = gcc
CFLAGS = -std=c17 -Wall -Wfatal-errors
SRCS = main.c traitementPGM.c
TARGET = test
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f $(OBJS) $(TARGET)