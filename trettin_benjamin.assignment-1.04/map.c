#include "heap.h"
#include "gameMap.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <endian.h>
#include <stdint.h>
#include <sys/stat.h>
#include <limits.h>


#define X 21
#define Y 80
#define floor '.'
#define rock ' '
#define cooridor '#'
#define pc '@'
#define horizontalWall '-'
#define verticalWall '|'

Map *m;
char weightArray[X][Y];
int numberOfMonster;

typedef struct{
    int x;
    int y;
}Pair;

int32_t compare_monster(const void *key,const void *with){
    int32_t first = (*(const Monster *) key).roundVal;
    int32_t second = (*(const Monster *) with).roundVal;
    return first - second;
}

static Pair getCoords(){
    Pair coords;
    int x;
    int y;
    while(1){
     y = (rand()%18)+1;
     x = (rand()%78)+1;
    if((*m).mapArray[y][x]==('#') || (*m).mapArray[y][x]==('.')){
        if(monsterArray[y][x]==NULL){
        coords.x=x;
        coords.y=y;
        return coords;
        }
    }
}


    return coords;
}

room* pointContains(int y,int x){
    int xt;
    room *temp;
    int number = m->numOfRooms;
    for(xt=0;xt<number;xt++){
        temp = &(m->rooms[xt]);
        if(y>=temp->cornerArray[0][1]){
            if(y<=temp->cornerArray[1][1]){
                if(x>=temp->cornerArray[0][0]){
                    if(x<=temp->cornerArray[1][0]){
                        return temp;
                    }
                }
            }
        }
    }
    return NULL;
}
void generateRoom(int roomNumber){
    int upperX = 79;
    int lowerX = 1;
    int randX = (rand() % (upperX-lowerX + 1)) + lowerX;
    int upperY = 19;
    int lowerY = 1;
    int randY = (rand() % (upperY-lowerY + 1)) + lowerY;

    int upperWidth = 20;
    int lowerWidth = 3;
    int randWidth = (rand() % (upperWidth-lowerWidth + 1)) + lowerWidth;
    int upperHeight = 10;
    int lowerHeight = 2;
    int randHeight = (rand() % (upperHeight-lowerHeight + 1)) + lowerHeight;

    if(randX+randWidth>79 || randY+randHeight>19 || randY-randHeight<2){
        return generateRoom(roomNumber);
    }

    for(int i=randY+randHeight-1; i>randY-1; i--){
       for(int j=randX; j<randX+randWidth; j++){
           if((*m).mapArray[i][j] == floor || (*m).mapArray[i+1][j] == floor || (*m).mapArray[i-1][j] == floor || (*m).mapArray[i][j-1] == floor || (*m).mapArray[i][j+1] == floor){
               return generateRoom(roomNumber);
           }
        }
     }
     for(int i=randY+randHeight-1; i>randY-1; i--){
       for(int j=randX; j<randX+randWidth; j++){
            (*m).mapArray[i][j] = floor;
            (*m).hardnessArray[i][j] = 0;
       }
     }
    if(roomNumber == 0){
      (*m).pcStartX = randX;
      (*m).pcStartY = randY;
    }
    (*m).rooms[roomNumber].cornerArray[0][0] = randX;
    (*m).rooms[roomNumber].cornerArray[0][1] = randY;
    (*m).rooms[roomNumber].cornerArray[1][0] = randX+randWidth-1;
    (*m).rooms[roomNumber].cornerArray[1][1] = randY+randHeight-1;
}

