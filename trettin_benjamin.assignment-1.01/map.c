#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <endian.h>maake
#include <stdint.h>
#include <sys/stat.h>

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
struct room rooms[100];
int pcStartX;
int pcStartY;

struct Room {
    int topLeft[2];
    int topright[2];
    int bottomLeft[2];
    int bottomRight[2];
};

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

static Room createRoomFile(int xUperLeft,int xSize,int yUperLeft,int ySize){
    Room r;
    r.topLeft[0]=xUperLeft;
    r.topLeft[1]=yUperLeft;
    r.bottomLeft[0]=xUperLeft+xSize;
    r.bottomLeft[1]=yUperLeft;
    r.topright[0]=xUperLeft;
    r.topright[1]=yUperLeft+ySize;
    r.bottomRight[0]=xUperLeft+xSize;
    r.bottomRight[1]=yUperLeft+ySize;
    return r;
}

static Room* createRoom(void){
    char done ='n';
    Room *room;
    room = (struct Room*)malloc(sizeof(struct Room));
    while(done!='y'){
        int height = rand();
        height = height % 7;
        if(height<5){
            height=height+5;
        }
        int width = rand();
        width = width % 7;
        if(width<5){
            width=width+5;
        }
        int locx = rand() % 19;
        locx++;
        int locy = rand() % 78;
        locy++;
        if((width+locy)<78){
            if((height+locx)<19){
                done='y';
            }
        }
        (*room).topLeft[0]=locx;
        (*room).topLeft[1]=locy;
        (*room).topright[0]=locx;
        (*room).topright[1]=locy+width;
        (*room).bottomLeft[0]=locx+height;
        (*room).bottomLeft[1]=locy;
        (*room).bottomRight[0]=locx+height;
        (*room).bottomRight[1]=locy+width;
    }

    return room;
}

int saveGame(){
    FILE *f;
    char *home = getenv("HOME");
    strcat(home,"/.rlg327/");
    strcat(home,"Dungeon");
    f = fopen(home,"w");
    if(!f){
      printf("could not write file\n");
        return 1;
    }
    char *title = "RLG327";
    int version = 0;
    version = htobe32(version);
    int sizeint;
    sizeint = 1694;
    sizeint = sizeint + 4 * (*m).numOfRooms;
    sizeint = htobe32(sizeint);
    fwrite(title,1,6,f);
    fwrite(&version,4,1,f);
    fwrite(&sizeint,4,1,f);
    fwrite((*m).hardness,1,1680,f);

    int a;


    for (a=0; a<(*m).numOfRooms; a++){

        int topLeftX;
        int xWidth;
        int topLeftY;
        int yWidth;
         topLeftY =  (*m).rooms[a].topLeft[1];
         yWidth = (*m).rooms[a].bottomLeft[0]-(*m).rooms[a].topLeft[0];
         topLeftX = (*m).rooms[a].topLeft[0];
         xWidth = (*m).rooms[a].topright[1]-(*m).rooms[a].topLeft[1];
        fwrite(&topLeftY, 1, 1, f);
        fwrite(&xWidth, 1, 1, f);
        fwrite(&topLeftX, 1, 1, f);
        fwrite(&yWidth, 1, 1, f);
        //Room r = createRoomFile(topLeftY,yWidth,topLeftX,xWidth);

    }

    fclose(f);
    return 0;
}



int size;
int loadGame(){
    m = (struct Map*)malloc(sizeof(struct Map));
    initBorder();
    FILE *f;
    char title[6];
    int version;
    char *home;
    home = (char*) malloc(sizeof(char)*100);
    strcpy(home,getenv("HOME"));
    strcat(home,"/.rlg327/");
    strcat(home,"Dungeon");
    f= fopen(home,"r");
    if(!f){
        printf("cant open file");
        return 1;
    }
    unsigned char hardnessModel[21][80];
    fread(title,1,6,f);
    fread(&version,4,1,f);
    fread(&size,4,1,f);
    version=be32toh(version);

    fread(hardnessModel,1,1680,f);
    int az;
    int hg;
    for(az=0;az<21;az++){
        for(hg=0;hg<80;hg++){
            (*m).hardness[az][hg]=hardnessModel[az][hg];
        }
    }
    size = be32toh(size);
    int max = (size-1694)/4;
    int t;
    int j;
    for(t=0;t<21;t++){
        for(j=0;j<80;j++){
            if(hardnessModel[t][j]==0){
                (*m).grid[t][j]='#';
            }
        }
    }
    int a;
    (*m).numOfRooms=0;
    for (a=0; a<max; a++){
        uint8_t topLeftX;
        uint8_t xWidth;
        uint8_t topLeftY;
        uint8_t yWidth;
        fread(&topLeftX, sizeof(topLeftX), 1, f);
        fread(&xWidth, sizeof(xWidth), 1, f);
        fread(&topLeftY, sizeof(topLeftY), 1, f);
        fread(&yWidth, sizeof(yWidth), 1, f);
        Room r = createRoomFile(topLeftY,yWidth,topLeftX,xWidth);
        (*m).rooms[a]=r;
        (*m).numOfRooms=(*m).numOfRooms+1;

        addRoom(r);
    }


    fclose(f);


return 0;
}
