#include "gameMap.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

static void performWander(Monster *npc);
static void moveNearestNonTunneling(Monster *npc);
void initList(struct list);
void addToList(struct list,int num);
int removeFromList(struct list);
static void getDirections(Monster *npc);
void reset(struct list);
static void getDirectionsTunneling(Monster *npc);
static void readDirections(Monster *mon);
static void moveNearestTunneling(Monster *npc);
/*Public Class Monster */
void initList(struct list dir){
    dir.size=0;
}
void addToList(struct list dir,int num){
    dir.directions[dir.size]=num;
    dir.size++;
}
int removeFromList(struct list dir){
    int num = dir.directions[0];
    int x;
    for(x=1;x<dir.size;x++){
        dir.directions[x-1]=dir.directions[x];
    }
    dir.size--;
    return num;
}
void reset(struct list dir){
    dir.size=0;
}


/*Fields for Library*/
static int maxMonsters;
//static Map *m;
Monster* monsterArray[21][80] = {{NULL}};
static int numOfMonsters;
int pcx;
int pcy;

/*Library Static Funcitons*/
void initMonsterLib(Map *map, int numOfMax){
    srand(time(NULL));
    maxMonsters = numOfMax;
    //m=map;

}
int getPCX(){
    return pcx;
}
int getPCY(){
    return pcy;
}
/*Constructor*/
Monster* MonsterInit(Map *map,int x,int y,int isPlayer){
    Monster *monster;
    monster = malloc(sizeof(Monster));
    if(isPlayer){
        monster->thePlayer=1;
        monster->speed=10;
        monster->roundVal=10;
        monster->xloc=x;
        monster->yloc=y;
        pcx = x;
        pcy = y;
        monster->alive=1;
    }else{
    initList(monster->directions);
    monster->thePlayer=0;
    monster->bigPeople=0;

    monster->dragon=0;
    monster->other=0;
    monster->patrolMode=1;
    monster->alive=1;
    monster->characteristics = rand()%16;
    monster->moveUp=moveUp;
    monster->moveDown=moveDown;
    monster->moveRight =moveRight;
    monster->moveLeft = moveLeft;
    monster->moveTopRight = moveTopRight;
    monster->moveTopLeft = moveTopLeft;
    monster->moveBottomLeft = moveBottomLeft;
    monster->moveBottomRight = moveBottomRight;
    monster->modelNumber=numOfMonsters;
    monster->isIntelegent = isIntelegent;
    monster->isTelapathic = isTelapathic;
    monster->canTunnle = canTunnle;
    monster->isErratic = isErratic;
    monster->performAction=performAction;
    monster->deconstructor = deconstructor;
    monster->scanArea=scanArea;
    monster->yloc=y;
    monster->searchLocationY=y;
    monster->searchLocationX=x;
    monster->xloc=x;
    int typeSwitch = rand()%3;
    switch(typeSwitch){
        case 0:
            monster->bigPeople=1;
            break;
        case 1:
            monster->dragon=1;
            break;
        case 2:
            monster->other=1;
            break;
    }

    if(!isPlayer){
        int spee = (rand()%19)+1;
        spee = 100 / spee;

    if(spee<5){
        spee=spee+5;
    }
    monster->roundVal=spee;
    monster->speed= spee;
    }

    numOfMonsters++;
    }
monsterArray[monster->yloc][monster->xloc]=monster;
return monster;
}
/*Deconstructor*/
void deconstructor(Monster *m){
    free(m);
}
/*Public Functions For Monsters*/

 int moveUp(Monster *mon){/*y-1,x*/
    Monster *temp;
    int ytemp = (*mon).yloc;
    if(ytemp==0){
        return 1;
    }
    int xtemp = (*mon).xloc;
    monsterArray[ytemp][xtemp]=NULL;/*y,x*/


    temp = monsterArray[ytemp-1][xtemp];
    if(temp!=NULL){
        (*temp).alive=0;
    }
    if(m->mapArray[ytemp-1][xtemp]==('#') || m->mapArray[ytemp-1][xtemp]==('.')){
       monsterArray[ytemp-1][xtemp] = mon;
       (*mon).yloc=ytemp-1;
    }else{
        unsigned char hardness =m->hardnessArray[ytemp-1][xtemp];
        if(hardness>85){
            hardness=hardness-85;
        }else{
            hardness=0;
        }
        m->hardnessArray[ytemp-1][xtemp]=hardness;
        if(hardness==0){
            monsterArray[ytemp-1][xtemp] = mon;
            (*mon).yloc=ytemp-1;
            if(m->mapArray[ytemp-1][xtemp]!='.'){
            m->mapArray[ytemp-1][xtemp]='#';
            }
        }
    }

    return 0;
}
 int moveDown(Monster *mon){/*y+1,x*/
    Monster *temp;
    int ytemp = (*mon).yloc;
    if(ytemp==19){
        return 1;
    }
    int xtemp = (*mon).xloc;
    monsterArray[ytemp][xtemp]=NULL;/*y,x*/


    temp = monsterArray[ytemp+1][xtemp];/*y+1,x*/
    if(temp!=NULL){
        (*temp).alive=0;
    }
    if(m->mapArray[ytemp+1][xtemp]==('#') || m->mapArray[ytemp+1][xtemp]==('.')){/*y+1,x*/
       monsterArray[ytemp+1][xtemp] = mon;/*y+1,x*/
       (*mon).yloc=ytemp+1; /*y+1,x*/
    }else{
        unsigned char hardness =m->hardnessArray[ytemp+1][xtemp];
        if(hardness>85){
            hardness=hardness-85;
        }else{
            hardness=0;
        }
        m->hardnessArray[ytemp+1][xtemp]=hardness;/*y+1,x*/
        if(hardness==0){
            monsterArray[ytemp+1][xtemp] = mon;/*y+1,x*/
            (*mon).yloc=ytemp+1;/*y+1,x*/
            if(m->mapArray[ytemp+1][xtemp]!='.'){
            m->mapArray[ytemp+1][xtemp]='#'; /*y+1,x*/
            }
        }
    }

    return 0;
}
 int moveRight(Monster *mon){/*y,x+1*/
    Monster *temp;
    int ytemp = (*mon).yloc;

    int xtemp = (*mon).xloc;
    if(xtemp==78){
        return 1;
    }
    monsterArray[ytemp][xtemp]=NULL;/*y,x*/
    temp = monsterArray[ytemp][xtemp+1];/*y,x+1*/
    if(temp!=NULL){
        (*temp).alive=0;
    }
    if(m->mapArray[ytemp][xtemp+1]==('#') || m->mapArray[ytemp][xtemp+1]==('.')){/*y,x+1*/
       monsterArray[ytemp][xtemp+1] = mon;/*y,x+1*/
       (*mon).xloc=xtemp+1; /*y,x+1*/
    }else{
        unsigned char hardness =m->hardnessArray[ytemp][xtemp+1];
        if(hardness>85){
            hardness=hardness-85;
        }else{
            hardness=0;
        }
        m->hardnessArray[ytemp][xtemp+1]=hardness;/*y,x+1*/
        if(hardness==0){
            monsterArray[ytemp][xtemp+1] = mon;/*y,x+1*/
            (*mon).xloc=xtemp+1;/*y,x+1*/
            if(m->mapArray[ytemp][xtemp+1]!='.'){
            m->mapArray[ytemp][xtemp+1]='#'; /*y,x+1*/
            }
        }
    }

    return 0;
}
 int moveLeft(Monster *mon){/*y,x-1*/
    Monster *temp;
    int ytemp = (*mon).yloc;

    int xtemp = (*mon).xloc;
    if(xtemp==1){
        return 1;
    }
    monsterArray[ytemp][xtemp]=NULL;/*y,x*/


    temp = monsterArray[ytemp][xtemp-1];/*y,x-1*/
    if(temp!=NULL){
        (*temp).alive=0;
    }
    if(m->mapArray[ytemp][xtemp-1]==('#') || m->mapArray[ytemp][xtemp-1]==('.')){/*y,x-1*/
       monsterArray[ytemp][xtemp-1] = mon;/*y,x-1*/
       (*mon).xloc=xtemp-1; /*y,x-1*/
    }else{
        unsigned char hardness =m->hardnessArray[ytemp][xtemp-1];
        if(hardness>85){
            hardness=hardness-85;
        }else{
            hardness=0;
        }
        m->hardnessArray[ytemp][xtemp-1]=hardness;/*y,x-1*/
        if(hardness==0){
            monsterArray[ytemp][xtemp-1] = mon;/*y,x-1*/
            (*mon).xloc=xtemp-1;/*y,x-1*/
            if(m->mapArray[ytemp][xtemp-1]!='.'){
            m->mapArray[ytemp][xtemp-1]='#'; /*y,x-1*/
            }
        }
    }

    return 0;
}
 int moveTopRight(Monster *mon){/*y-1,x+1*/
    Monster *temp;
    int ytemp = (*mon).yloc;

    int xtemp = (*mon).xloc;
    if(ytemp==1 || xtemp==78){
        return 1;
    }
    monsterArray[ytemp][xtemp]=NULL;/*y,x*/


    temp = monsterArray[ytemp-1][xtemp+1];/*y-1,x+1*/
    if(temp!=NULL){
        (*temp).alive=0;
    }
    if(m->mapArray[ytemp-1][xtemp+1]==('#') || m->mapArray[ytemp-1][xtemp+1]==('.')){/*y-1,x+1*/
       monsterArray[ytemp-1][xtemp+1] = mon;/*y-1,x+1*/
       (*mon).yloc=ytemp-1;/*y-1,x+1*/
        (*mon).xloc=xtemp+1;
    }else{
        unsigned char hardness =m->hardnessArray[ytemp-1][xtemp+1];
        if(hardness>85){
            hardness=hardness-85;
        }else{
            hardness=0;
        }
        m->hardnessArray[ytemp-1][xtemp+1]=hardness;/*y-1,x+1*/
        if(hardness==0){
            monsterArray[ytemp-1][xtemp+1] = mon;/*y-1,x+1*/
            (*mon).yloc=ytemp-1;/*y-1,x+1*/
            (*mon).xloc=xtemp+1;
            if(m->mapArray[ytemp-1][xtemp+1]!='.'){
            m->mapArray[ytemp-1][xtemp+1]='#'; /*y-1,x+1*/
            }
        }
    }

    return 0;
}
 int moveTopLeft(Monster *mon){/*y-1,x-1*/
        Monster *temp;
    int ytemp = (*mon).yloc;

    int xtemp = (*mon).xloc;
    if(ytemp==1 || xtemp==1){
        return 1;
    }
    monsterArray[ytemp][xtemp]=NULL;/*y,x*/


    temp = monsterArray[ytemp-1][xtemp-1];/*y-1,x-1*/
    if(temp!=NULL){
        (*temp).alive=0;
    }
    if(m->mapArray[ytemp-1][xtemp-1]==('#') || m->mapArray[ytemp-1][xtemp-1]==('.') ){/*y-1,x-1*/
       monsterArray[ytemp-1][xtemp-1] = mon;/*y-1,x-1*/
       (*mon).yloc=ytemp-1;/*y-1,x-1*/
        (*mon).xloc=xtemp-1;
    }else{
        unsigned char hardness =m->hardnessArray[ytemp-1][xtemp-1];
        if(hardness>85){
            hardness=hardness-85;
        }else{
            hardness=0;
        }
        m->hardnessArray[ytemp-1][xtemp-1]=hardness;/*y-1,x-1*/
        if(hardness==0){
            monsterArray[ytemp-1][xtemp-1] = mon;/*y-1,x-1*/
            (*mon).yloc=ytemp-1;/*y-1,x-1*/
            (*mon).xloc=xtemp-1;
            if(m->mapArray[ytemp-1][xtemp-1]!='.'){
            m->mapArray[ytemp-1][xtemp-1]='#'; /*y-1,x-1*/
            }
        }
    }

    return 0;
}
 int moveBottomLeft(Monster *mon){/*y+1,x-1*/
        Monster *temp;
    int ytemp = (*mon).yloc;

    int xtemp = (*mon).xloc;
     if(ytemp==78 || xtemp==1){
        return 1;
    }
    monsterArray[ytemp][xtemp]=NULL;/*y,x*/


    temp = monsterArray[ytemp+1][xtemp-1];/*y+1,x-1*/
    if(temp!=NULL){
        (*temp).alive=0;
    }
    if(m->mapArray[ytemp+1][xtemp-1]==('#') || m->mapArray[ytemp+1][xtemp-1]==('.')) {/*y+1,x-1*/
       monsterArray[ytemp+1][xtemp-1] = mon;/*y+1,x-1*/
       (*mon).yloc=ytemp+1;/*y+1,x-1*/
        (*mon).xloc=xtemp-1;
    }else{
        unsigned char hardness =m->hardnessArray[ytemp+1][xtemp-1];
        if(hardness>85){
            hardness=hardness-85;
        }else{
            hardness=0;
        }
        m->hardnessArray[ytemp+1][xtemp-1]=hardness;/*y+1,x-1*/
        if(hardness==0){
            monsterArray[ytemp+1][xtemp-1] = mon;/*y+1,x-1*/
            (*mon).yloc=ytemp+1;/*y+1,x-1*/
            (*mon).xloc=xtemp-1;
            if(m->mapArray[ytemp][xtemp+1]!='.'){
            m->mapArray[ytemp][xtemp+1]='#'; /*y+1,x-1*/
            }
        }
    }

    return 0;
}
 int moveBottomRight(Monster *mon){/*y+1,x+1*/
        Monster *temp;
    int ytemp = (*mon).yloc;
    int xtemp = (*mon).xloc;
    if(ytemp==19 || xtemp==78){
        return 1;
    }

    monsterArray[ytemp][xtemp]=NULL;/*y,x*/


    temp = monsterArray[ytemp+1][xtemp+1];/*y+1,x+1*/
    if(temp!=NULL){
        (*temp).alive=0;
    }
    if(m->mapArray[ytemp+1][xtemp+1]==('#') || m->mapArray[ytemp-1][xtemp]==('.')){/*y+1,x+1*/
       monsterArray[ytemp+1][xtemp+1] = mon;/*y+1,x+1*/
       (*mon).yloc=ytemp+1;/*y+1,x+1*/
        (*mon).xloc=xtemp+1;
    }else{
        unsigned char hardness =m->hardnessArray[ytemp+1][xtemp+1];
        if(hardness>85){
            hardness=hardness-85;
        }else{
            hardness=0;
        }
        m->hardnessArray[ytemp+1][xtemp+1]=hardness;/*y+1,x+1*/
        if(hardness==0){
            monsterArray[ytemp+1][xtemp+1] = mon;/*y+1,x+1*/
            (*mon).yloc=ytemp+1;/*y+1,x+1*/
            (*mon).xloc=xtemp+1;
            if(m->mapArray[ytemp+1][xtemp+1]!='.'){
            m->mapArray[ytemp+1][xtemp+1]='#'; /*y+1,x+1*/
            }
        }
    }

    return 0;
}
 int isIntelegent(Monster *mon){
    int unsigned temp = mon->characteristics;
    return 1 & temp;

}
 int isTelapathic(Monster *mon){
    int unsigned temp = mon->characteristics;
    return (2 & temp);
}
 int canTunnle(Monster *mon){
    int unsigned temp = mon->characteristics;
    return 4 & temp;
}
 int isErratic(Monster *mon){
    int unsigned temp = mon->characteristics;
    return (8 & temp);
}

