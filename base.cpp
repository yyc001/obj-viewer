#include <math.h>
#include <GL/glut.h>
// struct vec3d{
//     float x,y,z;
// };
// typedef float[3] vec3df;

void vecCross3(float *dst, const float *a, const float *b){
    dst[0] = a[1]*b[2] - a[2]*b[1];
    dst[1] = a[2]*b[0] - a[0]*b[2];
    dst[2] = a[0]*b[1] - a[1]*b[0]; 
}

float vecLen32(float x, float y, float z){
    return x*x+y*y+z*z;
}

void vecNormalize3(float *a){
    float norm = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void vecNormalize2(float *a){
    float norm = sqrt(a[0]*a[0] + a[1]*a[1]);
    a[0] /= norm;
    a[1] /= norm;
}

void menv_begin(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}
void menv_mul(float *matrix){
    glMultMatrixf(matrix);
}
void menv_get(float *matrix){
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}
void menv_end(){
    glPopMatrix();
}

double PI = acos(-1);