void connectRooms(int startRoom ,int endRoom){
    int randStart = (rand() % (2));
    int xStart = (*m).rooms[startRoom].cornerArray[randStart][0];
    int yStart = (*m).rooms[startRoom].cornerArray[randStart][1];

    int randEnd = (rand() % (2));
    int xEnd = (*m).rooms[endRoom].cornerArray[randEnd][0];
    int yEnd = (*m).rooms[endRoom].cornerArray[randEnd][1];

    int tempXstart = xStart;
    int tempXend = xEnd;
    while(tempXstart<tempXend){
        if((*m).mapArray[yStart][tempXstart+1] == rock){
        (*m).mapArray[yStart][tempXstart+1] = cooridor;
        (*m).hardnessArray[yStart][tempXstart+1] = 0;
        }
        tempXstart++;
    }
    int tempXstart2 = xStart;
    int tempXend2 = xEnd;
    while(tempXstart2>tempXend2){
        if((*m).mapArray[yStart][tempXstart2-1] == rock){
        (*m).mapArray[yStart][tempXstart2-1] = cooridor;
        (*m).hardnessArray[yStart][tempXstart2-1] = 0;
        }
        tempXstart2--;
    }

    int tempYstart = yStart;
    int tempYend = yEnd;
    while(tempYstart<tempYend){
        if((*m).mapArray[tempYstart+1][xEnd] == rock){
        (*m).mapArray[tempYstart+1][xEnd] = cooridor;
        (*m).hardnessArray[tempYstart+1][xEnd] = 0;
        }
        tempYstart++;
    }
    int tempYstart2 = yStart;
    int tempYend2 = yEnd;
    while(tempYstart2>tempYend2){
        if((*m).mapArray[tempYstart2-1][xEnd] == rock){
        (*m).mapArray[tempYstart2-1][xEnd] = cooridor;
        (*m).hardnessArray[tempYstart2-1][xEnd] = 0;
        }
        tempYstart2--;
    }
}
void generateCooridors(int roomCount){
    connectRooms(0,1);
    for(int i=2; i<roomCount; i++){
        int lowestDistance = 100;
        int closestRoom = 1;
        for(int j=0; j<i; j++){
            int xCenter = ((*m).rooms[j].cornerArray[0][0] + (*m).rooms[j].cornerArray[1][0])/2;
            int yCenter = ((*m).rooms[j].cornerArray[0][1] + (*m).rooms[j].cornerArray[1][1])/2;
            int centerX = ((*m).rooms[i].cornerArray[0][0] + (*m).rooms[i].cornerArray[1][0])/2;
            int centerY = ((*m).rooms[i].cornerArray[0][1] + (*m).rooms[i].cornerArray[1][1])/2;
            int gdistance = (centerX-xCenter)*(centerX-xCenter)+(centerY-yCenter)*(centerY-yCenter);
            int distance = sqrt(gdistance);
            if(distance<lowestDistance){
                lowestDistance = distance;
                closestRoom = j;
            }
        }
        connectRooms(i,closestRoom);
    }
}

static int getWeight(int num){
    if(num<85){
        return 1;
    }
    if(num<171){
        return 2;
    }
    if(num<255){
        return 3;
    }
    return 10000;
}

int32_t compare_cell(const void *key,const void *with){
  return (*(const distanceCell *) key).distance - (*(const distanceCell *) with).distance;
}

