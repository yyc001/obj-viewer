#include<vector>
#include<float.h>
#include<math.h>
#include<cstring>
#include "base.h"
#include "mesh.h"
#include "camera.h"
#include "picker.h"
#include "animation.h"
#include <GL/glut.h>
#include <cstdio>

Trimesh::Trimesh()
{
    min_dot[0] = min_dot[1] = min_dot[2] = FLT_MAX;
    max_dot[0] = max_dot[1] = max_dot[2] = -FLT_MAX;
    enable_transformation = 0;
    ac = 0;
	rgba[0] = 1;
	rgba[1] = 1;
	rgba[2] = 1;
	rgba[3] = 1;

}

void Trimesh::addVertex(float values[3])
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

void Trimesh::addFace(int tri[3])
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

float Trimesh::diam()
{
    float x_diff = max_dot[0] - min_dot[0];
    float y_diff = max_dot[1] - min_dot[1];
    float z_diff = max_dot[2] - min_dot[2];
    return max(x_diff, max(y_diff, z_diff));
}

vector<float> Trimesh::middlePoint()
{
    float t[3];
    t[0] = (max_dot[0] + min_dot[0])/2.0f;
    t[1] = (max_dot[1] + min_dot[1])/2.0f;
    t[2] = (max_dot[2] + min_dot[2])/2.0f;
	// t[3] = 1;
	if(ac){
		FrameTransformation ft = ac->getNowFrame();
		t[0] += ft.position[0];
		t[1] += ft.position[1];
		t[2] += ft.position[2];
    	return vector<float>(t, t+3);

	}
	if(enable_transformation){
		t[0] += frame.position[0];
		t[1] += frame.position[1];
		t[2] += frame.position[2];
	}
    return vector<float>(t, t+3);

}

void Trimesh::display()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    if (enable_transformation) {
		frame.display();
    }
    if(ac){
        ac->display();
    }
	glColor4f(rgba[0], rgba[1], rgba[2], rgba[3]);
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
    glPopMatrix();


}