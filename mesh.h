#ifndef __MESH__
#define __MESH__

#include<vector>
#include<float.h>
#include<math.h>
#include<cstring>
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

        float min_dot[3], max_dot[3];

        float transformation_mat[16];
        vector<vector<float> > prepare_transformations;
        int enable_transformation;

		// 动画

		Trimesh()
		{
			min_dot[0] = min_dot[1] = min_dot[2] = FLT_MAX;
			max_dot[0] = max_dot[1] = max_dot[2] = -FLT_MAX;
			memset(transformation_mat, 0, sizeof(transformation_mat));
			enable_transformation = 0;
		}

		void prepareTransformation(float t, float x, float y, float z, float theta = 0)
		{
			if(t == 4.0f)
			{
				prepare_transformations.clear();
				return;
			}

			vector<float> xd;		
			xd.push_back((float)t);
			xd.push_back(theta);
			xd.push_back(x);
			xd.push_back(y);
			xd.push_back(z);
			prepare_transformations.push_back(xd);
		}

		void applyTransformations(int append = 0)
		{
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();
			for(int i = prepare_transformations.size() - 1; i >= 0; i--) {
				vector<float> v = prepare_transformations[i];
				if(v[0] == MESH_TRANSLATE)
					glTranslatef(v[2], v[3], v[4]);
				if(v[0] == MESH_SCALE)
					glScalef(v[2], v[3], v[4]);
				if(v[0] == MESH_ROTATE)
					glRotatef(v[1], v[2], v[3], v[4]);
			}
            if (append) {
                glMultMatrixf(transformation_mat);
            }
            glGetFloatv(GL_MODELVIEW_MATRIX, transformation_mat);
            glPopMatrix();
            prepare_transformations.clear();
		}


		void addVertex(float values[3])
		{
			max_dot[0] = values[0] > max_dot[0] ? values[0] : max_dot[0];
			max_dot[1] = values[1] > max_dot[1] ? values[1] : max_dot[1];
			max_dot[2] = values[2] > max_dot[2] ? values[2] : max_dot[2];
			min_dot[0] = values[0] < min_dot[0] ? values[0] : min_dot[0];
			min_dot[1] = values[1] < min_dot[1] ? values[1] : min_dot[1];
			min_dot[2] = values[2] < min_dot[2] ? values[2] : min_dot[2];
			vector<float> v(values, values + 3);
			vertices.push_back(v);
		}

		void addFace(int tri[3])
		{
			vector<int> vec(tri, tri + 3);
			faces.push_back(vec);
			vector<float> v1 = vertices[tri[0]];
			vector<float> v2 = vertices[tri[1]];
			vector<float> v3 = vertices[tri[2]];
			
			float u[3];
			u[0] = (v2[0]-v1[0]);
			u[1] = (v2[1]-v1[1]);
			u[2] = (v2[2]-v1[2]);
			float v[3];
			v[0] = (v3[0]-v1[0]);
			v[1] = (v3[1]-v1[1]);
			v[2] = (v3[2]-v1[2]);

			//vector<float> normal;
			float n[3];
			n[0] = ( u[1] * v[2] - u[2] * v[1] ); //y1z2 - y2z1
			n[1] = ( u[2] * v[0] - u[0] * v[2] ); //z1x2 - z2x1
			n[2] = ( u[0] * v[1] - u[1] * v[0] ); //x1y2 - x2y1
			float length = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
			n[0] = n[0]/length;
			n[1] = n[1]/length;
			n[2] = n[2]/length;
			faces_normals.push_back(vector<float>(n, n+3));
		}

		float diam()
		{
			float x_diff = max_dot[0] - min_dot[0];
			float y_diff = max_dot[1] - min_dot[1];
			float z_diff = max_dot[2] - min_dot[2];
			return max(x_diff, max(y_diff, z_diff));
		}

		vector<float> middlePoint()
		{
			float t[3];
			t[0] = (max_dot[0] + min_dot[0])/2.0f;
			t[1] = (max_dot[1] + min_dot[1])/2.0f;
			t[2] = (max_dot[2] + min_dot[2])/2.0f;
			return vector<float>(t, t+3);
		}

		void display()
		{
            glMatrixMode(GL_MODELVIEW);
            if (enable_transformation) {
                glPushMatrix();
                glMultMatrixf(transformation_mat);
            }

			for (unsigned i=0; i < faces.size(); i++)
			{
				glBegin(GL_TRIANGLES);
                vector<int> face = faces[i];
                vector<float> normal = faces_normals[i];
                for(unsigned j = 0; j < 3; j++) {
                    glNormal3f(normal[0], normal[1], normal[2]);
                    int index = face[j];
                    vector<float> vertex = vertices[index];
                    glVertex3f(vertex[0], vertex[1], vertex[2]);
                }
				glEnd();
			}

            if (enable_transformation) {
                glPopMatrix();
            }

		}

};



#endif