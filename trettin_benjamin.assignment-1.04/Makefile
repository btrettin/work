All:
		gcc -Wall -g -O -c map.c
		gcc -Wall -g -O -c heap.c
		gcc -Wall -g -O -c monster.c
		ar -cvq heap.a heap.o
		ar -cvq map.a map.o
		ar -cvq monster.a monster.o
		ar -cvq gameMap.a
		gcc -Wall -g dungeonGame.c -o run gameMap.a map.a heap.a -lm

clean:
		rm -f dungeonGame
