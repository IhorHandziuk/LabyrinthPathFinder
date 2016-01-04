#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <stdio.h>
#include "maze.h"
#include <stack>
#include <vector>
#include <set>
#include <time.h>

#define GL_CLAMP_TO_EDGE 0x812F

Labyrinth* c;

//void display ()
//{
//	glClear(GL_COLOR_BUFFER_BIT);								
//	glColor3f(0.0f,0.0f,0.0f);
//	glPointSize(2.0);
//	glDisable(GL_LINE_SMOOTH);
//	glLineWidth(2.0);
//	glBegin(GL_LINES);
//	for (GLint i = 0; i < c->cellsNum; i++) {
//		for (GLint way = 0; way < 4; way++) {
//			if (c->maze[i].dir[way].painted) {
//				glVertex2f(c->maze[i].dir[way].x1, c->maze[i].dir[way].y1);
//				glVertex2f(c->maze[i].dir[way].x2, c->maze[i].dir[way].y2);
//			}
//		}
//	}
//	glEnd();
//	glColor4f(1.0f,0.0f,0.0f, 0.8);
//	glBegin(GL_QUADS);
//	for (GLint i = 0; i < c->cellsNum; i++)
//		if (!c->maze[i].visited)
//		{
//			glVertex2f(c->maze[i].dir[up].x1, c->maze[i].dir[up].y1);
//			glVertex2f(c->maze[i].dir[up].x2, c->maze[i].dir[up].y2);
//			glVertex2f(c->maze[i].dir[ri].x2, c->maze[i].dir[ri].y2);
//			glVertex2f(c->maze[i].dir[le].x2, c->maze[i].dir[le].y2);
//		}
//	glEnd();
//	glutSwapBuffers();
//}

GLuint LoadTexture(string name)
{
	    sf::Image image;
        if (!image.loadFromFile(name))
            return EXIT_FAILURE;
		image.flipVertically();
		GLuint texture=0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		return texture;
}

