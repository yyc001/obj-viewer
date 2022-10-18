#include <stdlib.h>
//#include <glut.h>
#include <GL/glut.h> //for linux
//#include <glut.h> //windows
//#include <math.h>
#include <stdio.h>
#include <string>
#include <pthread.h>
#include <iostream>
#include <thread>
//#include <future>
#include "loader.h"
#include "geom.h"

using namespace std;


bool mousePressed;
bool rightPressed;
bool middlePressed;
GLfloat x_click;
GLfloat y_click;
GLfloat y_rotate;
GLfloat x_rotate;
GLfloat x_translate;
GLfloat y_translate;
GLfloat Perm_y_rotate;
GLfloat Perm_x_rotate;
GLfloat perm_x_translate;
GLfloat perm_y_translate;
bool lighting_enabled;
bool perspective_mode_enabled =  true;
bool auto_rotate_enabled = false;
GLfloat zoom = 0;
GLfloat perm_zoom = 0;
int rotate_speed;
int auto_rotate_speed = 1;
vector<Trimesh*> meshes;
//future<void> fut;
bool future_initialized;
char command[1000];
//string command;
bool newCommand;
TrimeshLoader* loader;
vector<float> camera_target;

void myDisplay();
void initGL();
void myReshape(GLsizei width, GLsizei height);

int main(int argc, char* argv[])
{
	//cout << "seg fault" << endl;
	
	
	loader = new TrimeshLoader();  
	float origin[3] = {0,0,0};
	camera_target.assign(origin, origin + 3);
	//cout << "seg fault1" << endl;


	if(argc >= 2)
	{
		Trimesh* mesh = new Trimesh();
		loader->loadOBJ(argv[1], mesh);
		meshes.push_back(mesh);
	}
	//cout << "seg fault2";
	glutInit(&argc, argv); //init glut library

	glutInitWindowSize(1000.f, 1000.f);
	glutInitWindowPosition(0,0);
	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glutCreateWindow("Object loader");
	glutInitDisplayMode(GLUT_DOUBLE);

	
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	// glutMouseFunc(MouseButton);
	// glutKeyboardFunc(Keyboard);
	// glutMotionFunc(MouseMotion);
	// glutIdleFunc(idle);
	initGL();
	

	//fut = std::async(std::launch::async, wait_in);
	//cout << "seg fault4";
	glutMainLoop();	
	
	
}


//standard set-up
void myDisplay()
{
	
	//cout << " ??? " << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	myReshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	float diam = 0;
	float z_min = 0;

	vector<float> camera_target;
	Trimesh* mesh = NULL;
	if(meshes.size() > 0)
	{
		mesh = meshes.back();
		if(mesh)
		{
			camera_target = mesh->get_target();
			diam = mesh->get_diam();
			z_min = mesh->get_min_z();
		}
	}
	gluLookAt(0, 0, 4.0f + diam * 4.0f + perm_zoom + zoom, camera_target[0], camera_target[1], camera_target[2], 0.0, 1.0, 0.0);
	//gluLookAt(0., 0., 2.*(diam),  camera_target[0], camera_target[1], camera_target[2], 0.0, 1.0, 0.0);
    
    // void gluLookAt(
    //     GLdouble eyex, GLdouble eyey,GLdouble eyez,
    //     GLdouble centerx, GLdouble centery, GLdouble centerz,
    //     GLdouble upx, GLdouble upy,GLdouble upz
    // );

	// float auto_rotate_deg = auto_rotate_enabled ? glutGet(GLUT_ELAPSED_TIME)/100.0f : 0;

	// glTranslatef(x_translate/50.0f, -y_translate/50.0f, 0.f);

	//handle rotation about a point
	// glTranslatef(camera_target[0], camera_target[1], camera_target[2]);
	// float deg_x = Perm_x_rotate + x_rotate + auto_rotate_deg * auto_rotate_speed;
	// float deg_y = Perm_y_rotate + y_rotate + auto_rotate_deg * auto_rotate_speed;
	// glRotatef(-deg_x, 1, 0, 0); //rotating object, camera static'
	// glRotatef(deg_y, 0, 1, 0);	
	// glTranslatef(-camera_target[0], -camera_target[1], -camera_target[2]);

	// mesh->applyTransformations();
	if(mesh != NULL)
		mesh->drawFaces();


	glDisable(GL_LIGHTING);
	glFlush();
	glutSwapBuffers();
	
}

void initGL()
{
	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glDisable(GL_CULL_FACE);


	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);


}


void myReshape(GLsizei width, GLsizei height)
{
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar

	if(meshes.size() > 0)
	{
		Trimesh* mesh = meshes.back();
		if(mesh)
		{
			vector<float> v = mesh->get_target();
			float diff = mesh->get_xy_diff();
			float diam = mesh->get_diam();
			
			float left = v[0] - diam; 
			float right = v[0] + diam;
			float bottom = v[1] - diam; 
			float top = v[1] + diam; 
			//cout << left  << "   " <<right;
			if ( aspect < 1.0 ) { 
			// window taller than wide 
				bottom /= aspect; 
				top /= aspect; 
			} else { 
				left *= aspect; 
				right *= aspect;
			}
					
			if(perspective_mode_enabled)
			{
				//glFrustum(left, right, bottom, top, 0.1f, 0.1f + diam); 
				gluPerspective(45, aspect, 5.0f, 5.0f + diam * 50.0f);
				//gluPerspective(45, aspect, 0.1f, 100.0f);
				//cout << 2 * diam;
			}
			else
				glOrtho(left, right, bottom, top, 1.0f, 1.0f + diam); 
		}

	}
	
   
		

}