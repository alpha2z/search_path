
#include <iostream>
#include "map.h"
#include <time.h> 
#include <sys/time.h> 
#include "mylist.h"


using namespace std;

typedef struct Position {
    int x;
    int y;
    int value;  // 记录地图值
    int F;      // F = G + H
    int G;
    int H;
    int status; // 0 : NULL, 1 : open, 2 : close
    struct Position * father;
    struct list_head item;
    Position(){
        x = 0;
        y = 0;
        value = 0;
        F = 0;
        G = 0;
        H = 0;
        status = 0;
        father = NULL;
        INIT_LIST_HEAD(&item);
    };
} Position;

const int size = 1000;
Position openList; 
Position closeList;
static int pos_rec[size][size]; // 标记是否已经在open list里面了  1 open; 2 close
int new_item = 0;

bool notInTheCloseList(int x, int y);
void showOpenList();

void showRecPos() {
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            // if (map[i][j] == 1)
                // cout<<"4,";
            // else
                cout<<pos_rec[i][j]<<",";
        }
        cout<<endl;
    }
}

void showPosRecord(Position *pos) {
    pos_rec[pos->y][pos->x] = 3;
    if (pos->father) {
        // cout<<"pos : x "<<pos->x<<"| y = "<<pos->y<<"| F="<<pos->F<<"| G="<<pos->G<<endl;
        showPosRecord(pos->father);
    }
}

bool inTheOpenList(int x, int y) {
    return pos_rec[y][x] == 1;
}

bool notInTheCloseList(int x, int y) {
    return pos_rec[y][x] != 2;
}

/**
 * 1 2 3
 * 4   5
 * 6 7 8
 */

Position * getAroundPos(Position *pos, int index) {
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

    if (x < 0 || y < 0 || y >= map_height || x >= map_width) {
        return NULL;
    }

    Position *temp_one = NULL;

    if (notInTheCloseList(x, y)) {
        if (inTheOpenList(x, y)) {
            struct list_head * _pos = NULL;
            Position * temp_two = NULL;
            list_for_each(_pos, &(openList.item)) {
                temp_two = list_entry(_pos, Position, item);
                if (temp_two->x == x && temp_two->y == y) {
                    temp_one = temp_two;
                    break;
                }
            }

        } else {
            temp_one = new Position;
            new_item++;

            temp_one->x = x;
            temp_one->y = y;
            temp_one->value = map[y][x];
        }

    } 
    return temp_one;

}

void calculateFGH(Position * pos, Position * end_pos) {
    Position * father = pos->father;

    int v = 14;
    if (father->x == pos->x || father->y == pos->y){
        v = 10;
    } else { 
        int x1, x2;
        int y1, y2;
        x1 = pos->x;
        x2 = father->x;

        y1 = father->y;
        y2 = pos->y;

        if (map[y1][x1] == 1 && map[y2][x2] == 1) {
            v = 10000;
        }
        else if (map[y1][x1] == 1 || map[y2][x2] == 1) {
            v = 21;
        }
    }

    pos->G = father->G + v;
    pos->H = abs(end_pos->y - pos->y) * 10 + abs(end_pos->x - pos->x) * 10;
    pos->F = pos->G + pos->H;
}

bool putPosIntoOpenList(Position * pos) {

    if (!list_empty(&(pos->item))) {
        list_del(&(pos->item));
    }

    list_add_tail(&(pos->item), &(openList.item));
    // pos->status = 1;
    pos_rec[pos->y][pos->x] = 1;
    return true;
}

