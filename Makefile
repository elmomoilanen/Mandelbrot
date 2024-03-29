CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -O3

SRC=src/bitmap.c src/main.c src/mandelbrot.c src/palette.c src/argparser.c
OBJS=bitmap.o main.o mandelbrot.o palette.o argparser.o
TARGET=mandelbrot

.PHONY: all clean help

all: $(TARGET) clean

$(OBJS): $(SRC)
	$(CC) $(CFLAGS) -c -Isrc/ $(SRC)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lm -lpthread

clean:
	rm -f $(OBJS)

help:
	@echo "Available targets:\n"
	@echo "all:       Build executable and run clean afterwards"
	@echo "clean:     Remove object files"
	@echo "help:      Display this help message"