static void djikstra_cooridor(){
    for( int xPre=0;xPre<80;xPre++){
        for( int yPre=0;yPre<21;yPre++){
            distanceCell pass;
            pass.distance=1000;
            pass.yloc=yPre;/* 1000 will represent infinity */
            pass.xloc=xPre;
            (*m).roomDistanceGrid[yPre][xPre]=pass;
        }
    }
    (*m).roomDistanceGrid[(*m).pcStartY][(*m).pcStartX].distance=0;
    binheap_t heap;
    binheap_init(&heap,compare_cell,free);
    int pcXl;
    int pcYl;
    pcXl=(*m).pcStartX;
    pcYl=(*m).pcStartY;
    distanceCell root = (*m).roomDistanceGrid[pcYl][pcXl];
    root.distance=0;
    binheap_insert(&heap,&root);
    int tempx;
    int tempy;
    while(!binheap_is_empty(&heap)){
        distanceCell *temp;
        temp =(distanceCell*) binheap_remove_min(&heap);
        tempx = (*temp).xloc;
        tempy = (*temp).yloc;

        if((*m).mapArray[(*temp).yloc-1][(*temp).xloc]=='.' || (*m).mapArray[(*temp).yloc-1][(*temp).xloc]=='#'){/* top */
                if((*m).roomDistanceGrid[tempy-1][tempx].distance==1000){
                    (*m).roomDistanceGrid[tempy-1][tempx].distance=(*temp).distance+1;
                    distanceCell *temp0;
                     temp0 = &(*m).roomDistanceGrid[tempy-1][tempx];
                    binheap_insert(&heap, temp0);
                }
        }
         if((*m).mapArray[(*temp).yloc+1][(*temp).xloc]=='.' || (*m).mapArray[(*temp).yloc+1][(*temp).xloc]=='#'){/* bottom */
                tempx = (*temp).xloc;
                tempy = (*temp).yloc;
                if((*m).roomDistanceGrid[tempy+1][tempx].distance==1000){
                    (*m).roomDistanceGrid[tempy+1][tempx].distance=(*temp).distance+1;
                    distanceCell *temp1;
                    temp1 = &(*m).roomDistanceGrid[tempy+1][tempx];
                    binheap_insert(&heap,temp1);
                }
        }
         if((*m).mapArray[(*temp).yloc][(*temp).xloc+1]=='.' || (*m).mapArray[(*temp).yloc][(*temp).xloc+1]=='#'){/* right */
                tempx = (*temp).xloc;
                tempy = (*temp).yloc;
                if((*m).roomDistanceGrid[tempy][tempx+1].distance==1000){
                    (*m).roomDistanceGrid[tempy][tempx+1].distance=(*temp).distance+1;
                    distanceCell *temp2;
                    temp2 = &(*m).roomDistanceGrid[tempy][tempx+1];
                    binheap_insert(&heap,temp2);
                }
        }
         if((*m).mapArray[(*temp).yloc][(*temp).xloc-1]=='.' || (*m).mapArray[(*temp).yloc][(*temp).xloc-1]=='#'){/* left */
                tempx = (*temp).xloc;
                tempy = (*temp).yloc;
                if((*m).roomDistanceGrid[tempy][tempx-1].distance==1000){
                    (*m).roomDistanceGrid[tempy][tempx-1].distance=(*temp).distance+1;
                    distanceCell *temp3;
                    temp3 = &(*m).roomDistanceGrid[tempy][tempx-1];
                    binheap_insert(&heap,temp3);
                }
        }
        if((*m).mapArray[(*temp).yloc-1][(*temp).xloc+1]=='.' || (*m).mapArray[(*temp).yloc-1][(*temp).xloc+1]=='#'){/* top right */
                tempx = (*temp).xloc;
                tempy = (*temp).yloc;
                if((*m).roomDistanceGrid[tempy-1][tempx+1].distance==1000){
                    (*m).roomDistanceGrid[tempy-1][tempx+1].distance=(*temp).distance+1;
                    distanceCell *temp4;
                    temp4 = &(*m).roomDistanceGrid[tempy-1][tempx+1];
                    binheap_insert(&heap,temp4);
                }
        }
        if((*m).mapArray[(*temp).yloc-1][(*temp).xloc-1]=='.' || (*m).mapArray[(*temp).yloc-1][(*temp).xloc-1]=='#'){/* top left */
                tempx = (*temp).xloc;
                tempy = (*temp).yloc;
                if((*m).roomDistanceGrid[tempy-1][tempx-1].distance==1000){
                    (*m).roomDistanceGrid[tempy-1][tempx-1].distance=(*temp).distance+1;
                    distanceCell *temp5;
                    temp5 = &(*m).roomDistanceGrid[tempy-1][tempx-1];
                   binheap_insert(&heap,temp5);
                }
        }
        if((*m).mapArray[(*temp).yloc+1][(*temp).xloc-1]=='.' || (*m).mapArray[(*temp).yloc+1][(*temp).xloc-1]=='#'){/* bottomLeft left */
                tempx = (*temp).xloc;
                tempy = (*temp).yloc;
                if((*m).roomDistanceGrid[tempy+1][tempx-1].distance==1000){
                    (*m).roomDistanceGrid[tempy+1][tempx-1].distance=(*temp).distance+1;
                    distanceCell *temp6;
                    temp6 = &(*m).roomDistanceGrid[tempy+1][tempx-1];
                    binheap_insert(&heap,temp6);
                }
        }
        if((*m).mapArray[(*temp).yloc+1][(*temp).xloc+1]=='.' || (*m).mapArray[(*temp).yloc+1][(*temp).xloc+1]=='#'){/* bottom right */
                tempx = (*temp).xloc;
                tempy = (*temp).yloc;
                if((*m).roomDistanceGrid[tempy+1][tempx+1].distance==1000){
                    (*m).roomDistanceGrid[tempy+1][tempx+1].distance=(*temp).distance+1;
                    distanceCell *temp7;
                    temp7 = &(*m).roomDistanceGrid[tempy+1][tempx+1];
                    binheap_insert(&heap,temp7);
                }
          }
    }
}
static void djikstra_wall(void){
    int xPre=0;
    int yPre=0;

    for(xPre=1;xPre<79;xPre++){
        for(yPre=1;yPre<20;yPre++){
            distanceCell pass;
            pass.distance=10000;
            pass.yloc=yPre;/* 1000 will represent infinity */
            pass.xloc=xPre;
            (*m).wallDistanceGrid[yPre][xPre]=pass;
        }
    }
    (*m).wallDistanceGrid[(*m).pcStartY][(*m).pcStartX].distance=0;
    binheap_t heapcell;
    binheap_init(&heapcell,compare_cell,free);

    distanceCell root = (*m).wallDistanceGrid[(*m).pcStartY][(*m).pcStartX];
    root.distance=0;
    binheap_insert(&heapcell,&root);
    int tempx;
    int tempy;
    while(!binheap_is_empty(&heapcell)){
        distanceCell *temp;
        temp =(distanceCell*) binheap_remove_min(&heapcell);
        tempx = (*temp).xloc;
        tempy = (*temp).yloc;
        int alt;

        if((*m).mapArray[(*temp).yloc-1][(*temp).xloc]!='-' || (*m).mapArray[(*temp).yloc-1][(*temp).xloc]!='|'){/* top */

               alt = getWeight((*m).hardnessArray[tempy][tempx])+(*m).wallDistanceGrid[tempy][tempx].distance;
                if((*m).wallDistanceGrid[tempy-1][tempx].distance>alt){
                    (*m).wallDistanceGrid[tempy-1][tempx].distance=alt;
                    distanceCell *temp0;
                    temp0 = &(*m).wallDistanceGrid[tempy-1][tempx];
                    binheap_insert(&heapcell, temp0);

            }
        }
         if((*m).mapArray[(*temp).yloc+1][(*temp).xloc]!='-' || (*m).mapArray[(*temp).yloc+1][(*temp).xloc]!='|'){/* bottom */

                alt = getWeight((*m).hardnessArray[tempy][tempx])+(*m).wallDistanceGrid[tempy][tempx].distance;
                if((*m).wallDistanceGrid[tempy+1][tempx].distance>alt){
                    (*m).wallDistanceGrid[tempy+1][tempx].distance=alt;
                    distanceCell *temp1;
                    temp1 = &(*m).wallDistanceGrid[tempy+1][tempx];
                    binheap_insert(&heapcell,temp1);
                }
        }
         if((*m).mapArray[(*temp).yloc][(*temp).xloc+1]!='-' || (*m).mapArray[(*temp).yloc][(*temp).xloc+1]!='|'){/* right */

                alt = getWeight((*m).hardnessArray[tempy][tempx])+(*m).wallDistanceGrid[tempy][tempx].distance;
                if((*m).wallDistanceGrid[tempy][tempx+1].distance>alt){
                    (*m).wallDistanceGrid[tempy][tempx+1].distance=alt;
                    distanceCell *temp2;
                    temp2 = &(*m).wallDistanceGrid[tempy][tempx+1];
                    binheap_insert(&heapcell,temp2);
                }
        }
         if((*m).mapArray[(*temp).yloc][(*temp).xloc-1]!='-' || (*m).mapArray[(*temp).yloc][(*temp).xloc-1]!='|'){/* left */


                 alt = getWeight((*m).hardnessArray[tempy][tempx])+(*m).wallDistanceGrid[tempy][tempx].distance;
                if((*m).wallDistanceGrid[tempy][tempx-1].distance>alt){
                    (*m).wallDistanceGrid[tempy][tempx-1].distance=alt;
                    distanceCell *temp3;
                    temp3 = &(*m).wallDistanceGrid[tempy][tempx-1];
                    binheap_insert(&heapcell,temp3);
                }
        }
        if((*m).mapArray[(*temp).yloc-1][(*temp).xloc+1]!='-' || (*m).mapArray[(*temp).yloc-1][(*temp).xloc+1]!='|'){/* top right */


                alt = getWeight((*m).hardnessArray[tempy][tempx])+(*m).wallDistanceGrid[tempy][tempx].distance;
                if((*m).wallDistanceGrid[tempy-1][tempx+1].distance>alt){
                    (*m).wallDistanceGrid[tempy-1][tempx+1].distance=alt;
                    distanceCell *temp4;
                    temp4 = &(*m).wallDistanceGrid[tempy-1][tempx+1];
                    binheap_insert(&heapcell,temp4);

                }


        }
        if((*m).mapArray[(*temp).yloc-1][(*temp).xloc-1]!='-' || (*m).mapArray[(*temp).yloc-1][(*temp).xloc-1]=='|'){/* top left */


                alt = getWeight((*m).hardnessArray[tempy][tempx])+(*m).wallDistanceGrid[tempy][tempx].distance;
                if((*m).wallDistanceGrid[tempy-1][tempx-1].distance>alt){
                    (*m).wallDistanceGrid[tempy-1][tempx-1].distance=alt;
                    distanceCell *temp5;
                    temp5 = &(*m).wallDistanceGrid[tempy-1][tempx-1];
                   binheap_insert(&heapcell,temp5);
                }


        }
        if((*m).mapArray[(*temp).yloc+1][(*temp).xloc-1]!='-' || (*m).mapArray[(*temp).yloc+1][(*temp).xloc-1]!='|'){/* bottomLeft left */


                 alt = getWeight((*m).hardnessArray[tempy][tempx])+(*m).wallDistanceGrid[tempy][tempx].distance;
                if((*m).wallDistanceGrid[tempy+1][tempx-1].distance>alt){
                    (*m).wallDistanceGrid[tempy+1][tempx-1].distance=alt;
                    distanceCell *temp6;
                    temp6 = &(*m).wallDistanceGrid[tempy+1][tempx-1];
                    binheap_insert(&heapcell,temp6);

                }


        }
        if((*m).mapArray[(*temp).yloc+1][(*temp).xloc+1]!='-' || (*m).mapArray[(*temp).yloc+1][(*temp).xloc+1]!='|'){/* bottom right */


                 alt = getWeight((*m).hardnessArray[tempy][tempx])+(*m).wallDistanceGrid[tempy][tempx].distance;
                if((*m).wallDistanceGrid[tempy+1][tempx+1].distance>alt){
                    (*m).wallDistanceGrid[tempy+1][tempx+1].distance=alt;
                    distanceCell *temp7;
                    temp7 = &(*m).wallDistanceGrid[tempy+1][tempx+1];
                    binheap_insert(&heapcell,temp7);

                }
        }


    }
}
void init(int numOfMonster){
  numberOfMonster=numOfMonster;
  m = (Map*)malloc(sizeof(Map));
   for(int i=0; i<X; i++){
       for(int j=0; j<Y; j++){
        if(i==0 || i==X-1){
        (*m).mapArray[i][j] = horizontalWall;
        (*m).hardnessArray[i][j] = (rand() % (243)) + 2;
        }
        else if(j==0 || j==Y-1){
        (*m).mapArray[i][j] = verticalWall;
        (*m).hardnessArray[i][j] = (rand() % (243)) + 2;
        }
        else{
        (*m).mapArray[i][j] = rock;
        (*m).hardnessArray[i][j] = (rand() % (243)) + 2;
        }
     }
   }
    int mostRooms = 10;
    int leastRooms = 5;
    (*m).numOfRooms = (rand() % (mostRooms-leastRooms + 1)) + leastRooms;
   for(int i=0; i<(*m).numOfRooms; i++){
       generateRoom(i);
   }
   generateCooridors((*m).numOfRooms);
   djikstra_cooridor();
   djikstra_wall();
}

