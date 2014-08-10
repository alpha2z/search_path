
#include <iostream>
#include "map.h"
#include <time.h> 
#include <sys/time.h> 
#include "mylist.h"


using namespace std;

typedef struct Position {
    int x;
    int y;
    int value;      // 记录地图值
    int F;      // F = G + H
    int G;
    int H;
    int status; // 0 : NULL, 1 : open, 2 : close
    struct Position * father;
    struct list_head item;
} Position;

const int size = 1000;
// static int open_length = 0;
// static int close_length = 0;
Position openList; 
Position closeList;
static int pos_rec[size][size]; // = {0};       -- 标记是否已经在open list里面了      1 open; 2 close

bool notInTheCloseList(int x, int y);
void showOpenList();

void showRecPos() {
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            if (map[i][j] == 1)
                cout<<"4,";
            else
                cout<<pos_rec[i][j]<<",";

        }
        cout<<endl;
    }
}

void showPosRecord(Position *pos) {
    pos_rec[pos->y][pos->x] = 3;
    if (pos->father) {
        cout<<"pos : x "<<pos->x<<"| y = "<<pos->y<<"| F="<<pos->F<<"| G="<<pos->G<<endl;
        showPosRecord(pos->father);
    }
}

/**
 * 1 2 3
 * 4   5
 * 6 7 8
 */

Position *getAroundPos(Position *pos, int index) {
    int x = 0;
    int y = 0;

    if (index == 1 || index == 4 || index == 6) {
        x = pos->x - 1;
    }
    else if (index == 2 || index == 7) {
        x = pos->x;
    }
    else if (index == 3 || index == 5 || index == 8) {
        x = pos->x + 1;
    }

    if (index == 1 || index == 2 || index == 3) {
        y = pos->y - 1;
    }
    else if (index == 4 || index == 5) {
        y = pos->y;
    }
    else if (index == 6 || index == 7 || index == 8) {
        y = pos->y + 1;

    }

    Position *temp_one = new Position;
    INIT_LIST_HEAD(&(temp_one->item));
    if (x < 0 || y < 0 || y >= map_height || x >= map_width) {
        temp_one->value = -1;
        return temp_one;
    }

    temp_one->x = x;
    temp_one->y = y;

    if (notInTheCloseList(x, y)) {
        temp_one->value = map[y][x];
    } else {
        temp_one->value = -1;
    }

    return temp_one;
}

void calculateFGH(Position * pos, Position * end_pos) {
    Position * father = pos->father;

    int v = 14;
    if (father->x == pos->x || father->y == pos->y)
        v = 10;

    pos->G = father->G + v;
    pos->H = abs(end_pos->y - pos->y) * 10 + abs(end_pos->x - pos->x) * 10;
    pos->F = pos->G + pos->H;
}

bool putPosIntoOpenList(Position * pos) {
    if (pos_rec[pos->y][pos->x] == 1) {
        showRecPos();
        return false;
    }

    // if (!list_empty(&(pos->item))) {    
    //     list_del(&(pos->item));
    // }

    list_add_tail(&(pos->item), &(openList.item));

    pos_rec[pos->y][pos->x] = 1;
    return true;
}

void putPosIntoCloseList(Position * pos) {
    if (pos_rec[pos->y][pos->x] == 2) {
        showRecPos();
        return;
    }

    if (!list_empty(&(pos->item))) {    
        list_del(&(pos->item));
    }

    list_add_tail(&(pos->item), &(closeList.item));
    pos_rec[pos->y][pos->x] = 2;
}

bool isInTheOpenList(Position *pos) {
    return pos_rec[pos->y][pos->x] == 1;
}

Position * getLowestFPos() {
    Position *target = NULL;
    struct list_head * pos = NULL;
    Position * tmpTarget = NULL;
    list_for_each(pos, &(openList.item)) {
        tmpTarget = list_entry(pos, Position, item);
        if (target == NULL)
            target = tmpTarget;
        else if (tmpTarget) {
            if (target->F > tmpTarget->F)
                target = tmpTarget;
        }
    }


    return target;
}

bool inTheOpenList(int x, int y) {
    return pos_rec[y][x] == 1;
}

bool notInTheCloseList(int x, int y) {
    return pos_rec[y][x] != 2;
}

void showOpenList() {
    struct list_head * pos;
    Position * tmpPos;
    int i = 0;
    list_for_each(pos, &(openList.item)) {
        tmpPos = list_entry(pos, Position, item);
        if (tmpPos) {
            cout<<"[openList]["<<i<<"] y :"<<tmpPos->y<<
            ", x = "<<tmpPos->x<<"| F : "<<tmpPos->F<<endl;
            i++;
        }

    }

}

void showpos_rec() {
    for (int i = 0; i < size; i++) {
        bool x = false;
        for (int j = 0; j < size; j++) {
            if (pos_rec[i][j] != 0) {
                x = true;
                cout<<"r["<<i<<"]["<<j<<"] = "<<pos_rec[i][j]<<"| |";
            }
        }
        if (x)
            cout<<endl;
    }
}

int main(char argc, char* argv[]) {

    INIT_LIST_HEAD(&(openList.item));
    INIT_LIST_HEAD(&(closeList.item));

    Position start_pos;
    start_pos.x = 24;
    start_pos.y = 7;
    start_pos.father = NULL;
    INIT_LIST_HEAD(&(start_pos.item));

    Position end_pos;
    end_pos.x = 2;
    end_pos.y = 10;
    INIT_LIST_HEAD(&(end_pos.item));

    end_pos.father = NULL;
    Position * cur_pos_ptr = &start_pos;

    int count = 0;
    
    struct timeval tvpre, tvafter;
    gettimeofday(&tvpre, NULL);

    putPosIntoOpenList(cur_pos_ptr);

    while(1) {
        if (cur_pos_ptr) {
            if (cur_pos_ptr->x == end_pos.x && cur_pos_ptr->y == end_pos.y) {
                showPosRecord(cur_pos_ptr);
                showRecPos();
                cout<<"count times = "<<count<<endl;
                break;
            }
        } else {
            // showRecPos();
            showOpenList();
            cout<<"system error, there is no cur_pos_ptr to find the path."<<endl;
            break;
        }

        for(int index = 1; index < 9; index++) {
            Position *tt_pos = getAroundPos(cur_pos_ptr, index);
            if (tt_pos->value == -1) {
                continue;
            }

            if (inTheOpenList(tt_pos->x, tt_pos->y)) {
                if (tt_pos->F < cur_pos_ptr->F) {
                   tt_pos->father = cur_pos_ptr;    //新找到的点，属于cur_pos_ptr所以，修改其父节点指针值。
                   calculateFGH(tt_pos, &end_pos);  //并重新计算,F，G，H值。

                }
            } else if (notInTheCloseList(tt_pos->x, tt_pos->y)) {
                if (tt_pos->value == 0) {
                    tt_pos->father = cur_pos_ptr;
                    putPosIntoOpenList(tt_pos);
                    calculateFGH(tt_pos, &end_pos);
                } else if (tt_pos->value == 1) {
                    tt_pos->father = cur_pos_ptr;
                    putPosIntoCloseList(tt_pos); 
                }
            }

        }
        putPosIntoCloseList(cur_pos_ptr);

        Position * newPos = getLowestFPos();
        cur_pos_ptr = NULL;
        cur_pos_ptr = newPos;
        count++;
    }
    gettimeofday(&tvafter, NULL);
    long cost_time = (tvafter.tv_sec - tvpre.tv_sec) * 1000+(tvafter.tv_usec - tvpre.tv_usec) / 1000;
    printf("Cost time: %ld us\n", cost_time);

}



