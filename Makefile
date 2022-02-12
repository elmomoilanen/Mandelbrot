CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -g -O3

SRC=src/bitmap.c src/main.c src/mandelbrot.c src/palette.c src/argparser.c
OBJS=bitmap.o main.o mandelbrot.o palette.o argparser.o
TARGET=mandelbrot

.PHONY:all clean

all: $(TARGET)

$(OBJS): $(SRC)
	$(CC) $(CFLAGS) -c -Isrc/ $(SRC)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lm -lpthread

clean:
	rm -f $(OBJS)
