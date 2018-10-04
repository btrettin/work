#include "heap.h"
struct list{
    int directions[1000];
    int size;
};
typedef struct{
     struct list directions;
     int thePlayer;
     int bigPeople;
     int dragon;
     int other;
     unsigned int characteristics : 4; /*Intel,Telapath,Tunneling,Erratic*/
     int alive;
     int xloc;
     int yloc;
     int modelNumber;
     unsigned int roundVal;
     int speed;
     int patrolMode;
     int searchLocationX;
     int searchLocationY;
     int (* moveUp)();
     int (* moveDown)();
     int (* moveRight)();
     int (* moveLeft)();
     int (* moveTopRight)();
     int (* moveTopLeft)();
     int (* moveBottomLeft)();
     int (* moveBottomRight)();
     int (* isIntelegent)();
     int (* isTelapathic)();
     int (* canTunnle)();
     int (* isErratic)();
     void (* performAction)();
     void (* scanArea)();
     void (* deconstructor)();
}Monster;
typedef struct {
  int cornerArray[2][2];
  int connected;
}room;

typedef struct{
    int distance;
    int xloc;
    int yloc;
}distanceCell;

typedef struct {
    char mapArray[21][80];
    distanceCell roomDistanceGrid[21][80];
    distanceCell wallDistanceGrid[21][80];
    unsigned char hardnessArray[21][80];
    Monster *thePlayer;
    room rooms[100];
    int numOfRooms;
    int pcStartX;
    int pcStartY;
}Map;

typedef struct corridor_path corridor_path_t;
int initMap(int numOfMonster);
void printGrid(void);
int saveGame(void);
int loadGame(void);
void printDistanceGrid(void);
void printDistanceGridPlus();
void playGame();

void initMonsterLib(Map *map, int numOfMax);
Monster* MonsterInit(Map *map,int x,int y,int isPlayer);
 int moveUp(Monster *mon);
 int moveDown(Monster *mon);
 int moveRight(Monster *mon);
 int moveLeft(Monster *mon);
 int moveTopRight(Monster *mon);
 int moveTopLeft(Monster *mon);
 int moveBottomLeft(Monster *mon);
 int moveBottomRight(Monster *mon);
 int isIntelegent(Monster *mon);
 int isTelapathic(Monster *mon);
 int canTunnle(Monster *mon);
 int isErratic(Monster *mon);
int hasMonster(int xl, int yl);
void deconstructor(Monster *m);
void performAction(Monster *mon);
void scanArea(Monster *mon);
int getPCX();
int getPCY();
int32_t compare_monster(const void *key,const void *with);
void analyzeDistances(void);
room* pointContains(int y,int x);
extern Monster* monsterArray[21][80];
extern Map *m;
