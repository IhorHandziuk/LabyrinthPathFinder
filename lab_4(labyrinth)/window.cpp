#include "window.h"

//OpenGL visualization module
/*******************************************/
window* currThis;

void reshape(int w,int h)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(currThis->gl_le, currThis->gl_ri, currThis->gl_Bottom,
			currThis->gl_Top, currThis->gl_Near, currThis->gl_Far);
	glViewport(0,0,w,h);
}

int window::getWindowID() {
	return windowID;
}

window::window()
{
	draw = nullptr;
	key = nullptr;
	idle = nullptr;
	shape = reshape;
	gl_le = 0;
	gl_ri = 10;
	gl_Top = 0;
	gl_Bottom = 10;
	gl_Far = -10;
	gl_Near = 10;
}

window::window(void (*draw_)(void)) {
	setDisplay(draw_);
	key = nullptr;
	idle = nullptr;
	shape = reshape;
	gl_le = 0;
	gl_ri = 10;
	gl_Top = 0;
	gl_Bottom = 10;
	gl_Far = -10;
	gl_Near = 10;
}

void window::setDisplay(void (*draw_)(void)) {
	this->draw = draw_;
}

void window::setIdle(void (*idle_)(void)) {
	this->idle = idle_;
}


void window::setKeypress(void (*key_)(unsigned char, int, int)) {
	this->key = key_;
}

void window::setReshape(void (*reshape_)( int, int )) {
	this->shape = reshape_;
}

//void display()
//{
//	currThis->draw();
//	glutPostRedisplay();
//}
//
//void keypress(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 'n':
//		break;
//	};
//	glutPostRedisplay();
//}

void window::setupGlut()
{
	currThis = this;
	glutDisplayFunc(draw);
	glutKeyboardFunc(key);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
}

void window::display_start(int argc, char* argv)
{
	glutInit(&argc, &argv);
	create_window();
}

void window::create_window()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	float relation = (float)abs(gl_Top - gl_Bottom)/abs(gl_ri - gl_le);
	glutInitWindowSize(600, 600 * relation);
	glutInitWindowPosition(400, 100);
	windowID = glutCreateWindow("application");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_TEXTURE_2D);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	setupGlut();
}

void window::init(int argc, char* argv) {
	glutInit(&argc, &argv);
}

void window::display_loop() {
	glutMainLoop();
}