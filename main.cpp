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
	//cout << "seg fault" << endl;
    glutInit(&argc, argv); //init glut library
	
	
	TrimeshLoader *loader = new TrimeshLoader();  
	Trimesh* mesh;
	//cout << "seg fault1" << endl;


	if(argc >= 2)
	{
		mesh = new Trimesh();
		loader->loadOBJ(argv[1], mesh);
		window.meshes.push_back(mesh);
	} else {
		cout << "no input .obj"<<endl;
		exit(-1);
	}
	//cout << "seg fault2";
	window.init();
	Camera camera = stare_at(mesh);
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