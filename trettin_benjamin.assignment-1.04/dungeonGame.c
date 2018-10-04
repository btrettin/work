#include "dungeonGame.h"
#include <stdio.h>
#include "map.c"
#include <string.h>
#include "heap.h"
#include <stdlib.h>
#include <string.h>
/*Switches*/
int load=0;
int save=0;
int numberOfCreatures=0;
int main (int argc, char* argv[]){
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
        eval=strcmp(argv[x],"--nummon");
        if(eval==0){
            char* numberOfCritters = argv[x+1];
            numberOfCreatures = atoi(numberOfCritters);
            if(numberOfCreatures>=1680){
                printf("What are you even doing bro");
                return 1;
            }else{

            }
        }
    }
    initGame();
    StartGame();
    closeGame();
    return 0;
}
void initGame(void){
    if(load){
        loadGame();
    }else{
      init(numberOfCreatures);
    }
}
void StartGame(void){
    playGame();
}
void closeGame(void){
    if(save){
        saveGame();
    }
}
