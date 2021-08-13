#ifndef SNAKE_H
#define SNAKE_H

#include<vector>
#include<algorithm>
using std::pair;
typedef pair<int,int> pii;
using std::vector;

class snake
{
public:
    int last_move_direc;
    int rest;

    vector<pii>body;
    int direc;// 0: up  1:right 2:down  3:left
    int Map[43][43]; //0:nothing  1:snake  2:wall 3:apple

    snake();
    int move();  //0:nothing happened  1:eat  2:dead
    void ChangeDirec(int x);
    void clear();
};

#endif // SNAKE_H
