#include <math.h>

// struct vec3d{
//     float x,y,z;
// };
// typedef float[3] vec3df;

void vecCross3(float *dst, const float *a, const float *b){
    dst[0] = a[1]*b[2] - a[2]*b[1];
    dst[1] = a[2]*b[0] - a[0]*b[2];
    dst[2] = a[0]*b[1] - a[1]*b[0]; 
}

void vecNormalize3(float *a){
    float norm = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

double PI = acos(-1);