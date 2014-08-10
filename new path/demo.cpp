#include <time.h> 
#include <sys/time.h> 
#include <iostream>
#include "FindPath.h"
#include "map.h"

using namespace std;

int main(char argc, char* argv[]) {

    struct timeval tvpre, tvafter;
    gettimeofday(&tvpre, NULL);

	int w = map_width;
	int h = map_height;
    
    FindPath *finder = new FindPath(&map[0][0], w, h);

    char* c_x1 = argv[1];
    char* c_y1 = argv[2];
    char* c_x2 = argv[3];
    char* c_y2 = argv[4];

    int start_x = atoi(c_x1);
    int start_y = atoi(c_y1);
    int end_x = atoi(c_x2);
    int end_y = atoi(c_y2);

    finder->findThePath(start_x, start_y, end_x, end_y, false);

    gettimeofday(&tvafter, NULL);
    long cost_time = (tvafter.tv_sec - tvpre.tv_sec) * 1000+(tvafter.tv_usec - tvpre.tv_usec) / 1000;
    printf("Cost time: %ld us\n", cost_time);

}