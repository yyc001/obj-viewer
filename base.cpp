#include <math.h>
#include <GL/glut.h>
#include "base.h"
#include <cstdio>
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

Quaternion::Quaternion(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}

Quaternion::Quaternion(){}

void Quaternion::toMatrix(float *matrix)
{
    #ifndef MATRIX
    #define MATRIX( row, col ) matrix[row * 4 + col]
    #endif
    // MATRIX( 0, 0 ) = 2 * ( x * x + w * w ) - 1;
    // MATRIX( 0, 1 ) = 2 * ( x * y + z * w );
    // MATRIX( 0, 2 ) = 2 * ( x * z - y * w );
    // MATRIX( 0, 3 ) = 0.0f;
    // MATRIX( 1, 0 ) = 2 * ( x * y - z * w );
    // MATRIX( 1, 1 ) = 2 * ( y * y + w * w ) - 1;
    // MATRIX( 1, 2 ) = 2 * ( y * z + x * w );
    // MATRIX( 1, 3 ) = 0.0f;
    // MATRIX( 2, 0 ) = 2 * ( x * z + y * w );
    // MATRIX( 2, 1 ) = 2 * ( y * z - x * w );
    // MATRIX( 2, 2 ) = 2 * ( z * z + w * w ) - 1;
    // MATRIX( 2, 3 ) = 0.0f;
    // MATRIX( 3, 0 ) = 0.0f;
    // MATRIX( 3, 1 ) = 0.0f;
    // MATRIX( 3, 2 ) = 0.0f;
    // MATRIX( 3, 3 ) = 1.0f;
    MATRIX( 0, 0 ) = 1 - 2 * ( y * y + z * z );
    MATRIX( 0, 1 ) =     2 * ( y * x - z * w );
    MATRIX( 0, 2 ) =     2 * ( x * z + y * w );
    MATRIX( 0, 3 ) = 0.0f;
    MATRIX( 1, 0 ) =     2 * ( x * y + w * z );
    MATRIX( 1, 1 ) = 1 - 2 * ( x * x + z * z );
    MATRIX( 1, 2 ) =     2 * ( y * z - x * w );
    MATRIX( 1, 3 ) = 0.0f;
    MATRIX( 2, 0 ) =     2 * ( x * z - y * w );
    MATRIX( 2, 1 ) =     2 * ( y * z + x * w );
    MATRIX( 2, 2 ) = 1 - 2 * ( x * x + y * y );
    MATRIX( 2, 3 ) = 0.0f;
    MATRIX( 3, 0 ) = 0.0f;
    MATRIX( 3, 1 ) = 0.0f;
    MATRIX( 3, 2 ) = 0.0f;
    MATRIX( 3, 3 ) = 1.0f;
    #undef MATRIX
}

Quaternion Quaternion::operator*(double r)
{
    return Quaternion(x*r, y*r, z*r, w*r);
}

Quaternion Quaternion::operator*(const Quaternion &b)
{
    return Quaternion(
        y*b.z -z*b.y +x*b.w +w*b.x,
        z*b.x -x*b.z +y*b.w +w*b.y,
        x*b.y -y*b.x +z*b.w +w*b.z,
        w*b.w -y*b.y -z*b.z -x*b.x);
}

Quaternion Quaternion::operator+(const Quaternion &b)
{
    return Quaternion(x+b.x, y+b.y, z+b.z, w+b.w);
}


Quaternion Quaternion::normal()
{
    float length1 = 1/sqrt(x*x + y*y + z*z + w*w);
    return Quaternion(length1*x, length1*y, length1*z, length1*w);
}


Quaternion QfromRotation(float x, float y, float z, float alpha)
{
    float length = sqrt(x*x + y*y + z*z);
    x /= length;
    y /= length;
    z /= length;
    float sin2 = sin(alpha/2);
    return Quaternion(sin2*x, sin2*y, sin2*z, cos(alpha/2));
}

double dot(const Quaternion &a, const Quaternion &b){
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}


Quaternion slerp(Quaternion &a, Quaternion &b, float t)
{
    float cosa = dot(a,b);
    // float theta = acos(cosa);
    float sina = sqrt(1-cosa*cosa);
    float sa = acos(cosa);
    // printf("%f %f %f\n\n", t, , sin(sa*(t)));
    return ((a*sin(sa*(1-t)) + b*sin(sa*(t)))) *(1.0f/sina);//.normal();
}


void FrameTransformation::display()
{
    float mat[16];
    rotation.toMatrix(mat);
    // glPushMatrix();
    glTranslatef(position[0], position[1], position[2]);
    glMultMatrixf(mat);
    glScalef(shear[0], shear[1], shear[2]);
    // glPopMatrix();
}

// FrameTransformation interp(FrameTransformation &a, FrameTransformation &b){
//     FrameTransformation ret;
//     ret.position = 
// }
FrameTransformation interp(FrameTransformation &a, FrameTransformation &b, float p){
    FrameTransformation ret;
    ret.rotation = slerp(a.rotation, b.rotation, p);
    ret.position[0] = (1-p)*a.position[0] + p*b.position[0];
    ret.position[1] = (1-p)*a.position[1] + p*b.position[1];
    ret.position[2] = (1-p)*a.position[2] + p*b.position[2];
    ret.shear[0] = (1-p)*a.shear[0] + p*b.shear[0];
    ret.shear[1] = (1-p)*a.shear[1] + p*b.shear[1];
    ret.shear[2] = (1-p)*a.shear[2] + p*b.shear[2];
    return ret;
}

void FrameTransformation::toMatrix(float *matrix){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    display();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
}

void FrameTransformation::translate(float x, float y, float z)
{
    position[0] += x;
    position[1] += y;
    position[2] += z;
}

void FrameTransformation::rotate(Quaternion b){
    printf("rotate Q %f %f %f %f\n", b.x, b.y, b.z, b.w);
    rotation = (rotation * b.normal()).normal();
}
