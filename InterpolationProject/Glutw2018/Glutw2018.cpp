// interpolation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "glut.h"
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
using namespace std;

void display();
void reshape(int, int);
void idle();

void mouse_click(int button, int state, int x, int y);
void mouse_motion(int x, int y);
void initialization();



struct points {

	float x;
	float y;

};
struct color {
	float r, g, b;
};

class InterpolationElements {
public:

	vector<points> initial;
	
	vector<points> final;
	color col;
	
	InterpolationElements() {};




	InterpolationElements(points _initial, points _final) {


		
		initial.push_back(_initial);
		final.push_back(_final);

	}

	void setColor(color _color) {
		col = _color;
	}

	void add(points _initial, points _final) {
		

		initial.push_back(_initial);
		final.push_back(_final);
		
	}

	void interpolationCalculation(float time) {

		

		glBegin(GL_POLYGON);
		glColor3f(col.r, col.g, col.b);
		for (int i = 0; i < initial.size() ; i++) {

			glVertex3f((1.0 - time) * initial[i].x + time * final[i].x, (1.0 - time) * initial[i].y + time * final[i].y, 0);



		}
		glEnd();



	}
	void show() {

	}

};




float angle = 0.0;
float angle2 = 0.0;
int x_click, y_click;
int button;
float cameraAngle = M_PI / 2.0f;

float dx = 0.0;
float dy = 0.0;
float dz = 0.0;
float scale = 1.0;

//time t
float t = 0.0;
float t2 = 0.0;
float t3 = 0.0;
float t4 = 0.0;
float t5 = 0.0;
float t6 = 0.0;

float X0 = -2.0;
float X1 = 0.0;
float X2 = 2.0;

float Y0 = 2.0;
float Y1 = 2.0;
float Y2 = 2.0;

float h = 4.0 / 1000;

int stage = 0;
bool firstStage = true;
bool shoot = false;

InterpolationElements GunUpper;
InterpolationElements GunMiddle;
InterpolationElements GunLower;
InterpolationElements bang;
InterpolationElements bullet;
InterpolationElements bulletWind;
InterpolationElements wall;
InterpolationElements wallCrack;



int main(int argc, char** argv)
{
	// initialize glut
	glutInit(&argc, argv);

	// initialize window position
	glutInitWindowPosition(10, 10);

	// window size
	glutInitWindowSize(800, 600);

	// display mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);

	// create a window
	glutCreateWindow("A simple glut window");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMouseFunc(mouse_click);
	glutMotionFunc(mouse_motion);
	initialization();

	// glut main loop
	glutMainLoop();







	return 0;
}

void reshape(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0,     // eye's position
		0.0, 0.0, 1.0,     // center of interest
		0.0f, 1.0f, 0.0f); // up direction

}

void idle()
{
	
	if (stage == 0) {
		
		if (firstStage == true) {

			t2 += 0.002;
			if (t2 > 1) {
				firstStage = false;
			}
		}else {

			t2 -= 0.002;
			if (t2 < 0) {
				stage += 1;
				shoot = true;
			}
		}
	

	}

	if ( stage ==1) {

		t += 0.0003;
		t5 += 0.0003;

		if (t > 1) {
			stage += 1;
		}

	}

	if (shoot) {

		t3 += 0.01;

			if (t3 > 1) {
				shoot = false;
			}
	}

	if (stage == 2) {

		t4 += 0.001;
		t5 -= 0.001;
		
		if (t4 > 1) {
			stage += 1;
		}
	}

	if (stage == 3) {
		t6 += 0.007;
		if (t6 > 1) {
			stage += 1;
		}
	}


	//angle += 0.8;        // angle is a global var, visible to every func
	//angle2 += 0.05;
	glLoadIdentity();
	//initLight();
	//gluLookAt(5 * cos(angle2 * 0.01), 4, 5 * sin(angle2 * 0.01),     // eye's position
	float radius = 10;
	gluLookAt(0.0f, /*4.0f */radius * cos(cameraAngle), radius * sin(cameraAngle),     // eye's position
		0, 0, 0,     // center of interest
		0.0f, 1.0f, 0.0f); // up direction

	glutPostRedisplay();
}

