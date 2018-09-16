All:
	gcc -Wall -g -O -c map.c
	gcc -Wall -g -O -c heap.c
	ar -cvq heap.a heap.o
	ar -cvq map.a map.o
	gcc -Wall -g dungeonGame.c -o run map.a -lm

clean:
	rm -f dungeonGame
