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
#define horizontalWall '-'
#define verticalWall '|'


char mapArray[X][Y];
struct room{
    int cornerArray[2][2];
    int connected;
};
struct room rooms[10];

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
       }
     }
   // char number = roomNumber + '0' ;
   // mapArray[randY][randX] = number;
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

    printf("start room: ");
    printf("%d",startRoom);
    printf("\n");
    printf("end room: ");
    printf("%d", endRoom);
    printf("\n");
    printf("x start: ");
    printf("%d",xStart);
    printf("\n");
    printf("y start: ");
    printf("%d", yStart);
    printf("\n");
    printf("x end: ");
    printf("%d",xEnd);
    printf("\n");
    printf("y end: ");
    printf("%d", yEnd);
    printf("\n");

    int tempXstart = xStart;
    int tempXend = xEnd;
    while(tempXstart<tempXend){
        if(mapArray[yStart][tempXstart+1] == rock){
        mapArray[yStart][tempXstart+1] = cooridor;
        }
        tempXstart++;
    }
    int tempXstart2 = xStart;
    int tempXend2 = xEnd;
    while(tempXstart2>tempXend2){
        if(mapArray[yStart][tempXstart2-1] == rock){
        mapArray[yStart][tempXstart2-1] = cooridor;
        }
        tempXstart2--;
    }

    int tempYstart = yStart;
    int tempYend = yEnd;
    while(tempYstart<tempYend){
        if(mapArray[tempYstart+1][xEnd] == rock){
        mapArray[tempYstart+1][xEnd] = cooridor;
        }
        tempYstart++;
    }
    int tempYstart2 = yStart;
    int tempYend2 = yEnd;
    while(tempYstart2>tempYend2){
        if(mapArray[tempYstart2-1][xEnd] == rock){
        mapArray[tempYstart2-1][xEnd] = cooridor;
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

        printf("Closest to Room ");
        printf("%d",i);
         printf(": ");
        printf("%d", closestRoom);
        printf("\n");
        connectRooms(i,closestRoom);
    }
}
void generateMap(){
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

int main()
{
    srand(time(0));
    generateMap();
    printMap();
}