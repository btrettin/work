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
#include "heap.h"

#define X 21
#define Y 80
#define floor '.'
#define rock ' '
#define cooridor '#'
#define pc '@'
#define horizontalWall '-'
#define verticalWall '|'

typedef struct corridor_path corridor_path_t;

typedef struct{
  distanceCell roomDistanceGrid[30][100];
}
typedef struct{
    int distance;
    int xloc;
    int yloc;
}distanceCell;

unsigned char hardnessArray[X][Y];
char mapArray[X][Y];
int fullDistanceGrid[X][Y];
char mapArray[X][Y];
struct room{
    int cornerArray[2][2];
    int connected;
};
struct room rooms[100];
int numOfRooms;
int pcStartX;
int pcStartY;

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
           if(mapArray[i][j] == floor || mapArray[i+1][j] == floor || mapArray[i-1][j] == floor || mapArray[i][j-1] == floor || mapArray[i][j+1] == floor){
               return generateRoom(roomNumber);
           }
        }
     }
     for(int i=randY+randHeight-1; i>randY-1; i--){
       for(int j=randX; j<randX+randWidth; j++){
            mapArray[i][j] = floor;
            hardnessArray[i][j] = 0;
       }
     }
    if(roomNumber == 0){
      pcStartX = randX;
      pcStartY = randY;
    }
    rooms[roomNumber].cornerArray[0][0] = randX;
    rooms[roomNumber].cornerArray[0][1] = randY;
    rooms[roomNumber].cornerArray[1][0] = randX+randWidth-1;
    rooms[roomNumber].cornerArray[1][1] = randY+randHeight-1;
}

