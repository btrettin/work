#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define X 21
#define Y 80
#define floor '.'
#define rock ' '
#define cooridor '#'
#define pc '@'
#define horizontalWall '-'
#define verticalWall '|'

int hardnessArray[X][Y];
char mapArray[X][Y];
struct room{
    int cornerArray[2][2];
    int connected;
};
struct room rooms[10];
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
void generateMap(){
   for(int i=0; i<X; i++){
       for(int j=0; j<Y; j++){
        if(i==0 || i==X-1){
        mapArray[i][j] = horizontalWall;
        hardnessArray[i][j] = 1;
        }
        else if(j==0 || j==Y-1){
        mapArray[i][j] = verticalWall;
        hardnessArray[i][j] = 1;
        }
        else{
        mapArray[i][j] = rock;
        hardnessArray[i][j] = 1;
        }
     }
   }
    int mostRooms = 10;
    int leastRooms = 5;
    int numOfRooms = (rand() % (mostRooms-leastRooms + 1)) + leastRooms;
   for(int i=0; i<numOfRooms; i++){
       generateRoom(i);
   }
   generateCooridors(numOfRooms);
}
void printMap(){
   for(int i=0; i<X; i++){
       for(int j=0; j<Y; j++){
        printf("%c", mapArray[i][j]);
     }
     printf("\n");
   }
}
int initMap()
{
    generateMap();
    printMap();
    return 0;
}
