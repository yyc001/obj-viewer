#ifndef __MESH__
#define __MESH__

#include<vector>
#include<float.h>
#include<math.h>
#include<cstring>
#include "animation.h"
using namespace std;

#define MESH_TRANSLATE 1.0f
#define MESH_SCALE 2.0f
#define MESH_ROTATE 3.0f
#define MESH_CLEAR 4.0f



class Trimesh
{
	public:

		vector<vector<int> > faces; //contains a list of faces, which is a list on vertex index
		vector<vector<float> > vertices;
		vector<vector<float> > faces_normals; //contains a list of normal vectors corresponding to each face
		int rgba[4];

        float min_dot[3], max_dot[3];

        float transformation_mat[16];
        vector<vector<float> > prepare_transformations;
        int enable_transformation;

		AnimationController *ac;

		// 动画

		Trimesh();

		void prepareTransformation(float t, float x, float y, float z, float theta = 0);

		void applyTransformations(int append = 0);

		// void setTransformation(float *matrix)
		// {
		// 	memcpy(transformation_mat, matrix, sizeof(transformation_mat));
		// }


		void addVertex(float values[3]);

		void addFace(int tri[3]);

		float diam();

		vector<float> middlePoint();

		void display();

};



#endif