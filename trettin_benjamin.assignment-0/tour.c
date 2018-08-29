#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define N 5

int singleTour[25];
int explored[N][N];

//X-Y combination for the possible moves of the knight
int col[] = {1, 2,  2,  1, -1, -2, -2, -1, 1};
int row[] = {2, 1, -1, -2, -2, -1,  1,  2 , 2};

//Checks if a knight move will land on a square within the board
bool isASquare(int x, int y)
{
    if (x<0 || x>= N || y<0 || y>= N){
        return false;
    }
    return true;
}

//Finds and prints all possible tours for a single square
void Tour(int x, int y, int position)
{
    explored[x][y] = position;

    //Prints a single tour if all squares are explored
    if (position >= N*N){
        for (int i=0; i<N; i++){
            for (int j=0; j<N; j++){
                singleTour[(explored[i][j])-1] = (5*i)+j+1;
            }
        }
         for (int i = 0; i < 25; i++){
             printf("%d", singleTour[i]);
             if( i != 24){
             printf(",");
             }
         }
        printf("\n");
        explored[x][y] = 0;
        return;
    }

    //Calls tour if it is a valid and unexplored square
    for (int k=0; k<8; k++){
        int xNew = x + row[k];
        int yNew = y + col[k];
        if (isASquare(xNew, yNew) && !explored[xNew][yNew]){
            Tour(xNew, yNew, position + 1);
        }
    }
    explored[x][y] = 0;
}

//Runs tour for each square in the 5x5 board
void FullTour()
{
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            Tour(i,j,1);
        }
    }
}

int main()
{
    //set explored array to 0 for each position
    memset(explored, 0, sizeof explored);
    FullTour();
}
