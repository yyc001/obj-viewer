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
#include "windowing.h"

using namespace std;

Windowing window;
// Windowing *mWindow;
Camera stare_at(Trimesh *mesh);

int main(int argc, char* argv[])
{
    glutInit(&argc, argv); //init glut library
	window.init();
	TrimeshLoader *loader = new TrimeshLoader();  
	Trimesh mesh1, mesh2;
	if(argc >= 2)
	{
		// mesh = new Trimesh();
		loader->loadOBJ(argv[1], &mesh1);
		window.meshes.push_back(&mesh1);
		mesh2 = mesh1;
		mesh2.prepareTransformation(MESH_SCALE, 0.5, 0.5, 0.5);
		mesh2.prepareTransformation(MESH_TRANSLATE, 10, 0, 0);
		mesh2.applyTransformations();
		mesh2.enable_transformation = 1;
		window.meshes.push_back(&mesh2);
	} else {
		cout << "no input .obj"<<endl;
		exit(-1);
	}
	//cout << "seg fault2";
	Camera camera = stare_at(&mesh1);
	window.setCamera(camera);
	window.mainLoop();
}

Camera stare_at(Trimesh *mesh)
{
	Camera camera;
	vector<float> mp = mesh->middlePoint();
	camera.at.x = mp[0];
	camera.at.y = mp[1];
	camera.at.z = mp[2];
	
	// camera.at.x = 0;
	// camera.at.y = 0;
	// camera.at.z = 0;

	camera.eye.x = 0;
	camera.eye.y = mp[0];

	float diam = mesh->diam();
	camera.eye.z = diam * 2;
	// cout << diam <<endl;
	camera.up.x = 0;
	camera.up.y = 1;
	camera.up.z = 0;

	camera.fovy = 45;
	camera.width = 1000;
	camera.height = 1000;
	camera.near = 0.1;
	camera.far = diam * 4;
	camera.mode = CAMERA_PERSPECTIVE;

	return camera;
}