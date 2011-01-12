#include "simple3dscene.hpp"

#include <iostream>
#include <stdio.h>

#include <cmath>
#include <GL/glut.h>



static float angle=0.0,ratio;
static float x=0.0f,y=1.75f,z=5.0f;
static float lx=0.0f,ly=0.0f,lz=-1.0f;


Simple3DScene::Simple3DScene(){
}


Simple3DScene::~Simple3DScene(){
	
}


void Simple3DScene::init(int w, int h){

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE); 
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH); 

	/* Enable a single OpenGL light. */
	GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		  	x + lx,y + ly,z + lz,
			0.0f,1.0f,0.0f);
}




void Simple3DScene::draw(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		  	x + lx,y + ly,z + lz,
			0.0f,1.0f,0.0f);
	*/
	std::cout << x <<std::endl;

	glBegin(GL_POINTS); //Begin drawing points
  
	// a point
	glVertex3f(-0.75f, -0.25f, -5.0f);
  
	glEnd();
  
	glBegin(GL_LINES); //Begin drawing lines
  
	// 2 end points on line
	glVertex3f(-1.0f, 1.0f, -5.0f);
	glVertex3f(1.0f, -1.0f, -5.0f);
  
	glEnd();
  
	glBegin(GL_TRIANGLES);
  
	// 3 corner points on triangle
	glVertex3f(1.5f, 1.0f, -5.0f);
	glVertex3f(1.0f, 0.0f, -5.0f);
	glVertex3f(0.5f, 1.0f, -5.0f);
  
	glEnd();
  
	glBegin(GL_QUADS); //Begin drawing quadrilaterals
  
	// 4 corner points on square
	glVertex3f(-0.5f, -2.0f, -5.0f);
	glVertex3f(0.5f, -2.0f, -5.0f);
	glVertex3f(0.5f, -1.0f, -5.0f);
	glVertex3f(-0.5f, -1.0f, -5.0f);
 
	glEnd();
	glutSwapBuffers();

}



void Simple3DScene::moveFlat(int direction) {
	x = x + direction*(lx)*0.1;
	z = z + direction*(lz)*0.1;
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}

void Simple3DScene::orientMe(float ang) {
	angle += ang;

	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}


void Simple3DScene::resize(int w, int h){
	if(h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		  	x + lx,y + ly,z + lz,
			0.0f,1.0f,0.0f);
}