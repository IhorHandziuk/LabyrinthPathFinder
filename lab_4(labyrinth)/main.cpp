#include <iostream>
#include "maze.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
using namespace std;

extern Labyrinth* c;

int main() {
	Labyrinth myLab(7, 7);
	myLab.filling_maze();
	myLab.build_maze();
	c = &myLab;
	myLab.wndw.display_start();
	myLab.wndw.display_loop();
	system("pause");
}
