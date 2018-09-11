#include <stdio.h>
#include "map.c"
#include <string.h>

int load=0;
int save=0;

void initGame(void){
    if(load){
        loadGame();
    }else{
      generateMap();
    }
}

void playGame(void){
    printMap();
}

void closeGame(void){
    if(save){
        saveGame();
    }
}

int main (int argc, char* argv[]){
    srand(time(0));
    int x;
    int eval;
    for(x=1;x<argc;x++){
        eval=strcmp(argv[x],"--save");
        if(eval==0){
            save=1;
        }
        eval=strcmp(argv[x],"--load");
        if(eval==0){
            load=1;
        }
        }
    initGame();
    playGame();
    closeGame();
    return 0;
}
