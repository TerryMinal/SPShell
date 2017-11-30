all: main2.c
	gcc -g -o main main2.c

run: all
	./main