void createBox(GLuint skybox[],float size)
{
	    glBindTexture(GL_TEXTURE_2D, skybox[0]);
	    glBegin(GL_QUADS);
		    //front
            glTexCoord2f(0, 0);   glVertex3f(-size, -size, -size);
            glTexCoord2f(1, 0);   glVertex3f(size,  -size, -size);
            glTexCoord2f(1, 1);   glVertex3f( size,  size, -size);
            glTexCoord2f(0, 1);   glVertex3f( -size, size, -size);
        glEnd();
		
	    glBindTexture(GL_TEXTURE_2D, skybox[1]);
	    glBegin(GL_QUADS);
			//back
            glTexCoord2f(0, 0); glVertex3f(size, -size, size);
            glTexCoord2f(1, 0); glVertex3f(-size,  -size, size);
            glTexCoord2f(1, 1); glVertex3f( -size,  size, size);
            glTexCoord2f(0, 1); glVertex3f( size, size, size);
        glEnd();

		glBindTexture(GL_TEXTURE_2D, skybox[2]);
	    glBegin(GL_QUADS);
			//left
            glTexCoord2f(0, 0); glVertex3f(-size, -size,  size);
            glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
            glTexCoord2f(1, 1); glVertex3f(-size,  size, -size);
            glTexCoord2f(0, 1); glVertex3f(-size,  size,  size);
        glEnd();

		glBindTexture(GL_TEXTURE_2D, skybox[3]);
	    glBegin(GL_QUADS);
			//right
            glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
            glTexCoord2f(1, 0); glVertex3f(size,  -size, size);
            glTexCoord2f(1, 1); glVertex3f(size,  size,  size);
            glTexCoord2f(0, 1); glVertex3f(size, size,  -size);
        glEnd();

		glBindTexture(GL_TEXTURE_2D, skybox[4]);
	    glBegin(GL_QUADS);
			//bottom
            glTexCoord2f(0, 0); glVertex3f(-size, -size,  size);
            glTexCoord2f(1, 0); glVertex3f(size, -size, size);
            glTexCoord2f(1, 1); glVertex3f( size, -size, -size);
            glTexCoord2f(0, 1); glVertex3f( -size, -size,  -size);
        glEnd();

	    glBindTexture(GL_TEXTURE_2D, skybox[5]);
	    glBegin(GL_QUADS);
			//top  		
            glTexCoord2f(0, 0); glVertex3f(-size, size,  -size);
            glTexCoord2f(1, 0); glVertex3f(size, size, -size);
            glTexCoord2f(1, 1); glVertex3f( size, size, size);
            glTexCoord2f(0, 1); glVertex3f( -size, size,  size);	     
        glEnd();
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.7,0.7f,0.7f);
	float  x1, x2, y1, y2, thickX, thickY;
	GLuint wall = LoadTexture("resources/wall.png");
	GLuint floor = LoadTexture("resources/floor.png");
			
	glBindTexture(GL_TEXTURE_2D, floor);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glNormal3f(1, 1, 0); glVertex2f(c->wndw.gl_le, c->wndw.gl_Top);
	glTexCoord2f(1, 0); glNormal3f(0, 0, 1); glVertex2f(c->wndw.gl_ri, c->wndw.gl_Top);
	glTexCoord2f(1, 1); glNormal3f(0, 0, 1); glVertex2f(c->wndw.gl_ri, c->wndw.gl_Bottom);
	glTexCoord2f(0, 1); glNormal3f(0, 0, 1); glVertex2f(c->wndw.gl_le, c->wndw.gl_Bottom);
	glEnd();
	for (GLint i = 0; i < c->cellsNum; i++) {
		for (GLint way = 0; way < 4; way++) 
		{
			x1 = c->maze[i].dir[way].x1;
			x2 = c->maze[i].dir[way].x2;
			y1 = c->maze[i].dir[way].y1;
			y2 = c->maze[i].dir[way].y2;
			if (c->maze[i].dir[way].painted) 
			{
				if (y1 == y2) {thickX = 0; thickY = c->stepH/5;}
				else {thickX = c->stepV/5; thickY = 0;}
				glBindTexture(GL_TEXTURE_2D, wall);
				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex3f(x1, y1, 2);
				glTexCoord2f(1, 0); glVertex3f(x2, y2, 2);
				glTexCoord2f(1, 1); glVertex3f(x2, y2, 0);
				glTexCoord2f(0, 1); glVertex3f(x1, y1, 0);
				glEnd();

				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex3f(x1 - thickX, y1 - thickY, 2);
				glTexCoord2f(1, 0); glVertex3f(x2 - thickX, y2 - thickY, 2);
				glTexCoord2f(1, 1); glVertex3f(x2 - thickX, y2 - thickY, 0);
				glTexCoord2f(0, 1); glVertex3f(x1 - thickX, y1 - thickY, 0);
				glEnd();

				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex3f(x2, y2, 0);
				glTexCoord2f(1, 0); glVertex3f(x1, y1, 0);
				glTexCoord2f(1, 1); glVertex3f(x1 - thickX, y1 - thickY, 0);
				glTexCoord2f(0, 1); glVertex3f(x2 - thickX, y2 - thickY, 0);
				glEnd();

				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex3f(x1 - thickX, y1 - thickY, 2);
				glTexCoord2f(1, 0); glVertex3f(x2 - thickX, y2 - thickY, 2);
				glTexCoord2f(1, 1); glVertex3f(x2, y2, 2);
				glTexCoord2f(0, 1); glVertex3f(x1, y1, 2);
				glEnd();

				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex3f(x1 - thickX, y1 - thickY, 2);
				glTexCoord2f(1, 0);  glVertex3f(x1, y1, 2);
				glTexCoord2f(1, 1);glVertex3f(x1, y1, 0);
				glTexCoord2f(0, 1); glVertex3f(x1 - thickX, y1 - thickY, 0);
				glEnd();

				glBegin(GL_QUADS);
				glTexCoord2f(0, 0); glVertex3f(x2 - thickX, y2 - thickY, 2);
				glTexCoord2f(1, 0); glVertex3f(x2, y2, 2);
				glTexCoord2f(1, 1); glVertex3f(x2, y2, 0);
				glTexCoord2f(0, 1); glVertex3f(x2 - thickX, y2 - thickY, 0);
				glEnd();

			}
		}
	}
	glColor4f(1.0f,0.0f,0.0f, 0.8);
	glBegin(GL_QUADS);
	for (GLint i = 0; i < c->cellsNum; i++)
		if (!c->maze[i].visited)
		{
			glVertex3f(c->maze[i].dir[up].x1, c->maze[i].dir[up].y1, 0);
			glVertex3f(c->maze[i].dir[up].x2, c->maze[i].dir[up].y2, 0);
			glVertex3f(c->maze[i].dir[ri].x2, c->maze[i].dir[ri].y2, 0);
			glVertex3f(c->maze[i].dir[le].x2, c->maze[i].dir[le].y2, 0);
		}
	glEnd();
	glutSwapBuffers();
}

void keypress(unsigned char key, int x, int y)
{
	switch (key) {
	case '/':
		c->solve_maze();
		break;
	case 'w':
		glTranslatef(5, 5, 0);
		glRotatef(1, 1, 0, 0);
		glTranslatef(-5, -5, 0);
		break;
	case 's':
		glTranslatef(5, 5, 0);
		glRotatef(1, -1, 0, 0);
		glTranslatef(-5, -5, 0);
		break;
	case 'e':
		glTranslatef(5, 5, 0);
		glRotatef(1, 0, 1, 0);
		glTranslatef(-5, -5, 0);
		break;
	case 'q':
		glTranslatef(5, 5, 0);
		glRotatef(1, 0, -1, 0);
		glTranslatef(-5, -5, 0);
		break;
	case 'd':
		glTranslatef(5, 5, 0);
		glRotatef(1, 0, 0, 1);
		glTranslatef(-5, -5, 0);
		break;
	case 'a':
		glTranslatef(5, 5, 0);
		glRotatef(1, 0, 0, -1);
		glTranslatef(-5, -5, 0);
		break;
	case 'p':
		glTranslatef(5, 5, 0);
		glScalef(0.9, 0.9, 0.9);
		glTranslatef(-5, -5, 0);
		break;	
	case 'l':
		glTranslatef(5, 5, 0);
		glScalef(1.1, 1.1, 1.1);
		glTranslatef(-5, -5, 0);
		break;
	};
	glutPostRedisplay();
}

