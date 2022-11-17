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
#include "base.h"
#include "mesh.h"
#include "camera.h"
#include "windowing.h"
#include "picker.h"

using namespace std;

Windowing window;
// Windowing *mWindow;
Camera stare_at(Trimesh *mesh);
Camera camera;
CameraController cc;
AnimationController ac;
Picker picker;

Trimesh mesh1, mesh2, mesh3, mesh4, mesh5;

int main(int argc, char* argv[])
{
    glutInit(&argc, argv); //init glut library
	window.init();
	TrimeshLoader *loader = new TrimeshLoader();  
	Trimesh mesh1;
	if(argc >= 2)
	{
		// mesh = new Trimesh();
		loader->loadOBJ(argv[1], &mesh1);
		window.meshes.push_back(&mesh1);
		mesh2 = mesh3 = mesh4 = mesh5 = mesh1;
		window.meshes.push_back(&mesh2);
		window.meshes.push_back(&mesh3);
		window.meshes.push_back(&mesh4);
		window.meshes.push_back(&mesh5);


		// mesh1.prepareTransformation(MESH_SCALE, 0.5, 0.5, 0.5);
		// mesh1.applyTransformations();

		// mesh2 = mesh1;
		// mesh2.prepareTransformation(MESH_SCALE, 0.5, 0.5, 0.5);
		// mesh2.prepareTransformation(MESH_TRANSLATE, 10, 0, 0);
		// mesh2.applyTransformations();
		// mesh2.enable_transformation = 1;
		// window.meshes.push_back(&mesh2);
		mesh1.ac = &ac;
	} else {
		cout << "no input .obj"<<endl;
		exit(-1);
	}
	//cout << "seg fault2";
	camera = stare_at(&mesh1);
	cc.reset = camera;
	window.cc = &cc;
	cc.camera = &camera;
	camera.mode = CAMERA_PERSPECTIVE;
	ac.rtime = CLOCKS_PER_SEC * 10;
	ac.repeat = 1;
	ac.type = ANIMATION_BEZIER;

	FrameTransformation f2 = (FrameTransformation{QfromRotation(0,1,0,0),      30.0f, 30.0f, 0.0f, 1.0f, 1.0f, 1.0f});
	FrameTransformation f3 = (FrameTransformation{QfromRotation(0,1,0,PI/2),   30.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f});
	FrameTransformation f4 = (FrameTransformation{QfromRotation(0,1,0,PI),     0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f});
	FrameTransformation f5 = (FrameTransformation{QfromRotation(0,1,0,PI/2*3), 0.0f, 30.0f, 0.0f, 1.0f, 1.0f, 1.0f});
	mesh2.enable_transformation = 1;
	mesh3.enable_transformation = 1;
	mesh4.enable_transformation = 1;
	mesh5.enable_transformation = 1;
	mesh2.frame = f2;
	mesh3.frame = f3;
	mesh4.frame = f4;
	mesh5.frame = f5;
	
	ac.frames.push_back(f2);
	ac.frames.push_back(f3);
	ac.frames.push_back(f4);
	ac.frames.push_back(f5);
	// cout << "???" << endl;

	window.picker = &picker;
	picker.camera = &camera;
	picker.meshes.push_back(&mesh1);
	picker.meshes.push_back(&mesh2);
	picker.meshes.push_back(&mesh3);
	picker.meshes.push_back(&mesh4);
	picker.meshes.push_back(&mesh5);
	picker.mode = 1;
	window.fps = 30;

	// printf("color %f %f %f %f\n", mesh1.rgba[0], mesh1.rgba[1], mesh1.rgba[2], mesh1.rgba[3]);
	window.mainLoop();
}

Camera stare_at(Trimesh *mesh)
{
	Camera camera;
	vector<float> mp = mesh->middlePoint();
	camera.at[0] = mp[0];
	camera.at[1] = mp[1];
	camera.at[2] = mp[2];
	
	// camera.at.x = 0;
	// camera.at.y = 0;
	// camera.at.z = 0;

	camera.eye[0] = 0;
	camera.eye[1] = mp[0];

	float diam = mesh->diam();
	camera.eye[2] = diam * 2;
	// cout << diam <<endl;
	camera.up[0] = 0;
	camera.up[1] = 1;
	camera.up[2] = 0;

	camera.fovy = 60;
	camera.width = 400;
	camera.height = 300;
	camera.near = 0.1;
	camera.far = diam * 4;
	camera.mode = CAMERA_PERSPECTIVE;
	camera.dirty = 1;
	return camera;
}