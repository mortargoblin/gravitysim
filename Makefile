CC = cc
CFLAGS = $(shell pkg-config --cflags raylib) -Wall -Wextra
LDFLAGS = $(shell pkg-config --libs raylib) -lm

gravity: main.c
	$(CC) main.c -o gravity $(CFLAGS) $(LDFLAGS)