Labyrinth::Labyrinth(size_t cellsV, size_t cellsH, size_t exitsNum_) {
	crossV = cellsV + 1;
	crossH = cellsH + 1;
	exitsNum = exitsNum_;
	cellsNum = cellsV * cellsH;
	stepH = abs(wndw.gl_Top - wndw.gl_Bottom)/(GLfloat)(crossH+1);
	stepV = abs(wndw.gl_ri - wndw.gl_le)/(GLfloat)(crossV+1);
	wndw.setDisplay(display);
	wndw.setKeypress(keypress);
}

void Labyrinth::filling_maze()
{	
	GLint c = 0;
	for (GLfloat j = stepH; j < crossH * stepH - 0.1; j += stepH)
		for (GLfloat i = stepV; i < crossV * stepV - 0.1; i += stepV)
		{
			maze[c].dir[up].x1 = maze[c].dir[le].x1 = i;
			maze[c].dir[up].y1 = maze[c].dir[le].y1 = j;

			maze[c].dir[up].x2 = maze[c].dir[up].x1+stepV;
			maze[c].dir[up].y2 = maze[c].dir[up].y1;

			maze[c].dir[le].x2 = maze[c].dir[le].x1;
			maze[c].dir[le].y2 = maze[c].dir[le].y1+stepH;
				
			maze[c].dir[down].x1 = maze[c].dir[up].x1;
			maze[c].dir[down].y1 = maze[c].dir[up].y1+stepH;
			maze[c].dir[down].x2 = maze[c].dir[up].x2;
			maze[c].dir[down].y2 = maze[c].dir[up].y2+stepH;

			maze[c].dir[ri].x1 = maze[c].dir[le].x1+stepV;
			maze[c].dir[ri].y1 = maze[c].dir[le].y1;
			maze[c].dir[ri].x2 = maze[c].dir[le].x2+stepV;
			maze[c].dir[ri].y2 = maze[c].dir[le].y2;;

			if ((c+1)%(crossV-1) != 0) maze[c].neig[ri] = &maze[c+1];
			if (c%(crossV-1) != 0) maze[c].neig[le] = &maze[c-1];
			if (c < (crossH-2)*(crossV-1)) maze[c].neig[down] = &maze[c+crossV-1];
			if (c >= crossV-1) maze[c].neig[up] = &maze[c-crossV+1];
			c++;
		}
}

void set_way (Cell* b, int way, bool value) {
	b->dir[way].painted = value;
	if (b->neig[way]) b->neig[way]->dir[OPPOSITE(way)].painted = value;
}

int sides(Cell b) {
	int sides = 0;
	for (int way = 0; way < 4; way++)
		if (b.dir[way].painted) sides++;
	return sides;
}

//function that returns number of colored cells  
int red (Cell a) {
	int sides = 0;
	for (int way = 0; way < 4; way++)
		if (!a.dir[way].painted && a.neig[way] && !a.neig[way]->visited) sides++;
	return sides;
}

void Labyrinth::build_maze() 
{
	stack<Cell*> cellsStack;
	set<int> ways;
	bool removeWall = false;

	srand(time(NULL));
	cellsStack.push(&maze[0]);
	
	while (!cellsStack.empty()) {
		Cell* curr = cellsStack.top();
		curr->visited = true;
		ways.clear();
		while (ways.size() != 4) {
			short way = rand() % 4;
			if (curr->neig[way] && !curr->neig[way]->visited) {
				set_way(curr, way, false);
				removeWall = true;
				cellsStack.push(curr->neig[way]);
				break;
			}
			ways.insert(way);
		}	
		if (!removeWall) cellsStack.pop();
		removeWall = false;
	}
	maze[0].dir[le].painted = false;
	maze[cellsNum-1].dir[ri].painted = false;
}

void Labyrinth::solve_maze() {
	bool change = false;
	Cell* curr;
	for (int i = 0; i < cellsNum; i++)
		if (sides(maze[i]) == 3 && maze[i].visited) {
			curr = &maze[i];
			curr->visited = false;
			while (sides(*curr)+red(*curr) > 1) {
				for (GLint way = 0; way < 4; way++) {
					if (!curr->dir[way].painted && curr->neig[way]->visited && 
						(sides(*curr->neig[way]) + red(*curr->neig[way]) - 1 == 2)) {
						curr->neig[way]->visited = false;
						curr = curr->neig[way];
						change = true;
					}
				}
				if (!change) break;
				change = false;
			}
		}
}