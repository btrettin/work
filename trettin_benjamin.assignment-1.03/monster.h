void initMonsterLib(Map *map);
void MonsterInit(Map *map,);
Monster getMonster(yl,xl);
static int moveUp(Monster *mon);
static int moveDown(Monster *mon);
static int moveRight(Monster *mon);
static int moveLeft(Monster *mon);
static int moveTopRight(Monster *mon);
static int moveTopLeft(Monster *mon);
static int moveBottomLeft(Monster *mon);
static int moveBottomRight(Monster *mon);

int hasMonster(int xl, int yl);
typedef struct Monster;
