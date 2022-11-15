#include <time.h>
#include <GL/glut.h>
#include<math.h>
#include "animation.h"
#include<cstdio>
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
    // suofang 

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
    return ret;
}

AnimationController::AnimationController()
{
    btick = clock();
}
// int idle()

FrameTransformation AnimationController::getNowFrame()
{
    ntick = clock();
    // printf("ac!\n");
    float p;
    if(repeat){
        p = ((ntick - btick)%rtime)*1.0f/rtime;
    } else {
        p = (ntick - btick)*1.0f/rtime;
        if(p>1) p=1;
        // if(p<0) p=0;
    }
    if(type == ANIMATION_LINEAR) {
        return interp(sframe, eframe, p);
    }
    if(type == ANIMATION_BEZIER) {
        FrameTransformation f1,f2,f3,f4,f5,f6;
        f1 = interp(frames[0], frames[1], p);
        f2 = interp(frames[1], frames[2], p);
        f3 = interp(frames[2], frames[3], p);
        f4 = interp(f1, f2, p);
        f5 = interp(f2, f3, p);
        f6 = interp(f4, f5, p);
        return f6;
    }
    return sframe;
}

void AnimationController::display_begin()
{

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    // glLoadIdentity();
    FrameTransformation now = getNowFrame();
    now.display();
    // interp(frames[0], frames[1], p).display();
}
void AnimationController::display_end()
{
    glPopMatrix();
}


void FrameTransformation::toMatrix(float *matrix){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    display();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
}