int saveGame(){
    FILE *f;
    char *home = getenv("HOME");
    strcat(home,"/rlg327/");
    strcat(home,"dungeon");
    f = fopen(home,"w");
    if(!f){
      printf("could not write file\n");
      return 0;
    }
    char *title = "RLG327-F2018";
    int version = 0;
    version = htobe32(version);
    int sizeint;
    sizeint = 1702 + (4 * (*m).numOfRooms);
    sizeint = htobe32(sizeint);
    int startX = htobe32((*m).pcStartX);
    int startY = htobe32((*m).pcStartY);
    fwrite(title,1,12,f);
    fwrite(&version,4,1,f);
    fwrite(&sizeint,4,1,f);
    fwrite(&startX,1,1,f);
    fwrite(&startY,1,1,f);
    fwrite((*m).hardnessArray,1,1680,f);

    for (int i=0; i<(*m).numOfRooms; i++){
        int topLeftX;
        int xWidth;
        int topLeftY;
        int yWidth;
         topLeftY =  (*m).rooms[i].cornerArray[0][1];
         yWidth = (*m).rooms[i].cornerArray[1][1]-(*m).rooms[i].cornerArray[0][1]+1;
         topLeftX = (*m).rooms[i].cornerArray[0][0];
         xWidth = (*m).rooms[i].cornerArray[1][0]-(*m).rooms[i].cornerArray[0][0]+1;
        fwrite(&topLeftX, 1, 1, f);
        fwrite(&topLeftY, 1, 1, f);
        fwrite(&xWidth, 1, 1, f);
        fwrite(&yWidth, 1, 1, f);
    }
    fclose(f);
    printf("Succesfully Saved!\n");
    return 0;
}

