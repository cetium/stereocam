#include "simple3dscene.hpp"

#include <iostream>
#include <stdio.h>

#include <cmath>
#include <GL/glut.h>

using namespace cv;

static float angleX=0.0, angleY = 0.0, ratio;
static float x=0.0f,y=0.0f,z=5.0f;
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
  
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(angleX, 0.0f, 1.0f, 0.0f);
	glRotatef(angleY, 1.0f, 0.0f, 0.0f); 
	
	drawPointCloud();

	//glutSolidCube(2.0f); // Render the primitive  
	glPopMatrix();

	glutSwapBuffers();
}


void Simple3DScene::drawPointCloud(){
	if(cloud.empty()){
		return;
	}

	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(2.0f);
	glScalef(-1.0/100.0, -1.0/100.0, 1.0/255.0);
	
	const double max_z = 1.0e4;
	glBegin(GL_POINTS);
	for(int y = 0; y < cloud.rows; y++){
        for(int x = 0; x < cloud.cols; x++){

            Vec3f point = cloud.at<Vec3f>(y, x);
            if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) 
				continue;
			
			glVertex3f(point[0],point[1],point[2]);
			
        }
    }
	glEnd();
}



void Simple3DScene::moveFlat(int direction) {
	x = x + direction*(lx)*0.1;
	z = z + direction*(lz)*0.1;
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}



void Simple3DScene::rotate(int xdiff, int ydiff){
	angleX += (float)xdiff/2.0f;
	angleY += (float)ydiff/2.0f;
}


void Simple3DScene::setPointCloud(Mat & mat){
	if(!cloud.empty())
		cloud.release();
	cloud = mat.clone();
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