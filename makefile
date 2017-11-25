all: main.c
	gcc -g -o main main.c

run: all
		./main
