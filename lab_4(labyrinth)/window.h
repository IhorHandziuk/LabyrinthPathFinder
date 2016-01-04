#pragma once
#include <GL/freeglut.h>
using namespace std;

void reshape(int w,int h);

class window
{
public:	
	window();
	window(void (*)(void));
	void display_start(int argc = 0, char* argv = "");
	static void init (int argc = 0, char* argv = "");
	void create_window();
	static void display_loop();
	void (*draw)(void);
	void (*idle)(void);
	void (*key)(unsigned char, int, int);
	void (*shape)( int, int );
	void setDisplay(void (*)(void));
	void setIdle(void (*)(void));
	void setKeypress(void (*)(unsigned char, int, int));
	void setReshape(void (*)( int, int ));
	int getWindowID();
	int gl_le, gl_ri, gl_Top, gl_Bottom, gl_Near, gl_Far;
private:
	int windowID;
	//pair<int, int> resolution;
	void setupGlut();
};