void connectRooms(int startRoom ,int endRoom){
    int randStart = (rand() % (2));
    int xStart = rooms[startRoom].cornerArray[randStart][0];
    int yStart = rooms[startRoom].cornerArray[randStart][1];

    int randEnd = (rand() % (2));
    int xEnd = rooms[endRoom].cornerArray[randEnd][0];
    int yEnd = rooms[endRoom].cornerArray[randEnd][1];

    int tempXstart = xStart;
    int tempXend = xEnd;
    while(tempXstart<tempXend){
        if(mapArray[yStart][tempXstart+1] == rock){
        mapArray[yStart][tempXstart+1] = cooridor;
        hardnessArray[yStart][tempXstart+1] = 0;
        }
        tempXstart++;
    }
    int tempXstart2 = xStart;
    int tempXend2 = xEnd;
    while(tempXstart2>tempXend2){
        if(mapArray[yStart][tempXstart2-1] == rock){
        mapArray[yStart][tempXstart2-1] = cooridor;
        hardnessArray[yStart][tempXstart2-1] = 0;
        }
        tempXstart2--;
    }

    int tempYstart = yStart;
    int tempYend = yEnd;
    while(tempYstart<tempYend){
        if(mapArray[tempYstart+1][xEnd] == rock){
        mapArray[tempYstart+1][xEnd] = cooridor;
        hardnessArray[tempYstart+1][xEnd] = 0;
        }
        tempYstart++;
    }
    int tempYstart2 = yStart;
    int tempYend2 = yEnd;
    while(tempYstart2>tempYend2){
        if(mapArray[tempYstart2-1][xEnd] == rock){
        mapArray[tempYstart2-1][xEnd] = cooridor;
        hardnessArray[tempYstart2-1][xEnd] = 0;
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
            int xCenter = (rooms[j].cornerArray[0][0] + rooms[j].cornerArray[1][0])/2;
            int yCenter = (rooms[j].cornerArray[0][1] + rooms[j].cornerArray[1][1])/2;
            int centerX = (rooms[i].cornerArray[0][0] + rooms[i].cornerArray[1][0])/2;
            int centerY = (rooms[i].cornerArray[0][1] + rooms[i].cornerArray[1][1])/2;
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
void init(){
   for(int i=0; i<X; i++){
       for(int j=0; j<Y; j++){
        if(i==0 || i==X-1){
        mapArray[i][j] = horizontalWall;
        hardnessArray[i][j] = (rand() % (254)) + 1;
        }
        else if(j==0 || j==Y-1){
        mapArray[i][j] = verticalWall;
        hardnessArray[i][j] = (rand() % (254)) + 1;
        }
        else{
        mapArray[i][j] = rock;
        hardnessArray[i][j] = (rand() % (254)) + 1;
        }
     }
   }
    int mostRooms = 10;
    int leastRooms = 5;
    numOfRooms = (rand() % (mostRooms-leastRooms + 1)) + leastRooms;
   for(int i=0; i<numOfRooms; i++){
       generateRoom(i);
   }
   mapArray[pcStartY][pcStartX] = pc;
   generateCooridors(numOfRooms);
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
    sizeint = 1702 + (4 * numOfRooms);
    sizeint = htobe32(sizeint);
    int startX = htobe32(pcStartX);
    int startY = htobe32(pcStartY);
    fwrite(title,1,12,f);
    fwrite(&version,4,1,f);
    fwrite(&sizeint,4,1,f);
    fwrite(&startX,1,1,f);
    fwrite(&startY,1,1,f);
    fwrite(hardnessArray,1,1680,f);

    for (int i=0; i<numOfRooms; i++){
        int topLeftX;
        int xWidth;
        int topLeftY;
        int yWidth;
         topLeftY =  rooms[i].cornerArray[0][1];
         yWidth = rooms[i].cornerArray[1][1]-rooms[i].cornerArray[0][1]+1;
         topLeftX = rooms[i].cornerArray[0][0];
         xWidth = rooms[i].cornerArray[1][0]-rooms[i].cornerArray[0][0]+1;
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
         mapArray[i][j] = floor;
    }
  }
 if(roomNumber == 0){
   pcStartX = x;
   pcStartY = y;
 }
  rooms[roomNumber].cornerArray[0][0] = x;
  rooms[roomNumber].cornerArray[0][1] = y;
  rooms[roomNumber].cornerArray[1][0] = x+width-1;
  rooms[roomNumber].cornerArray[1][1] = y+height-1;
}


int loadGame(){
  for(int i=0; i<X; i++){
      for(int j=0; j<Y; j++){
       if(i==0 || i==X-1){
       mapArray[i][j] = horizontalWall;
       }
       else if(j==0 || j==Y-1){
       mapArray[i][j] = verticalWall;
       }
       else{
       mapArray[i][j] = rock;
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
    pcStartX = be32toh(startX);
    pcStartY = be32toh(startY);

    fread(hardnessModel,1,1680,f);
    for(int i=0; i<21; i++){
        for(int j=0; j<80; j++){
            hardnessArray[i][j]=hardnessModel[i][j];
        }
    }
    numOfRooms = (size-1702)/4;
    for(int i=0; i<21; i++){
        for(int j=0; j<80; j++){
            if(hardnessModel[i][j]==0){
                mapArray[i][j]= cooridor;
            }
        }
    }
    for (int i=0; i<numOfRooms; i++){
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
    mapArray[pcStartY][pcStartX] = pc;
    fclose(f);
    printf("Succesfully Loaded!\n");
    djikstra_cooridor();
    return 0;
  }

  int32_t compare_cell(const void *key,const void *with){
  return (*(const distanceCell *) key).distance - (*(const distanceCell *) with).distance;
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

  static void djikstra_cooridor(){
      for( int xPre=0;xPre<80;xPre++){
          for( int yPre=0;yPre<21;yPre++){
              distanceCell pass;
              pass.distance=1000;
              pass.yloc=yPre;/* 1000 will represent infinity */
              pass.xloc=xPre;
              roomDistanceGrid[yPre][xPre]=pass;
          }
      }
      roomDistanceGrid[pcStartY][pcStartX].distance=0;
      binheap_t heap;
      binheap_init(&heap,compare_cell,free);
      int pcXl;
      int pcYl;
      pcXl=pcStartX;
      pcYl=pcStartY;
      distanceCell root = roomDistanceGrid[pcYl][pcXl];
      root.distance=0;
      binheap_insert(&heap,&root);
      int tempx;
      int tempy;
      while(!binheap_is_empty(&heap)){
          distanceCell *temp;
          temp =(distanceCell*) binheap_remove_min(&heap);
          tempx = (*temp).xloc;
          tempy = (*temp).yloc;
          int nextVal = roomDistanceGrid[tempy][tempx].distance+1;

          if(mapArray[(*temp).yloc-1][(*temp).xloc]=='.' || mapArray[(*temp).yloc-1][(*temp).xloc]=='#'){/* top */
                  if((roomDistanceGrid[tempy-1][tempx].distance==1000){
                      roomDistanceGrid[tempy-1][tempx].distance=(*temp).distance+1;
                      distanceCell *temp0;
                       temp0 = &roomDistanceGrid[tempy-1][tempx];
                      binheap_insert(&heap, temp0);
                  }
          }
           if(mapArray[(*temp).yloc+1][(*temp).xloc]=='.' || mapArray[(*temp).yloc+1][(*temp).xloc]=='#'){/* bottom */
                  tempx = (*temp).xloc;
                  tempy = (*temp).yloc;
                  if(roomDistanceGrid[tempy+1][tempx].distance==1000){
                      roomDistanceGrid[tempy+1][tempx].distance=(*temp).distance+1;
                      distanceCell *temp1;
                      temp1 = &roomDistanceGrid[tempy+1][tempx];
                      binheap_insert(&heap,temp1);
                  }
          }
           if(mapArray[(*temp).yloc][(*temp).xloc+1]=='.' || mapArray[(*temp).yloc][(*temp).xloc+1]=='#'){/* right */
                  tempx = (*temp).xloc;
                  tempy = (*temp).yloc;
                  if(roomDistanceGrid[tempy][tempx+1].distance==1000){
                      roomDistanceGrid[tempy][tempx+1].distance=(*temp).distance+1;
                      distanceCell *temp2;
                      temp2 = &roomDistanceGrid[tempy][tempx+1];
                      binheap_insert(&heap,temp2);
                  }
          }
           if(mapArray[(*temp).yloc][(*temp).xloc-1]=='.' || mapArray[(*temp).yloc][(*temp).xloc-1]=='#'){/* left */
                  tempx = (*temp).xloc;
                  tempy = (*temp).yloc;
                  if(roomDistanceGrid[tempy][tempx-1].distance==1000){
                      roomDistanceGrid[tempy][tempx-1].distance=(*temp).distance+1;
                      distanceCell *temp3;
                      temp3 = &roomDistanceGrid[tempy][tempx-1];
                      binheap_insert(&heap,temp3);
                  }
          }
          if(mapArray[(*temp).yloc-1][(*temp).xloc+1]=='.' || mapArray[(*temp).yloc-1][(*temp).xloc+1]=='#'){/* top right */
                  tempx = (*temp).xloc;
                  tempy = (*temp).yloc;
                  if(roomDistanceGrid[tempy-1][tempx+1].distance==1000){
                      roomDistanceGrid[tempy-1][tempx+1].distance=(*temp).distance+1;
                      distanceCell *temp4;
                      temp4 = &roomDistanceGrid[tempy-1][tempx+1];
                      binheap_insert(&heap,temp4);
                  }
          }
          if(mapArray[(*temp).yloc-1][(*temp).xloc-1]=='.' || mapArray[(*temp).yloc-1][(*temp).xloc-1]=='#'){/* top left */
                  tempx = (*temp).xloc;
                  tempy = (*temp).yloc;
                  if(roomDistanceGrid[tempy-1][tempx-1].distance==1000){
                      roomDistanceGrid[tempy-1][tempx-1].distance=(*temp).distance+1;
                      distanceCell *temp5;
                      temp5 = &roomDistanceGrid[tempy-1][tempx-1];
                     binheap_insert(&heap,temp5);
                  }
          }
          if(mapArray[(*temp).yloc+1][(*temp).xloc-1]=='.' || mapArray[(*temp).yloc+1][(*temp).xloc-1]=='#'){/* bottomLeft left */
                  tempx = (*temp).xloc;
                  tempy = (*temp).yloc;
                  if(roomDistanceGrid[tempy+1][tempx-1].distance==1000){
                      roomDistanceGrid[tempy+1][tempx-1].distance=(*temp).distance+1;
                      distanceCell *temp6;
                      temp6 = &roomDistanceGrid[tempy+1][tempx-1];
                      binheap_insert(&heap,temp6);
                  }
          }
          if(mapArray[(*temp).yloc+1][(*temp).xloc+1]=='.' || mapArray[(*temp).yloc+1][(*temp).xloc+1]=='#'){/* bottom right */
                  tempx = (*temp).xloc;
                  tempy = (*temp).yloc;
                  if(roomDistanceGrid[tempy+1][tempx+1].distance==1000){
                      roomDistanceGrid[tempy+1][tempx+1].distance=(*temp).distance+1;
                      distanceCell *temp7;
                      temp7 = &roomDistanceGrid[tempy+1][tempx+1];
                      binheap_insert(&heap,temp7);
                  }
            }
      }
}
  static void djikstra_wall(){
    for( int xPre=1;xPre<79;xPre++){
        for(int yPre=1;yPre<20;yPre++){
            distanceCell pass;
            pass.distance=10000;
            pass.yloc=yPre;/* 1000 will represent infinity */
            pass.xloc=xPre;
            roomDistanceGrid[yPre][xPre]=pass;
        }
    }
    roomDistanceGrid[pcStartY][pcStartX].distance=0;
    binheap_t heap;
    binheap_init(&heap,compare_cell,free);
    int pcXl;
    int pcYl;
    pcXl=pcStartX;
    pcYl=pcStartY;
    distanceCell root = roomDistanceGrid[pcYl][pcXl];
    root.distance=0;
    binheap_insert(&heap,&root);
    int tempx;
    int tempy;
    while(!binheap_is_empty(&heap)){
        distanceCell *temp;
        temp =(distanceCell*) binheap_remove_min(&heap);
        tempx = (*temp).xloc;
        tempy = (*temp).yloc;
        int alt;

        if(mapArray[(*temp).yloc-1][(*temp).xloc]!='-' || mapArray[(*temp).yloc-1][(*temp).xloc]!='|'){/* top */
                alt = getWeight(hardnessArray[tempy-1][tempx]);
                alt = alt + roomDistanceGrid[tempy][tempx].distance;
                if(roomDistanceGrid[tempy-1][tempx].distance>alt){
                    roomDistanceGrid[tempy-1][tempx].distance=alt;
                    distanceCell *temp0;
                    temp0 = &roomDistanceGrid[tempy-1][tempx];
                    binheap_insert(&heap, temp0);
            }
        }
         if(mapArray[(*temp).yloc+1][(*temp).xloc]!='-' || mapArray[(*temp).yloc+1][(*temp).xloc]!='|'){/* bottom */
                alt = getWeight(hardnessArray[tempy+1][tempx])+roomDistanceGrid[tempy][tempx].distance;
                if(roomDistanceGrid[tempy+1][tempx].distance>alt){
                    roomDistanceGrid[tempy+1][tempx].distance=(*temp).distance+1;
                    distanceCell *temp1;
                    temp1 = &roomDistanceGrid[tempy+1][tempx];
                    binheap_insert(&heap,temp1);
                }
        }
         if(mapArray[(*temp).yloc][(*temp).xloc+1]!='-' || mapArray[(*temp).yloc][(*temp).xloc+1]!='|'){/* right */
                alt = getWeight(hardnessArray[tempy][tempx+1])+roomDistanceGrid[tempy][tempx].distance;
                if(roomDistanceGrid[tempy][tempx+1].distance>alt){
                    roomDistanceGrid[tempy][tempx+1].distance=(*temp).distance+1;
                    distanceCell *temp2;
                    temp2 = &roomDistanceGrid[tempy][tempx+1];
                    binheap_insert(&heap,temp2);
                }
        }
         if(mapArray[(*temp).yloc][(*temp).xloc-1]!='-' || mapArray[(*temp).yloc][(*temp).xloc-1]!='|'){/* left */
                 alt = getWeight(hardnessArray[tempy][tempx])+roomDistanceGrid[tempy][tempx].distance;
                if(roomDistanceGrid[tempy][tempx-1].distance>alt){
                    roomDistanceGrid[tempy][tempx-1].distance=alt;
                    distanceCell *temp3;
                    temp3 = &roomDistanceGrid[tempy][tempx-1];
                    binheap_insert(&heap,temp3);
                }
        }
        if(mapArray[(*temp).yloc-1][(*temp).xloc+1]!='-' || mapArray[(*temp).yloc-1][(*temp).xloc+1]!='|'){/* top right */
                alt = getWeight(hardnessArray[tempy-1][tempx+1])+roomDistanceGrid[tempy][tempx].distance;
                if(roomDistanceGrid[tempy-1][tempx+1].distance>alt){
                    roomDistanceGrid[tempy-1][tempx+1].distance=alt;
                    distanceCell *temp4;
                    temp4 = &roomDistanceGrid[tempy-1][tempx+1];
                    binheap_insert(&heap,temp4);
                }
        }
        if(mapArray[(*temp).yloc-1][(*temp).xloc-1]!='-' || mapArray[(*temp).yloc-1][(*temp).xloc-1]=='|'){/* top left */
                 alt = getWeight(hardnessArray[tempy-1][tempx-1])+roomDistanceGrid[tempy][tempx].distance;
                if(roomDistanceGrid[tempy-1][tempx-1].distance>alt){
                    roomDistanceGrid[tempy-1][tempx-1].distance=alt;
                    distanceCell *temp5;
                    temp5 = &roomDistanceGrid[tempy-1][tempx-1];
                   binheap_insert(&heap,temp5);
                }
        }
        if(mapArray[(*temp).yloc+1][(*temp).xloc-1]!='-' || mapArray[(*temp).yloc+1][(*temp).xloc-1]!='|'){/* bottomLeft left */
                 alt = getWeight(hardnessArray[tempy+1][tempx-1])+roomDistanceGrid[tempy][tempx].distance;
                if(roomDistanceGrid[tempy+1][tempx-1].distance>alt){
                    roomDistanceGrid[tempy+1][tempx-1].distance=alt;
                    distanceCell *temp6;
                    temp6 = &roomDistanceGrid[tempy+1][tempx-1];
                    binheap_insert(&heap,temp6);
                }
        }
        if(mapArray[(*temp).yloc+1][(*temp).xloc+1]!='-' || mapArray[(*temp).yloc+1][(*temp).xloc+1]!='|'){/* bottom right */
                 alt = getWeight(hardnessArray[tempy+1][tempx+1])+roomDistanceGrid[tempy][tempx].distance;
                if(roomDistanceGrid[tempy+1][tempx+1].distance>alt){
                    roomDistanceGrid[tempy+1][tempx+1].distance=alt;
                    distanceCell *temp7;
                    temp7 = &roomDistanceGrid[tempy+1][tempx+1];
                    binheap_insert(&heap,temp7);
                  }
             }
       }
}

static char getAsci(int num){
  char asci;
  if(num>=10 && num<36){
      num = num-10;
      num = num%26;
      asci='a';
      int x;
      for(x=0;x<num;x++){
          asci++;
      }
      return asci;
  }else{
          num=num-10;
          num= num%26;
          asci='A';
          int x;
          for(x=0;x<num;x++){
              asci++;
          }
          return asci;
      }
}

  void printMap(){
     for(int i=0; i<X; i++){
         for(int j=0; j<Y; j++){
          printf("%c", mapArray[i][j]);
       }
       printf("\n");
     }
  }

  void printDistanceGrid(){
    int i;
    int j;
    for(i=0;i<21;i++){
        for(j=0;j<80;j++){
            char temp = mapArray[i][j];
            if(!(temp=='-' || temp == '|' || temp=='.' || temp=='#')){
                temp=' ';
            }
            if(pcStartX==j && pcStartY==i){
                printf("0");
            }else{
                if(mapArray[i][j]=='.' || mapArray[i][j]=='#'){
                    int num = roomDistanceGrid[i][j].distance;
                        if(num<10){
                            printf("%i",num);
                        }
                        if(num<62 && num>9){
                            printf("%c",getAsci(num));
                        }
            }else{
                printf("%c",' ');
          }
       }
    }
        printf("%c\n",' ');
  }

}
void printDistanceGridPlus(){
    int i;
    int j;
    for(i=1;i<20;i++){
        for(j=1;j<79;j++){
            char temp = mapArray[i][j];
            if(pcStartX==j && pcStartY==i){
                printf("0");
            }else{
                int num = roomDistanceGrid[i][j].distance;
                if(num==10000){
                    printf("%c",mapArray[i][j]);
                }else{
                    if(num<10){
                        printf("%i",num);
                    }else{
                        if(num<62 && num>9){
                        printf("%c",getAsci(num));
                            }else{
                            char sample = mapArray[i][j];
                            if(sample!='.' && sample!='#'){
                                sample=' ';
                            }
                        printf("%c",sample);
                        }
                    }

                }

            }

        }
        printf("%c\n",' ');
    }
