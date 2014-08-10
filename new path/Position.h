
#ifndef __POSITION_H__
#define __POSITION_H__

#include <iostream>
#include "mylist.h"

typedef struct Position {
    int x;
    int y;
    int value;  // 记录地图值
    int F;      // F = G + H
    int G;
    int H;
    // int status; // 0 : NULL, 1 : open, 2 : close
    struct Position * father;
    struct list_head item;
    Position(){
        x = 0;
        y = 0;
        value = 0;
        F = 0;
        G = 0;
        H = 0;
        // status = 0;
        father = NULL;
        INIT_LIST_HEAD(&item);
    };
} Position;

#endif