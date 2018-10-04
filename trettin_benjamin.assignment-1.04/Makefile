All:
	gcc -Wall -g -O -c map.c
		gcc -Wall -g -O -c heap.c
		gcc -Wall -g -O -c monster.c
		ar -cvq heap.a heap.o
		ar -cvq gameMap.a map.o monster.o
		gcc -static -Wall -O0 -ggdb dungeonGame.c -o run gameMap.a heap.a -lm

clean:
	rm -f dungeonGame