void performAction(Monster *mon){
    scanArea(mon);
    if(mon->thePlayer){
        performWander(mon);
        return;
    }
     int twoFacesCoin = rand()%2;
     if(isErratic(mon) && twoFacesCoin){
            performWander(mon);
            return;
        }

    if(isTelapathic(mon)){
        if(canTunnle(mon)){
            moveNearestTunneling(mon);
            return;
        }else{
            moveNearestNonTunneling(mon);
            return;
        }
    }
    if(mon->patrolMode){
        performWander(mon);
        return;
    }else{
            if(isIntelegent(mon)){
                if(!mon->patrolMode){
                    if(canTunnle(mon)){
                         reset(mon->directions);
                         getDirectionsTunneling(mon);
                        readDirections(mon);
                         return;
                    }else{
                        reset(mon->directions);
                        getDirections(mon);
                        readDirections(mon);
                        return;
                        }

                    }
                }
                int gottooX = mon->searchLocationX;
                int gottooY = mon->searchLocationY;
                int xhere = mon->xloc;
                int yhere = mon->yloc;
                float deltay = gottooY-yhere;
                float deltax = gottooX-xhere;
                if(deltax<0 && deltay<0){
                    moveTopLeft(mon);
                    return;
                }
                if(deltay<0 && (deltax<1 && deltax>-1) ){
                    moveUp(mon);
                    return;
                }
                if(deltay<0 && deltax>0){
                    moveTopRight(mon);
                    return;
                }
                if((deltay>-1 && deltay<1) && deltax<0 ){
                    moveRight(mon);
                    return;
                }
                if((deltay>-1 && deltay<1) && deltax>0){
                    moveRight(mon);
                    return;
                }
                if(deltay>0 && deltax<0){
                    moveBottomLeft(mon);
                    return;
                }
                if(deltay>0 && (deltax>-1 && deltax<1)){
                    moveDown(mon);
                    return;
                }
                if(deltay>0 && deltax>0){
                    moveBottomRight(mon);
                    return;
                }

            }
        }