void createRoom(int roomNumber,int x,int y,int width,int height){
  for(int i = y+height-1; i > y-1; i--){
    for(int j = x; j < x+width; j++){
         (*m).mapArray[i][j] = floor;
    }
  }
 if(roomNumber == 0){
   (*m).pcStartX = x;
   (*m).pcStartY = y;
 }
  (*m).rooms[roomNumber].cornerArray[0][0] = x;
  (*m).rooms[roomNumber].cornerArray[0][1] = y;
  (*m).rooms[roomNumber].cornerArray[1][0] = x+width-1;
  (*m).rooms[roomNumber].cornerArray[1][1] = y+height-1;
}


int loadGame(){
  for(int i=0; i<X; i++){
      for(int j=0; j<Y; j++){
       if(i==0 || i==X-1){
       (*m).mapArray[i][j] = horizontalWall;
       }
       else if(j==0 || j==Y-1){
       (*m).mapArray[i][j] = verticalWall;
       }
       else{
       (*m).mapArray[i][j] = rock;
       }
    }
  }
    int size;
    FILE *f;
    char title[12];
    int version;

    int startX;
    int startY;
    char *home;
    home = (char*) malloc(sizeof(char)*100);
    strcpy(home,getenv("HOME"));
    strcat(home,"/rlg327/");
    strcat(home,"dungeon");
    f = fopen(home,"r");
    if(!f){
        printf("cant open file");
        return 0;
    }
    unsigned char hardnessModel[21][80];

    fread(title,1,12,f);
    fread(&version,4,1,f);
    fread(&size,4,1,f);
    fread(&startX,1,1,f);
    fread(&startY,1,1,f);

    version=be32toh(version);
    size = be32toh(size);
    (*m).pcStartX = be32toh(startX);
    (*m).pcStartY = be32toh(startY);

    fread(hardnessModel,1,1680,f);
    for(int i=0; i<21; i++){
        for(int j=0; j<80; j++){
            (*m).hardnessArray[i][j]=hardnessModel[i][j];
        }
    }
    (*m).numOfRooms = (size-1702)/4;
    for(int i=0; i<21; i++){
        for(int j=0; j<80; j++){
            if(hardnessModel[i][j]==0){
                (*m).mapArray[i][j]= cooridor;
            }
        }
    }
    for (int i=0; i<(*m).numOfRooms; i++){
        uint8_t topLeftX;
        uint8_t xWidth;
        uint8_t topLeftY;
        uint8_t yWidth;
        fread(&topLeftX, sizeof(topLeftX), 1, f);
        fread(&topLeftY, sizeof(topLeftY), 1, f);
        fread(&xWidth, sizeof(xWidth), 1, f);
        fread(&yWidth, sizeof(yWidth), 1, f);
        createRoom(i, topLeftX, topLeftY, xWidth, yWidth);
    }
    (*m).mapArray[(*m).pcStartY][(*m).pcStartX] = pc;
    fclose(f);
    printf("Succesfully Loaded!\n");
    djikstra_cooridor();
    djikstra_wall();
    return 0;
  }

  void printMap(){
     for(int i=0; i<X; i++){
         for(int j=0; j<Y; j++){
           Monster *tempMon;
           tempMon = monsterArray[i][j];

               if(tempMon!=NULL){
                   if(tempMon->bigPeople){
                       printf("%c", 'P');
                   }
                   if(tempMon->dragon){
                       printf("%c", 'D');
                   }
                   if(tempMon->thePlayer){
                       printf("%c",'@');
                   }
                   if(tempMon->other){
                       printf("%c", 'p');
                   }
                 }
                   else{
          printf("%c", (*m).mapArray[i][j]);
        }
       }
       printf("\n");
     }
  }

  void printDistanceGrid(){
    for(int i=0; i<21; i++){
      for (int j=0; j<80; j++){
        char temp = (*m).mapArray[i][j];
        if((temp == '.') | (temp == '#')){
          int value = (*m).roomDistanceGrid[i][j].distance;
          value = value % 10;
          printf("%d", value);
        }
        else if(temp == '@'){
          printf("%c", '@');
        }
        else{
          printf("%c",' ');
        }
      }
      printf("\n");
    }
}
void printDistanceGridPlus(){
  for(int i=0; i<21; i++){
    for (int j=0; j<80; j++){
        char temp = (*m).mapArray[i][j];

        if((temp == '.') | (temp == '#') | (temp == ' ')){
          int value = (*m).wallDistanceGrid[i][j].distance;
          value = value % 10;
          printf("%d", value);
      }
        else if(temp == '@'){
          printf("%c", '@');
        }
    }
    printf("\n");
  }
}

