
#ifndef __FINDPATH_H__
#define __FINDPATH_H__

#include "Position.h"

#define MAX_SIZE 1000
class FindPath {
public:
	// FindPath(int **find_map, int w, int h);
	FindPath(int *find_map, int w, int h);
	~FindPath();

	virtual void findThePath(int start_x, int start_y, int end_x, int end_y, bool opposite);
	void showRecPos();
	void showOpenList();
	void showpos_rec();

private:
	void updatePosRecord(Position *pos);
	bool inTheOpenList(int x, int y);
	bool notInTheCloseList(int x, int y);

	/**
	 * 1 2 3
	 * 4   5
	 * 6 7 8
	 */
	Position * getAroundPos(Position *pos, int index);
	Position * getLowestFPos();

	void checkPosValid(int sx, int sy, int &ex, int &ey);
	void calculateFGH(Position * pos, Position * end_pos);
	bool putPosIntoOpenList(Position * pos);
	void putPosIntoCloseList(Position * pos);

	int size;
	int pos_rec[MAX_SIZE][MAX_SIZE];
	Position * openList;
	Position * closeList;
	int **map;
	// int *map[];
	int map_height;
	int map_width;

};


#endif




