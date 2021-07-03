CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -g -D_DEFAULT_SOURCE -fsanitize=address
LDFLAGS= -fsanitize=address
DEBUG=-g

OBJS=src/main.o src/board.o src/snake.o src/score.o src/keyboard.o

all : $(OBJS)
	gcc $(LDFLAGS) $(OBJS) -o snake

clean :
	$(RM) $(OBJS) snake

.PHONY: clean