void display() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




	glPushMatrix();
	glRotatef(angle, 0.0, 1.0, 0.0);
	glScalef(scale, scale, scale);

	bang.interpolationCalculation(t2);
	GunUpper.interpolationCalculation(t);
	GunMiddle.interpolationCalculation(t);
	GunLower.interpolationCalculation(t);
	bulletWind.interpolationCalculation(t5);
	bullet.interpolationCalculation(t3);
	wall.interpolationCalculation(t4);
	wallCrack.interpolationCalculation(t6);




	glPopMatrix();


	glFlush();
}

void mouse_click(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		x_click = x;
		y_click = y;
		::button = button; // button = GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, GLUT_MIDDLE_BUTTON
	}
	else {
		::button = -1;
	}
}


void mouse_motion(int x, int y) {
	double mvmatrix[16];
	double projmatrix[16];
	int    viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	double cur_y = double(viewport[3] - y);
	double obj_cur_y, obj_cur_x, obj_cur_z;
	gluUnProject((double)x, cur_y, 0.5, mvmatrix, projmatrix, viewport, &obj_cur_x, &obj_cur_y, &obj_cur_z);

	double pre_y = double(viewport[3] - y_click);
	double obj_pre_y, obj_pre_x, obj_pre_z;
	gluUnProject((double)x_click, pre_y, 0.5, mvmatrix, projmatrix, viewport, &obj_pre_x, &obj_pre_y, &obj_pre_z);

	switch (button) {
	case GLUT_LEFT_BUTTON:
		dx += (obj_cur_x - obj_pre_x) * 2.5;
		dy -= (obj_pre_y - obj_cur_y) * 2.5;
		break;
	case GLUT_RIGHT_BUTTON:
		angle -= (x_click - x);
		cameraAngle += (y_click - y) * 0.02;
		break;
	case GLUT_MIDDLE_BUTTON:
		scale -= (x_click - x) * 0.02;
		break;
	}
	x_click = x;
	y_click = y;
}



