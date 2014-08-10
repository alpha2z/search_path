
#include <iostream>
#include "map.h"

#include "list"

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
} Position;

const int size = 1000;
static int open_length = 0;
static int close_length = 0;
Position * openList[size] = {};// = new Position[size];
Position * closeList[size];
static int pos_rec[size][size]; // = {0};       -- 标记是否已经在open list里面了      1 open; 2 close

// openList 不能这样定义。出错了。


bool notInTheCloseList(int x, int y);
void showOpenList();

void showRecPos() {
    cout<<"show rec pos =============== "<<endl;
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            cout<<pos_rec[i][j]<<",";
        }
        cout<<endl;
    }
}

void showPosRecord(Position *pos) {
    printf("cur pos x:%d.", pos->x);
    printf("cur pos y:%d.\n", pos->y);
    if (pos->father) {
        showPosRecord(pos->father);
    }
}

/**
 * 1 2 3
 * 4   5
 * 6 7 8
 */

Position *getAroundPos(Position *pos, int index) {
    cout<<"0[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;
    int x = 0;
    int y = 0;

    if (index == 1 || index == 4 || index == 6) {
        x = pos->x - 1;
        cout<<"1[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

    }
    else if (index == 2 || index == 7) {
        x = pos->x;
        cout<<"2[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

    }
    else if (index == 3 || index == 5 || index == 8) {
        x = pos->x + 1;
        cout<<"3[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

    }

    if (index == 1 || index == 2 || index == 3) {
        y = pos->y - 1;
        cout<<"4[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

    }
    else if (index == 4 || index == 5) {
        y = pos->y;
        cout<<"5[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

    }
    else if (index == 6 || index == 7 || index == 8) {
        y = pos->y + 1;
        cout<<"6[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

    }

    Position *temp_one = new Position;
    cout<<"7[getAroundPos] temp_one = "<<temp_one<<endl;

    if (x < 0 || y < 0 || y >= map_height || x >= map_width) {
        temp_one->value = -1;
        cout<<"7[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

        return temp_one;
    }

    temp_one->x = x;
    cout<<"8[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

    temp_one->y = y;
    cout<<"9[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;


    if (notInTheCloseList(x, y)) {
        temp_one->value = map[y][x];
        cout<<"10[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

    } else {
        temp_one->value = -1;
        cout<<"11[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

    }
    cout<<"12[getAroundPos] pos->x = "<<pos->x<<"| pos->y = "<<pos->y<<endl;

    cout<<"[getAroundPos] result x = "<<temp_one->x<<"| y = "<<temp_one->y<<"| value = "<<temp_one->value<<endl;

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

bool removeFromClose(Position *pos) {
    cout<<"!!!removeFromClose x = "<<pos->x<<"| y = "<<pos->y<<endl;

    if (pos_rec[pos->y][pos->x] != 2)
        return false;

    bool flag = false;
    for (int i = 0; i < close_length; i++) {
        if (closeList[i]->x == pos->x && closeList[i]->y == pos->y) {
            flag = true;
            pos_rec[pos->y][pos->x] = 0;
        }
        if (flag && (i+1) < close_length)
            closeList[i] = closeList[i+1];
    }

    if (flag)
        close_length--;
    cout<<"!!!removeFromClose x = "<<pos->x<<"| y = "<<pos->y<<"| success."<<endl;

    return false;
}

bool removeFromOpenList(Position *pos) {
    cout<<"###removeFromOpenList x = "<<pos->x<<"| y = "<<pos->y<<endl;
    if (pos_rec[pos->y][pos->x] != 1)
        return false;

    bool flag = false;
    for (int i = 0; i < open_length; i++) {
        if (openList[i]->x == pos->x && openList[i]->y == pos->y) {
            flag = true;
            pos_rec[pos->y][pos->x] = 0;
        }
        if (flag && (i+1) < open_length)
            openList[i] = openList[i+1];
    }

    if (flag)
        open_length--;
    cout<<"###removeFromOpenList x = "<<pos->x<<"| y = "<<pos->y<<"| success."<<endl;
    

    return false;

}

bool putPosIntoOpenList(Position * pos) {
    cout<<"[putPosIntoOpenList] x = "<<pos->x<<"| y = "<<pos->y<<endl;

    if (pos_rec[pos->y][pos->x] == 1) {
        showRecPos();
        // cout<<" | break 2| pos->x = "<<pos->x<<"         | pos->y = "<<pos->y<<endl;
        return false;
    }

    openList[open_length] = pos;
    open_length++;
    removeFromClose(pos);
    pos_rec[pos->y][pos->x] = 1;
    cout<<"[putPosIntoOpenList] x = "<<pos->x<<"| y = "<<pos->y<<"| success."<<endl;

    return true;
}

void putPosIntoCloseList(Position * pos) {
    cout<<"[putPosIntoCloseList] x = "<<pos->x<<"| y = "<<pos->y<<endl;

    if (pos_rec[pos->y][pos->x] == 2) {
        showRecPos();
        cout<<"[putPosIntoCloseList] break 2"<<endl;
        return;
    }

    closeList[close_length] = pos;
    close_length++;
    removeFromOpenList(pos);
    pos_rec[pos->y][pos->x] = 2;

    cout<<"[putPosIntoCloseList] x = "<<pos->x<<"| y = "<<pos->y<<"| success."<<endl;

}

bool isInTheOpenList(Position *pos) {
    return pos_rec[pos->y][pos->x] == 1;
}

Position * getLowestFPos() {
    Position *target = NULL;
    for (int i = 0; i < open_length; i++) {
        if (target == NULL) {
            if (isInTheOpenList(openList[i])) {
                target = openList[i];
            }
        }
        else if (openList[i]) {         
            if (isInTheOpenList(openList[i]))
                if (target->F > openList[i]->F) {
                    target = openList[i];
                }
        }
    }
    if (target)
        cout<<"[getLowestFPos] target->x = "<<target->x<<"| target->y = "<<target->y<<endl;
    return target;
}

bool inTheOpenList(int x, int y) {
    return pos_rec[y][x] == 1;
}

bool notInTheCloseList(int x, int y) {
    return pos_rec[y][x] != 2;
}

void showOpenList() {
    for (int i = 0; i < open_length; i++)
        if (openList[i])
            cout<<"[openList]["<<i<<"] y :"<<openList[i]->y<<", x = "<<openList[i]->x<<"| F : "<<openList[i]->F<<endl;
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

    Position start_pos;
    start_pos.x = 9;
    start_pos.y = 9;
    start_pos.father = NULL;

    Position end_pos;
    end_pos.x = 29;
    end_pos.y = 11;

    // end_pos.x = 28;
    // end_pos.y = 10;

    end_pos.father = NULL;

    Position * cur_pos_ptr = &start_pos;

    int count = 0;

    // cout<<"{"<<endl;
    // for (int i = 0; i < map_height; ++i) {
    //     cout<<"{";
    //     for (int j = 0; j < map_width; ++j) {
    //         cout<<map[i][j]<<",";
    //     }
    //     cout<<"},"<<endl;
    // }
    // cout<<"};"<<endl;

    // for (int i = 0; i < map_height; i++) {
    //     for (int j = 0; j < map_width; j++) {
    //         pos_rec[i][j] = 0;
    //         cout<<pos_rec[i][j]<<",";
    //     }
    //     cout<<endl;
    // }
    
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

        cout<<"cur_pos_ptr x = "<<cur_pos_ptr->x<<"| y = "<<cur_pos_ptr->y<<endl;
        for(int index = 1; index < 9; index++) {
            cout<<"cur_pos_ptr ==== "<<cur_pos_ptr<<endl;
            cout<<"cur_pos_ptr ==== x = "<<cur_pos_ptr->x<<"| y = "<<cur_pos_ptr->y<<endl;

            Position *tt_pos = getAroundPos(cur_pos_ptr, index);

            if (tt_pos->value == -1) {
                continue;
            }

            if (inTheOpenList(tt_pos->x, tt_pos->y)) {
                if (tt_pos->F < cur_pos_ptr->F) {
                    tt_pos->father = cur_pos_ptr;
                    calculateFGH(tt_pos, &end_pos);
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

            /**
             Position tt_pos = getAroundPos(cur_pos_ptr, index);

            if (tt_pos.value == -1) {
                continue;
            }

            if (inTheOpenList(tt_pos.x, tt_pos.y)) {
                if (tt_pos.F < cur_pos_ptr->F) {
                    tt_pos.father = cur_pos_ptr;
                    calculateFGH(&tt_pos, &end_pos);
                }
            } else if (notInTheCloseList(tt_pos.x, tt_pos.y)) {
                if (tt_pos.value == 0) {
                    tt_pos.father = cur_pos_ptr;
                    putPosIntoOpenList(&tt_pos);
                    calculateFGH(&tt_pos, &end_pos);
                } else if (tt_pos.value == 1) {
                    tt_pos.father = cur_pos_ptr;
                    putPosIntoCloseList(&tt_pos); 
                }
            }
            */
        }
        putPosIntoCloseList(cur_pos_ptr);

        Position * newPos = getLowestFPos();
        cout<<"=============================next round============================="<<endl;
        cur_pos_ptr = NULL;
        cur_pos_ptr = newPos;
        count++;
        /*if (count > 100) {
            cout<<"1000 times break;"<<endl;
            // showOpenList();
            // showpos_rec();
            cout<<"open_length : "<<open_length<<endl;

            // showRecPos();
            break;
        }*/

    }
}