void putPosIntoCloseList(Position * pos) {

    if (!list_empty(&(pos->item))) {    
        list_del(&(pos->item));
    }

    list_add_tail(&(pos->item), &(closeList.item));
    // pos->status = 2;
    pos_rec[pos->y][pos->x] = 2;
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

void showOpenList() {
    cout<<"show open list."<<endl;
    struct list_head * pos;
    Position * tmpPos;
    list_for_each(pos, &(closeList.item)) {
        tmpPos = list_entry(pos, Position, item);
        if (tmpPos) {
            cout<<"[closeList] y :"<<tmpPos->y<<", x = "<<tmpPos->x<<"| F : "<<tmpPos->F<<endl;
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

void checkPosValid(int sx, int sy, int &ex, int &ey) {
    if (map[ey][ex] == 1) {
        cout<<"invalid"<<endl;
        int velocity_x = ex - sx;
        int velocity_y = ey - sy;
        int tmpX = ex;
        int tmpY = ey;

        bool flag = true;
        while(map[tmpY][tmpX] == 1) {
            if (flag) {
                if (velocity_x > 0){
                    tmpX--;
                } else {
                    tmpX++;
                }
                cout<<"flag1"<<endl;
            }
            if (!flag) {
                if (velocity_y > 0) {
                    tmpY--;
                } else {
                    tmpY++;
                }
                cout<<"flag2"<<endl;
            }
            flag = !flag;
        }
        ex = tmpX;
        ey = tmpY;
    }
}

void findThePath(Position *cur_pos_ptr, Position *end_pos, bool opposite) {
    int valid_value = 0;
    int invalid_value = 1;
    if (opposite) {
        valid_value = 1;
        invalid_value = 0;
    }

    int count = 0;
    while(1) {
        if (cur_pos_ptr) {
            if (cur_pos_ptr->x == end_pos->x && cur_pos_ptr->y == end_pos->y) {
                showPosRecord(cur_pos_ptr);
                showRecPos();
                cout<<"count times = "<<count<<endl;
                break;
            }
        } else {
            cout<<"system error, there is no cur_pos_ptr to find the path."<<endl;
            break;
        }

        for(int index = 1; index < 9; index++) {
            Position *tt_pos = getAroundPos(cur_pos_ptr, index);
            if (!tt_pos) {
                continue;
            }

            if (inTheOpenList(tt_pos->x, tt_pos->y)) {
                if (tt_pos->G > cur_pos_ptr->G) {
                   tt_pos->father = cur_pos_ptr;   //新找到的点，属于cur_pos_ptr所以，修改其父节点指针值。
                   calculateFGH(tt_pos, end_pos);  //并重新计算,F，G，H值。
                }

            } else if (notInTheCloseList(tt_pos->x, tt_pos->y)) {
                if (tt_pos->value == valid_value) {
                    tt_pos->father = cur_pos_ptr;
                    putPosIntoOpenList(tt_pos);
                    calculateFGH(tt_pos, end_pos);
                } else if (tt_pos->value == invalid_value) {
                    tt_pos->father = cur_pos_ptr;
                    putPosIntoCloseList(tt_pos); 
                }
            } else
                cout<<"leak ..."<<endl;
        }

        putPosIntoCloseList(cur_pos_ptr);
        cur_pos_ptr = NULL;

        Position * newPos = getLowestFPos();
        cur_pos_ptr = newPos;
        count++;
    }
}

int main(char argc, char* argv[]) {
    // cout<<"argc : " <<argc<<endl;
    // cout<<"argv[0] : " <<argv[0]<<endl;
    
    char* c_x1 = argv[1];
    char* c_y1 = argv[2];
    char* c_x2 = argv[3];
    char* c_y2 = argv[4];

    int start_x = atoi(c_x1);
    int start_y = atoi(c_y1);
    int end_x = atoi(c_x2);
    int end_y = atoi(c_y2);

    cout<<"### !!! [before] start_x : " <<start_x<<", start_y : "<<start_y<<endl;
    cout<<"### !!! [before]   end_x : " <<end_x<<",     end_y : "<<end_y<<endl;

    // checkPosValid(start_x, start_y);
    checkPosValid(start_x, start_y, end_x, end_y);

    cout<<"### !!! [after]  start_x : " <<start_x<<", start_y : "<<start_y<<endl;
    cout<<"### !!! [after]    end_x : " <<end_x<<",     end_y : "<<end_y<<endl;

    struct timeval tvpre, tvafter;
    gettimeofday(&tvpre, NULL);

    for (int i = 0; i < map_height; i++)
        for (int j = 0; j< map_width; j++)
            pos_rec[i][j] = 0;

    INIT_LIST_HEAD(&(openList.item));
    INIT_LIST_HEAD(&(closeList.item));

    Position *start_pos = new Position;
    new_item++;
    start_pos->x = start_x; //38;
    start_pos->y = start_y; //20;
    start_pos->father = NULL;

    Position *end_pos = new Position;
    end_pos->x = end_x; //2;
    end_pos->y = end_y; //10;
    end_pos->father = NULL;

    // Position * cur_pos_ptr = start_pos;
    // Position * pre_cur_pos_ptr = NULL;
    // putPosIntoOpenList(start_pos);

    findThePath(start_pos, end_pos, false);

    // while(1) {
    //     if (cur_pos_ptr) {
    //         if (cur_pos_ptr->x == end_pos->x && cur_pos_ptr->y == end_pos->y) {
    //             showPosRecord(cur_pos_ptr);
    //             // showRecPos();
    //             cout<<"count times = "<<count<<endl;
    //             break;
    //         }
    //     } else {
    //         // showPosRecord(pre_cur_pos_ptr);
    //         // showRecPos();
    //         cout<<"system error, there is no cur_pos_ptr to find the path."<<endl;
    //         break;
    //     }

    //     for(int index = 1; index < 9; index++) {
    //         Position *tt_pos = getAroundPos(cur_pos_ptr, index);
    //         if (!tt_pos) {
    //             continue;
    //         }

    //         if (inTheOpenList(tt_pos->x, tt_pos->y)) {
    //             if (tt_pos->G > cur_pos_ptr->G) {
    //                tt_pos->father = cur_pos_ptr;   //新找到的点，属于cur_pos_ptr所以，修改其父节点指针值。
    //                calculateFGH(tt_pos, end_pos);  //并重新计算,F，G，H值。
    //             }

    //         } else if (notInTheCloseList(tt_pos->x, tt_pos->y)) {
    //             if (tt_pos->value == 0) {
    //                 tt_pos->father = cur_pos_ptr;
    //                 putPosIntoOpenList(tt_pos);
    //                 calculateFGH(tt_pos, end_pos);
    //             } else if (tt_pos->value == 1) {
    //                 tt_pos->father = cur_pos_ptr;
    //                 putPosIntoCloseList(tt_pos); 
    //             }
    //         } else
    //             cout<<"leak ..."<<endl;

    //     }
    //     // pre_cur_pos_ptr = cur_pos_ptr;
    //     putPosIntoCloseList(cur_pos_ptr);

    //     Position * newPos = getLowestFPos();
    //     cur_pos_ptr = NULL;
    //     cur_pos_ptr = newPos;
    //     count++;
    // }

    struct list_head* __pos__ = NULL;
    Position * _pos_ = NULL;
    int delet_item = 0;
    list_for_each(__pos__, &(openList.item)) {
        _pos_ = list_entry(__pos__, Position, item);
        if (_pos_) {
            Position* father = _pos_->father;

            if (!list_empty(&(_pos_->item))) {
                list_del(&(_pos_->item));
            }
            delet_item++;
            delete _pos_;
            _pos_ = NULL;
        }
    }

    __pos__ = NULL;
    _pos_ = NULL;

    list_for_each(__pos__, &(closeList.item)) {
        _pos_ = list_entry(__pos__, Position, item);
        if (_pos_) {
            Position* father = _pos_->father;

            if (!list_empty(&(_pos_->item))) {
                list_del(&(_pos_->item));
            }
            delet_item++;
            delete _pos_;
            _pos_ = NULL;
        }
    }
    cout<<"delet_item : " <<delet_item<<endl;
    cout<<"new_item : " <<new_item<<endl;

    gettimeofday(&tvafter, NULL);
    long cost_time = (tvafter.tv_sec - tvpre.tv_sec) * 1000+(tvafter.tv_usec - tvpre.tv_usec) / 1000;
    printf("Cost time: %ld us\n", cost_time);

}