void initialization() {

	//upper gun part point initialization 
	points point1Initial, point1final;
	points point2Initial, point2final;
	points point3Initial, point3final;
	points point4Initial, point4final;
	color upperColor;

	upperColor.r = 0.75;
	upperColor.g = 0.75;
	upperColor.b = 0.75;

	point1Initial.x = 0.0;
	point1Initial.y = 2.0;

	point1final.x = 6.0;
	point1final.y = 2.0;

	point2Initial.x = 3.0;
	point2Initial.y = 2.0;

	point2final.x = 9.0;
	point2final.y = 2.0;

	point3Initial.x = 3.0;
	point3Initial.y = 1.0;

	point3final.x = 9.0;
	point3final.y = 1.0;

	point4Initial.x = 0.0;
	point4Initial.y = 1.0;

	point4final.x = 6.0;
	point4final.y = 1.0;

	//middle gun part point  initialization 
	points point5Initial, point5final;
	points point6Initial, point6final;
	points point7Initial, point7final;
	points point8Initial, point8final;
	color middleColor;

	middleColor.r = 0.67;
	middleColor.g = 0.67;
	middleColor.b = 0.67;

	point5Initial.x = 1.0;
	point5Initial.y = 1.0;

	point5final.x = 7.0;
	point5final.y = 1.0;

	point6Initial.x = 3.0;
	point6Initial.y = 1.0;

	point6final.x = 9.0;
	point6final.y = 1.0;

	point7Initial.x = 3.0;
	point7Initial.y = 0.7;

	point7final.x = 9.0;
	point7final.y = 0.7;

	point8Initial.x = 1.0;
	point8Initial.y = 0.7;

	point8final.x = 7.0;
	point8final.y = 0.7;

	//lower gun part initialization
	points point9Initial, point9final;
	points point10Initial, point10final;
	points point11Initial, point11final;
	points point12Initial, point12final;
	color lowerColor;

	lowerColor.r = 0.55;
	lowerColor.g = 0.27;
	lowerColor.b = 0.07;

	point9Initial.x = 2.0;
	point9Initial.y = 0.7;

	point9final.x = 8.0;
	point9final.y = 0.7;

	point10Initial.x = 3.0;
	point10Initial.y = 0.7;

	point10final.x = 9.0;
	point10final.y = 0.7;

	point11Initial.x = 3.5;
	point11Initial.y = -1.0;

	point11final.x = 9.5;
	point11final.y = -1.0;

	point12Initial.x = 2.5;
	point12Initial.y = -1.0;

	point12final.x = 8.5;
	point12final.y = -1.0;

	//Bang initialization
	points point13Initial, point13final;
	points point14Initial, point14final;
	points point15Initial, point15final;
	points point16Initial, point16final;
	points point17Initial, point17final;
	points point18Initial, point18final;
	color bangColor;

	bangColor.r = 1.0;
	bangColor.g = 1.0;
	bangColor.b = 0.0;
	
	point13Initial.x = -0.2;
	point13Initial.y = 1.5;

	point13final.x = -0.2;
	point13final.y = 1.5;

	point14Initial.x = -0.2;
	point14Initial.y = 1.5;

	point14final.x = -2.3;
	point14final.y = 2.5;

	point15Initial.x = -0.2;
	point15Initial.y = 1.5;

	point15final.x = -1.3;
	point15final.y = 1.8;

	point16Initial.x = -0.2;
	point16Initial.y = 1.5;

	point16final.x = -2.5;
	point16final.y = 1.5;

	point17Initial.x = -0.2;
	point17Initial.y = 1.5;

	point17final.x = -1.3;
	point17final.y = 1.2;

	point18Initial.x = -0.2;
	point18Initial.y = 1.5;

	point18final.x = -2.3;
	point18final.y = 0.5;

	//bullet points initialization
	points point19Initial, point19final;
	points point20Initial, point20final;
	points point21Initial, point21final;
	color bulletColor;

	bulletColor.r = 0.86;
	bulletColor.g = 0.65;
	bulletColor.b = 0.12;

	point19Initial.x = -0.2;
	point19Initial.y = 1.5;

	point19final.x = -0.2;
	point19final.y = 1.6;

	point20Initial.x = -0.2;
	point20Initial.y = 1.5;

	point20final.x = -0.2;
	point20final.y = 1.4;

	point21Initial.x = -0.2;
	point21Initial.y = 1.5;

	point21final.x = -0.5;
	point21final.y = 1.5;

	//bullet wind points initialization
	points point22Initial, point22final;
	points point23Initial, point23final;
	points point24Initial, point24final;
	points point25Initial, point25final;
	color windColor;

	windColor.r = 1.0;
	windColor.g = 1.0;
	windColor.b = 1.0;
	


	point22Initial.x = -0.15;
	point22Initial.y = 1.5;

	point22final.x = -0.15;
	point22final.y = 1.55;

	point23Initial.x = -0.15;
	point23Initial.y = 1.5;

	point23final.x = 5.0;
	point23final.y = 1.55;

	point24Initial.x = -0.15;
	point24Initial.y = 1.5;

	point24final.x = 5.0;
	point24final.y = 1.45;

	point25Initial.x = -0.15;
	point25Initial.y = 1.5;

	point25final.x = -0.15;
	point25final.y = 1.45;

	//wall points initialization 
	points point26Initial, point26final;
	points point27Initial, point27final;
	points point28Initial, point28final;
	points point29Initial, point29final;
	color wallColor;

	wallColor.r = 0.54;
	wallColor.g = 0.81;
	wallColor.b = 0.76;
	
	point26Initial.x = -8.0;
	point26Initial.y = 5.0;

	point26final.x =-2.0;
	point26final.y = 5.0;

	point27Initial.x = -6.0;
	point27Initial.y = 5.0;

	point27final.x = -0.35;
	point27final.y = 5.0;

	point28Initial.x = -6.0;
	point28Initial.y = -5.0;

	point28final.x = -0.35;
	point28final.y = -5.0;

	point29Initial.x = -8.0;
	point29Initial.y = -5.0;

	point29final.x = -2.0;
	point29final.y = -5.0;


	//wall crack points initialization 
	points point30Initial, point30final;
	points point31Initial, point31final; 
	points point32Initial, point32final;
	points point33Initial, point33final;
	points point34Initial, point34final;
	points point35Initial, point35final;
	points point36Initial, point36final;
	color crackColor;

	crackColor.r = 0.0;
	crackColor.g = 0.0;
	crackColor.b = 0.0;

	point30Initial.x = -0.5;
	point30Initial.y = 1.5;

	point30final.x = -0.5;
	point30final.y = 1.5;

	point31Initial.x = -0.5;
	point31Initial.y = 1.5;

	point31final.x = -0.7;
	point31final.y = 1.4;

	point32Initial.x = -0.5;
	point32Initial.y = 1.5;

	point32final.x = -0.9;
	point32final.y = 1.5;

	point33Initial.x = -0.5;
	point33Initial.y = 1.5;

	point33final.x = -0.7;
	point33final.y = 1.3;

	point34Initial.x = -0.5;
	point34Initial.y = 1.5;

	point34final.x = -0.55;
	point34final.y = 1.35;

	point35Initial.x = -0.5;
	point35Initial.y = 1.5;

	point35final.x = -0.6;
	point35final.y = 1.2;

	point36Initial.x = -0.5;
	point36Initial.y = 1.5;

	point36final.x = -0.52;
	point36final.y = 1.35;


	//Uppger gun part, adding the points to the element
	GunUpper = InterpolationElements(point1Initial, point1final);
	GunUpper.add(point2Initial, point2final);
	GunUpper.add(point3Initial, point3final);
	GunUpper.add(point4Initial, point4final);
	GunUpper.setColor(upperColor);

	//middle gun part, adding the points to the element
	GunMiddle = InterpolationElements(point5Initial, point5final);
	GunMiddle.add(point6Initial, point6final);
	GunMiddle.add(point7Initial, point7final);
	GunMiddle.add(point8Initial, point8final);
	GunMiddle.setColor(middleColor);

	//lower gun part, adding the points to the element
	GunLower = InterpolationElements(point9Initial, point9final);
	GunLower.add(point10Initial, point10final);
	GunLower.add(point11Initial, point11final);
	GunLower.add(point12Initial, point12final);
	GunLower.setColor(lowerColor);

	//bang, adding the points to the element 
	bang = InterpolationElements(point13Initial, point13final);
	bang.add(point14Initial, point14final);
	bang.add(point15Initial, point15final);
	bang.add(point16Initial, point16final);
	bang.add(point17Initial, point17final);
	bang.add(point18Initial, point18final);
	bang.setColor(bangColor);

	//bullet, adding the points to the element
	bullet = InterpolationElements(point19Initial, point19final);
	bullet.add(point20Initial, point20final);
	bullet.add(point21Initial, point21final);
	bullet.setColor(bulletColor);

	//bullet wind, adding the points to th element
	bulletWind = InterpolationElements(point22Initial, point22final);
	bulletWind.add(point23Initial, point23final);
	bulletWind.add(point24Initial, point24final);
	bulletWind.add(point25Initial, point25final);
	bulletWind.setColor(windColor);

	//wall, adding points to the element
	wall = InterpolationElements(point26Initial, point26final);
	wall.add(point27Initial, point27final);
	wall.add(point28Initial, point28final);
	wall.add(point29Initial, point29final);
	wall.setColor(wallColor);

	//wall crack, adding the points to the elemenet
	wallCrack = InterpolationElements(point30Initial, point30final);
	wallCrack.add(point31Initial, point31final);
	wallCrack.add(point32Initial, point32final);
	wallCrack.add(point33Initial, point33final);
	wallCrack.add(point34Initial, point34final);
	wallCrack.add(point35Initial, point35final);
	wallCrack.add(point36Initial, point36final);
	wallCrack.setColor(crackColor);
}