CC = gcc

all:learn

learn: learn.c
	gcc -g -Wall -Werror -fsanitize=address learn.c -o learn

clean:
	rm -rf learn