/*Helper Functions For Monsters */
static void readDirections(Monster *mon){
     int switchValue = removeFromList(mon->directions);
                            switch(switchValue){
                                case 1:
                                    moveTopLeft(mon);
                                    break;
                                case 2:
                                    moveUp(mon);
                                    break;
                                case 3:
                                    moveTopRight(mon);
                                    break;
                                case 4:
                                    moveLeft(mon);
                                    break;
                                case 5:
                                    moveRight(mon);
                                    break;
                                case 6:
                                    moveBottomLeft(mon);
                                    break;
                                case 7:
                                    moveDown(mon);
                                    break;
                                case 8:
                                    moveBottomRight(mon);
                                    break;
                            }
}

static void getDirectionsTunneling(Monster *npc){
    int done=0;
    int xhere = npc->xloc;
    int yhere = npc->yloc;
    while(!done){
    unsigned volatile int min = 1000;
    volatile int topLeft=0;
    volatile int top=0;
    volatile int topRight=0;
    volatile int left=0;
    volatile int right =0;
    volatile int bottomLeft =0;
    volatile int bottom =0;
    volatile int bottomRight=0;
    /*Top Left*/
    if(xhere-1!=0 && yhere-1!=0){
            int temp = m->wallDistanceGrid[yhere-1][xhere-1].distance;
             if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                 topLeft=1;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }

    }
    /*Top*/
    if(yhere-1!=0){
            int temp = m->wallDistanceGrid[yhere-1][xhere].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                 topLeft=0;
                 top=1;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }

    }
    /*TopRight*/
    if(xhere+1!=79 && yhere-1!=0){
            int temp = m->wallDistanceGrid[yhere-1][xhere+1].distance;
              if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=1;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }


    }
    /*Left*/
    if(xhere-1!=0){if(yhere==0){
                done=1;
            }
            int temp = m->wallDistanceGrid[yhere][xhere-1].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=1;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*Right*/
    if(xhere+1!=79){
            int temp = m->wallDistanceGrid[yhere][xhere+1].distance;
            if(!temp){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =1;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }

    }
    /*BottomLeft*/
    if(xhere-1!=0 && yhere+1!=20){
            int temp = m->wallDistanceGrid[yhere+1][xhere-1].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =1;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*Bottom*/
    if(yhere+1!=79){
            int temp = m->wallDistanceGrid[yhere+1][xhere].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =1;
                 bottomRight=0;
            }
    }
    /*BottomRight*/
    if(xhere+1!=79 && yhere+1!=20){
            int temp = m->wallDistanceGrid[yhere+1][xhere+1].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=1;
            }
    }
    if(top){
        yhere--;
        addToList(npc->directions,2);
    }
    if(topLeft){
        yhere--;
        xhere--;
        addToList(npc->directions,1);
    }
    if(topRight){
        xhere++;
        yhere--;
        addToList(npc->directions,3);
    }
    if(left){
        xhere--;
        addToList(npc->directions,4);
    }
    if(right){
        xhere++;
        addToList(npc->directions,5);
    }
    if(bottomLeft){
        xhere--;
        yhere++;
        addToList(npc->directions,6);
    }
    if(bottomRight){
        xhere++;
        yhere++;
        addToList(npc->directions,8);
    }
    if(bottom){
        yhere++;
        addToList(npc->directions,7);
    }
    }
}
static void getDirections(Monster *npc){/*Hey Shane please work here on this function and check for correct dimensions and terrain*/
    int done=0;
    volatile int xhere = npc->xloc;
    volatile int yhere = npc->yloc;
    while(!done){


   volatile int min = 1000;

    volatile int topLeft=0;
    volatile int top=0;
    volatile int topRight=0;
    volatile int left=0;
    volatile int right =0;
    volatile int bottomLeft =0;
    volatile int bottom =0;
    volatile int bottomRight=0;
    /*Top Left*/
    if(xhere-1!=0 && yhere-1!=0 && (m->mapArray[yhere-1][xhere-1]==('#') || m->mapArray[yhere-1][xhere-1]==('.'))){
            int temp = m->roomDistanceGrid[yhere-1][xhere-1].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                 topLeft=1;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*Top*/
    if(yhere-1!=0 && (m->mapArray[yhere-1][xhere]==('#') || m->mapArray[yhere-1][xhere]==('.'))){
            int temp = m->roomDistanceGrid[yhere-1][xhere].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                 topLeft=0;
                 top=1;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*TopRight*/
    if((xhere+1!=79 && yhere-1!=0) && (m->mapArray[yhere-1][xhere+1]=='#' || m->mapArray[yhere-1][xhere+1]=='.')){
            int temp = m->roomDistanceGrid[yhere-1][xhere+1].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=1;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*Left*/
    if(xhere-1!=0 && (m->mapArray[yhere][xhere-1]==('#') || m->mapArray[yhere][xhere-1]==('.'))){
            int temp = m->roomDistanceGrid[yhere][xhere-1].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=1;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*Right*/
    if(xhere+1!=79 && (m->mapArray[yhere][xhere+1]==('#') || m->mapArray[yhere][xhere+1]==('.'))){
            int temp = m->roomDistanceGrid[yhere][xhere+1].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =1;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*BottomLeft*/
    if(xhere-1!=0 && yhere+1!=20 && (m->mapArray[yhere+1][xhere-1]==('#') || m->mapArray[yhere+1][xhere-1]==('.'))){
            int temp = m->roomDistanceGrid[yhere+1][xhere-1].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =1;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*Bottom*/
    if(yhere+1!=79 && (m->mapArray[yhere+1][xhere]==('#') || m->mapArray[yhere+1][xhere]==('.'))){
            int temp = m->roomDistanceGrid[yhere+1][xhere].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =1;
                 bottomRight=0;
            }
    }
    /*BottomRight*/
    if(xhere+1!=79 && yhere+1!=20 && (m->mapArray[yhere+1][xhere+1]==('#') || m->mapArray[yhere+1][xhere+1]==('.'))){
            int temp = m->roomDistanceGrid[yhere+1][xhere+1].distance;
            if(temp==0){
                done=1;
            }
            if(temp<=min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=1;
            }
    }
    if(top){
        yhere--;
        addToList(npc->directions,2);
    }
    if(topLeft){
        yhere--;
        xhere--;
        addToList(npc->directions,1);
    }
    if(topRight){
        xhere++;
        yhere--;
        addToList(npc->directions,3);
    }
    if(left){
        xhere--;
        addToList(npc->directions,4);
    }
    if(right){
        xhere++;
        addToList(npc->directions,5);
    }
    if(bottomLeft){
        yhere++;
        xhere--;
        addToList(npc->directions,6);
    }
    if(bottomRight){
        xhere++;
        yhere++;
        addToList(npc->directions,8);
    }
    if(bottom){
        yhere++;
        addToList(npc->directions,7);
    }

    }
}
/*
    [1][2][3]
    [4][0][5]
    [6][7][8]
*/

static void performWander(Monster *npc){
        int control = rand()%8;
        int digSwitch = canTunnle(npc);
        while(1){
        control = rand()%8;
        switch(control){

            case 0:/*Move Up*/
            if(npc->yloc-1>0){
            if(m->mapArray[npc->yloc-1][npc->xloc]==('#') || m->mapArray[npc->yloc-1][npc->xloc]==('.') || digSwitch){
                int isd = moveUp(npc);
                if(!isd){
                    return;
                }
            }
        }
            break;

            case 1:/*Move Down*/
            if(npc->yloc+1<20){

                if(m->mapArray[npc->yloc+1][npc->xloc]==('#') || m->mapArray[npc->yloc+1][npc->xloc]==('.') || digSwitch){

                    if(!moveDown(npc)){
                        return;
                    }
            }
            }
            break;

            case 2:/*Move Right*/

            if(npc->xloc+1<79){

            if(m->mapArray[npc->yloc][npc->xloc+1]==('#') || m->mapArray[npc->yloc][npc->xloc+1]==('.') || digSwitch){

                if(!moveRight(npc)){
                    return;
                }
            }
        }
            break;

            case 3:/*Move Left*/

             if(npc->xloc-1>0){

             if(m->mapArray[npc->yloc][npc->xloc-1]==('#') || m->mapArray[npc->yloc][npc->xloc-1]==('.') || digSwitch){

                if(!moveLeft(npc)){
                    return;
                }
            }
             }
            break;
            case 4:/*Move TopRight*/
            if(npc->yloc-1>0 && npc->xloc+1<79){

            if(m->mapArray[npc->yloc-1][npc->xloc+1]==('#') || m->mapArray[npc->yloc-1][npc->xloc+1]==('.') || digSwitch ){

                if(!moveTopRight(npc)){
                    return;
                }
            }
            }
            break;

            case 5:/*Move TopLeft*/
            if(npc->yloc-1>0 && npc->xloc-1>0){

            if(m->mapArray[npc->yloc-1][npc->xloc-1]==('#') || m->mapArray[npc->yloc-1][npc->xloc-1]==('.') || digSwitch ){

                if(!moveTopLeft(npc)){
                    return;
                }
            }
        }
            break;
            case 6:/*Move BottomRight*/
             if(npc->yloc+1<20 && npc->xloc+1<79){

             if(m->mapArray[npc->yloc+1][npc->xloc+1]==('#') || m->mapArray[npc->yloc+1][npc->xloc+1]==('.') || digSwitch){

                if(!moveBottomRight(npc)){
                    return;
                }
            }
        }
            break;
            case 7:/*Move BottomLeft*/
            if(npc->yloc+1<20 && npc->xloc-1>0){

             if(m->mapArray[(npc->yloc)+1][npc->xloc-1]==('#') || m->mapArray[(npc->yloc)+1][npc->xloc-1]==('.') || digSwitch){

                if(!moveBottomLeft(npc)){
                    return;
                }
                break;
            }
        }
}
        }
}
static void moveNearestNonTunneling(Monster *npc){
    int min = 1000;
    int xhere = npc->xloc;
    int yhere = npc->yloc;


    int topLeft=0;
    int top=0;
    int topRight=0;
    int left=0;
    int right =0;
    int bottomLeft =0;
    int bottom =0;
    int bottomRight=0;
    /*Top Left*/
    if(xhere-1!=0 && yhere-1!=0){
        if(m->mapArray[yhere-1][xhere-1]==('#') || m->mapArray[yhere-1][xhere-1]==('.')){
            int temp = m->roomDistanceGrid[yhere-1][xhere-1].distance;
            if(temp<min){
                min=temp;
                 topLeft=1;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
        }
    }
    /*Top*/
    if(yhere-1!=0){
        if(m->mapArray[yhere-1][xhere]==('#') || m->mapArray[yhere-1][xhere]==('.')){
            int temp = m->roomDistanceGrid[yhere-1][xhere].distance;
            if(temp<min){
                min=temp;
                 topLeft=0;
                 top=1;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
        }
    }
    /*TopRight*/
    if(xhere+1!=79 && yhere-1!=0){
        if(m->mapArray[yhere-1][xhere+1]==('#') || m->mapArray[yhere-1][xhere+1]==('.')){
            int temp = m->roomDistanceGrid[yhere-1][xhere+1].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=1;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
        }
    }
    /*Left*/
    if(xhere-1!=0){
        if(m->mapArray[yhere][xhere-1]==('#') || m->mapArray[yhere][xhere-1]==('.')){
            int temp = m->roomDistanceGrid[yhere][xhere-1].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=1;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
        }
    }
    /*Right*/
    if(xhere+1!=79){
        if(m->mapArray[yhere-1][xhere+1]==('#') || m->mapArray[yhere-1][xhere+1]==('.')){
            int temp = m->roomDistanceGrid[yhere-1][xhere+1].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =1;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
        }
    }
    /*BottomLeft*/
    if(xhere-1!=0 && yhere+1!=20){
        if(m->mapArray[yhere+1][xhere-1]==('#') || m->mapArray[yhere+1][xhere-1]==('.')){
            int temp = m->roomDistanceGrid[yhere+1][xhere-1].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =1;
                 bottom =0;
                 bottomRight=0;
            }
        }
    }
    /*Bottom*/
    if(yhere+1!=79){
        if(m->mapArray[yhere+1][xhere]==('#') || m->mapArray[yhere+1][xhere]==('.')){
            int temp = m->roomDistanceGrid[yhere+1][xhere].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =1;
                 bottomRight=0;
            }
        }
    }
    /*BottomRight*/
    if(xhere!=0 && yhere+1!=0){
        if(m->mapArray[yhere+1][xhere]==('#') || m->mapArray[yhere+1][xhere]==('.')){
            int temp = m->roomDistanceGrid[yhere+1][xhere].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=1;
            }
        }
    }
    if(top){
        moveUp(npc);
    }
    if(topLeft){
        moveTopLeft(npc);
    }
    if(topRight){
        moveTopRight(npc);
    }
    if(left){
        moveLeft(npc);
    }
    if(right){
        moveRight(npc);
    }
    if(bottomLeft){
        moveBottomLeft(npc);
    }
    if(bottomRight){
        moveBottomRight(npc);
    }
    if(bottom){
        moveDown(npc);
    }
}
static void moveNearestTunneling(Monster *npc){
    int min = 1000;
    int xhere = npc->xloc;
    int yhere = npc->yloc;
    int topLeft=0;
    int top=0;
    int topRight=0;
    int left=0;
    int right =0;
    int bottomLeft =0;
    int bottom =0;
    int bottomRight=0;
    /*Top Left*/
    if(xhere-1!=0 && yhere-1!=0){
            int temp = m->wallDistanceGrid[yhere-1][xhere-1].distance;
            if(temp<min){
                min=temp;
                 topLeft=1;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*Top*/
    if(yhere-1!=0){
            int temp = m->wallDistanceGrid[yhere-1][xhere].distance;
            if(temp<min){
                min=temp;
                 topLeft=0;
                 top=1;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*TopRight*/
    if(xhere+1!=79 && yhere-1!=0){
            int temp = m->wallDistanceGrid[yhere-1][xhere+1].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=1;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*Left*/
    if(xhere-1!=0){
            int temp = m->wallDistanceGrid[yhere][xhere-1].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=1;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*Right*/
    if(xhere+1!=79){
            int temp = m->wallDistanceGrid[yhere][xhere+1].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =1;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*BottomLeft*/
    if(xhere-1!=0 && yhere+1!=20){
            int temp = m->wallDistanceGrid[yhere+1][xhere-1].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =1;
                 bottom =0;
                 bottomRight=0;
            }
    }
    /*Bottom*/
    if(yhere+1!=79){
            int temp = m->wallDistanceGrid[yhere+1][xhere].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =1;
                 bottomRight=0;
            }
    }
    /*BottomRight*/
    if(xhere+1!=79 && yhere+1!=20){
            int temp = m->wallDistanceGrid[yhere+1][xhere+1].distance;
            if(temp<min){
                min=temp;
                topLeft=0;
                 top=0;
                 topRight=0;
                 left=0;
                 right =0;
                 bottomLeft =0;
                 bottom =0;
                 bottomRight=1;
            }
    }
    if(top){
        moveUp(npc);
    }
    if(topLeft){
        moveTopLeft(npc);
    }
    if(topRight){
        moveTopRight(npc);
    }
    if(left){
        moveLeft(npc);
    }
    if(right){
        moveRight(npc);
    }
    if(bottomLeft){
        moveBottomLeft(npc);
    }
    if(bottomRight){
        moveBottomRight(npc);
    }
    if(bottom){
        moveDown(npc);
    }
}
/*********************************/
void scanArea(Monster *mon){
    int xhere = mon->xloc;
    int yhere = mon->yloc;
    /*Monster Looks Forward*/
    int done=0;
    int ytemp = yhere;
    while(!done){
        ytemp++;
        int test =0;
        char first = (*m).mapArray[ytemp][xhere];
        if(first=='.'){
            if(first=='#'){
                test=1;
            }
        }

        if( test && (ytemp>0)){
            if(hasMonster(ytemp,xhere)){
                Monster *temp;
                 temp = monsterArray[ytemp][xhere];

                if(temp->thePlayer){
                    mon->patrolMode=0;
                    done=1;
                    mon->searchLocationY=ytemp;
                }
            }
        }else{
            done=1;
        }
    }
    /*Monster Looks Behind*/
    done=0;
    ytemp = yhere;
    while(!done){
        ytemp--;
        if((m->mapArray[ytemp][xhere]=='.' || m->mapArray[ytemp][xhere]=='#') && ytemp<20){
            if(hasMonster(ytemp,xhere)){
                 Monster *temp;
                 temp = monsterArray[ytemp][xhere];
                if(temp->thePlayer){
                    done=1;
                    mon->patrolMode=0;
                    mon->searchLocationY=ytemp;
                }
            }
        }else{
            done=1;
        }
    }
    /*Monster Looks Right*/
    done=0;
    int xtemp = xhere;
    while(!done){
        xtemp++;
        if((m->mapArray[ytemp][xhere]=='.' || m->mapArray[ytemp][xhere]=='#') && xtemp<79){
            if(hasMonster(ytemp,xhere)){
                 Monster *temp;
                 temp = monsterArray[ytemp][xhere];
                if(temp->thePlayer){
                    mon->patrolMode=0;
                    done=1;
                    mon->searchLocationY=xtemp;
                }
            }
        }else{
            done=1;
        }
    }
    /*Monster Looks Left*/
    done=0;
    xtemp = xhere;
    while(!done){
        xtemp--;
        if((m->mapArray[ytemp][xhere]=='.' || m->mapArray[ytemp][xhere])=='#'  && xtemp>0){
            if(hasMonster(ytemp,xhere)){
                 Monster *temp;
                 temp = monsterArray[ytemp][xhere];
                if(temp->thePlayer){
                    mon->patrolMode=0;
                    done=1;
                    mon->searchLocationY=xtemp;
                }
            }
        }else{
            done=1;
        }
    }
    /*Checks Room*/
    int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    room *room1;
    room1 = pointContains(y,x);
    int x1 = mon->xloc;
    int y1 = mon->yloc;
    room *room2;
    room2 = pointContains(y1,x1);
    if(room1!=NULL && room2!=NULL){
    if(room1->cornerArray[0][1] == room2->cornerArray[0][1]){
        if(room1->cornerArray[0][0] == room2->cornerArray[0][0]){
            if(room1->cornerArray[1][1] == room2->cornerArray[1][1]){
                if(room1->cornerArray[1][0] == room2->cornerArray[1][0]){
                        mon->patrolMode=0;
                        mon->searchLocationY=y;
                        mon->searchLocationX=x;
                }
            }
        }
    }
    }
}

/*Functions for Library*/
int hasMonster(int yl, int xl){
    if(yl>21 || 0>yl){
        return 0;
    }
    if(xl>80 || 0>xl){
        return 0;
    }
    if(!monsterArray[yl][xl]){
        return 0;
    }
    return 1;
}