void playGame(){
    int done = 0;
    binheap_t heap;
    binheap_init(&heap,compare_monster,free);
        initMonsterLib(m, numberOfMonster);
        /*Setup Monsters*/
        int x;
        for(x=0;x<numberOfMonster;x++){
            Pair coords;
            coords = getCoords();
            int isPlay=0;
            if(x==0){
                isPlay=1;
            }
            Monster *monster;
            monster = MonsterInit(m,coords.x,coords.y,isPlay);

            if(!x){
                m->thePlayer=monster;
            }

            binheap_insert(&heap,monster);
        }
    while(!done){

        Monster *tem;
        tem = (Monster*)binheap_remove_min(&heap);
        if((*tem).thePlayer){
            if(!tem->alive){
                printf("DEAD");
                return;
            }else{
                if(binheap_peek_min(&heap)==NULL){
                    printf("VICTORY");
                    return;
                }
            }
        }

        djikstra_cooridor();
        djikstra_wall();
        performAction(tem);
        tem->roundVal= tem->roundVal + tem->speed;
        if((*tem).alive){
            binheap_insert(&heap,tem);
        }else{
            deconstructor(tem);
        }
        printMap();
        //printDistanceGridPlus();
        usleep(250000);
        if(system ("clear")==1){
            printf("-=||=-");
        }
    }
}
