#include "snake.h"
#include<cmath>

snake::snake()
{
    body.push_back(pii(20,20));
    body.push_back(pii(20,21));
    for(int i=1;i<=40;++i) for(int j=1;j<=40;++j)
        Map[i][j]=0;
    Map[20][20] = 1; Map[20][21] = 1;
    for(int i=0;i<=41;++i)
    {
        Map[0][i] = 2;
        Map[41][i] = 2;
        Map[i][0] = 2;
        Map[i][41] = 2;
    }
    last_move_direc = -8;
    direc = 0;
    rest = 0;
}

const int move1[4]= {0,1,0,-1},move2[4]={-1,0,1,0};
int snake::move()
{
    int hx = body[0].first, hy= body[0].second;
    int nx= hx+move1[direc], ny=hy+move2[direc];
    body.insert(body.begin(),pii(nx,ny));
    pii Tail = body.back();
    body.pop_back();

    last_move_direc = direc;
    if(Map[nx][ny] == 1 || Map[nx][ny]==2)
    {
        return 2; //dead
    }
    if(Map[nx][ny]==3)
    {
        rest += 3;
        rest--;
        body.push_back(Tail);
        Map[nx][ny] = 1;
        return 1;
    }
    if(rest)
    {
        rest--;
        body.push_back(Tail);
        Map[nx][ny] = 1;
        return 0;
    }
    if(Map[nx][ny]==0)
    {
        Map[nx][ny]=1;
        Map[Tail.first][Tail.second] = 0;
        return 0;
    }
}

void snake::ChangeDirec(int x)
{
    if(abs(last_move_direc - x) == 2) return ;
    direc = x;
}
void snake::clear()
{
    body.clear();
    body.push_back(pii(20,20));
    body.push_back(pii(20,21));
    for(int i=1;i<=40;++i) for(int j=1;j<=40;++j)
        Map[i][j]=0;
    Map[20][20] = 1; Map[20][21] = 1;
    last_move_direc = -8;
    direc = 0;
}
