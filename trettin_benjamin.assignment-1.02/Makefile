All:
	gcc -Wall -g -O -c map.c
	ar -cvq map.a map.o
	gcc -Wall -g dungeonGame.c -o run map.a -lm

clean:
	rm -f dungeonGame
