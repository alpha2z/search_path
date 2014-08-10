
#include "FindPath.h"
#include <iostream>
// #include "mylist.h"

using namespace std;

FindPath::FindPath(int *find_map, int w, int h) {
	map_width = w;
	map_height = h;
    for (int i = 0; i < map_height; i++)
        for (int j = 0; j< map_width; j++)
            pos_rec[i][j] = 0;

    openList = new Position;
    closeList = new Position;
    map = new int*[h];
    for (int i = 0; i < h; i++)
    	map[i] = new int[w];

    for (int i = 0; i < h; i++) {
    	for (int j = 0; j < w; j++) {
    		map[i][j] = find_map[i * w + j];
            int x = i*w + j;
        }
    }

}

FindPath::~FindPath(){
	struct list_head* __pos__ = NULL;
    Position * _pos_ = NULL;
    int delet_item = 0;
    list_for_each(__pos__, &(openList->item)) {
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
    delete openList;

    __pos__ = NULL;
    _pos_ = NULL;

    list_for_each(__pos__, &(closeList->item)) {
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
    delete closeList;

    for(int i = 0; i < map_height; i++) {
         delete []map[i];
         map[i]=NULL;
    }
    delete []map;
    map = NULL;

}

void FindPath::showRecPos() {
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            if (map[i][j] == 1)
                cout<<"T,";
            else
                cout<<pos_rec[i][j]<<",";
        }
        cout<<endl;
    }
}

void FindPath::updatePosRecord(Position *pos) {
    pos_rec[pos->y][pos->x] = 3;
    if (pos->father) {
        updatePosRecord(pos->father);
    }
}

bool FindPath::inTheOpenList(int x, int y) {
    return pos_rec[y][x] == 1;
}

bool FindPath::notInTheCloseList(int x, int y) {
    return pos_rec[y][x] != 2;
}

/**
 * 1 2 3
 * 4   5
 * 6 7 8
 */

Position * FindPath::getAroundPos(Position *pos, int index) {
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
            list_for_each(_pos, &(openList->item)) {
                temp_two = list_entry(_pos, Position, item);
                if (temp_two->x == x && temp_two->y == y) {
                    temp_one = temp_two;
                    break;
                }
            }

        } else {
            temp_one = new Position;
            temp_one->x = x;
            temp_one->y = y;
            temp_one->value = map[y][x];
        }

    } 
    return temp_one;

}

void FindPath::calculateFGH(Position * pos, Position * end_pos) {
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

bool FindPath::putPosIntoOpenList(Position * pos) {

    if (!list_empty(&(pos->item))) {
        list_del(&(pos->item));
    }

    list_add_tail(&(pos->item), &(openList->item));
    pos_rec[pos->y][pos->x] = 1;
    return true;
}

void FindPath::putPosIntoCloseList(Position * pos) {

    if (!list_empty(&(pos->item))) {    
        list_del(&(pos->item));
    }

    list_add_tail(&(pos->item), &(closeList->item));
    pos_rec[pos->y][pos->x] = 2;
}



Position * FindPath::getLowestFPos() {
    Position *target = NULL;
    struct list_head * pos = NULL;
    Position * tmpTarget = NULL;
    list_for_each(pos, &(openList->item)) {
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

void FindPath::showOpenList() {
    struct list_head * pos;
    Position * tmpPos;
    list_for_each(pos, &(closeList->item)) {
        tmpPos = list_entry(pos, Position, item);
        if (tmpPos) {
            cout<<"[closeList] y :"<<tmpPos->y<<", x = "<<tmpPos->x<<"| F : "<<tmpPos->F<<endl;
        }
    }
}

void FindPath::showpos_rec() {
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

void FindPath::checkPosValid(int sx, int sy, int &ex, int &ey) {
    if (map[ey][ex] == 1) {
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
            }

            if (!flag) {
                if (velocity_y > 0) {
                    tmpY--;
                } else {
                    tmpY++;
                }
            }
            flag = !flag;
        }

        ex = tmpX;
        ey = tmpY;
    }
}

void FindPath::findThePath(int start_x, int start_y, int end_x, int end_y, bool opposite) {
    cout<<"### !!! [before] start_x : " <<start_x<<",start_y : "<<start_y<<"|end_x : " <<end_x<<", end_y : "<<end_y<<endl;
	this->checkPosValid(start_x, start_y, end_x, end_y);
    cout<<"### !!! [after]  start_x : " <<start_x<<",start_y : "<<start_y<<"|end_x : " <<end_x<<",  end_y : "<<end_y<<endl;

    Position *start_pos = new Position;
    start_pos->x = start_x;
    start_pos->y = start_y;
    start_pos->father = NULL;

    Position *end_pos = new Position;
    end_pos->x = end_x;
    end_pos->y = end_y;
    end_pos->father = NULL;

    int valid_value = 0;
    int invalid_value = 1;
    if (opposite) {
        valid_value = 1;
        invalid_value = 0;
    }
    Position * cur_pos_ptr = start_pos;

    int count = 0;
    while(1) {
        if (cur_pos_ptr) {
            if (cur_pos_ptr->x == end_pos->x && cur_pos_ptr->y == end_pos->y) {
                updatePosRecord(cur_pos_ptr);
                showRecPos();
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
    cout<<"count times = "<<count<<endl;
	delete start_pos;
	delete end_pos;

}




