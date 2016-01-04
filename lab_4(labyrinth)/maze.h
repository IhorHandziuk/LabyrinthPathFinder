#pragma once
#include "window.h"
#define MAX_CELLS 10000

enum direction {up, down, le, ri};

#define OPPOSITE(way) ((way) % 2 == 0) ? way+1 : way-1

struct Wall {
	float x1, y1, x2, y2;
	bool painted;
	Wall() {painted = true;}
};

struct Cell {
	bool visited;
	Wall dir[4];
	Cell* neig[4];
	Cell()  {
		visited = false;
		for (int way = 0; way < 4; way++)
			neig[way] = nullptr;
	}
};
 
class Labyrinth {
	friend void display();
public:
	Labyrinth(size_t, size_t, size_t exitsNum_ = 1);

	void filling_maze();
	void build_maze();
	void solve_maze();
	
	window wndw;
private:
	//width and height in cells
	size_t crossV, crossH, cellsNum;
	size_t exitsNum;

	Cell maze[MAX_CELLS]; 
	
	//distance betwen different cells
	float stepV, stepH;
};

