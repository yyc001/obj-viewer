#include "base.h"
#include <math.h>
#include<stdio.h>
#include <GL/glut.h>
#include "camera.h"
#include <cstring>

void Camera::info(){
    printf("camera %p:\n", this);
    printf("   eye %f %f %f\n", eye[0], eye[1], eye[2]);
    printf("   at  %f %f %f\n", at[0], at[1], at[2]);
    printf("   up  %f %f %f\n", up[0], up[1], up[2]);
    printf("fovy=%f  [n,f]=[%f,%f]\n", fovy, near, far);
    printf("mode=%d  [w,h]=[%2d,%2d] ditry=%d\n", mode, width, height, dirty);
}

CameraController::CameraController()
{
    freealpha = freetheta = 1;
    deltastep[0] = 1;
    deltastep[1] = 1;
    deltastep[2] = 1;
    camera = nullptr;
}

void CameraController::setCameraMat()
{
    glMatrixMode(GL_MODELVIEW);
    // if(1)
    if(camera->dirty)
    {
        glLoadIdentity();
        gluLookAt(
            camera->eye[0], camera->eye[1], camera->eye[2],
            camera->at[0], camera->at[1], camera->at[2],
            camera->up[0], camera->up[1], camera->up[2]
            );
        camera->dirty = 0;
        glGetFloatv(GL_MODELVIEW_MATRIX, camera->savedmat);
        // printf("reload camera mat.\n");
    } else {
        glLoadMatrixf(this->camera->savedmat);
    }
}

void CameraController::reshape(int width, int height)
{
    camera->width = width;
    camera->height = height;
    if(camera->mode == CAMERA_FREE || camera->mode == CAMERA_PERSPECTIVE) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
        gluPerspective(camera->fovy, 1.0*camera->width/camera->height, camera->near, camera->far);
        glMatrixMode(GL_MODELVIEW);
    } else if(camera->mode == CAMERA_ORTHO) {
        float dp = sin(camera->fovy/180*PI)*camera->near/2;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble nFar);
        glOrtho(
            -camera->width*dp, camera->width*dp,
            -camera->height*dp, camera->height*dp,
            camera->near, camera->far
            );
        glMatrixMode(GL_MODELVIEW);
    }
}

int CameraController::motion(int x, int y, int pressed)
{
    if(camera->mode == CAMERA_FREE) {
        if(pressed == 0){
            savedx = x;
            savedy = y;
            return 0;
        }
        int width = camera->width, height = camera->height;
        int dx = savedx - x , dy = savedy - y;
        if(abs(dx) < width/100 && abs(dy) < height/100){
            return 0;
            //ignore
        }
        float dalpha = (camera->fovy/180*PI)*dx/width;
        float dtheta = (camera->fovy/180*PI)*dy/height;
        freealpha += dalpha ;
        freetheta += dtheta ;
        if(freetheta > PI/2) freetheta = PI/2;
        if(freetheta < -PI/2) freetheta = -PI/2;

        // float up[3], front[3], left[3];
        // up[0] = 0;
        // up[1] = 1;
        // up[2] = 0;
        // front[0] = camera->at[0] - camera->eye[0];
        // front[1] = camera->at[1] - camera->eye[1];
        // front[2] = camera->at[2] - camera->eye[2];
        // vecNormalize3(front);
        // vecCross3(left, up, front);

        camera->at[0] = camera->eye[0] + camera->far/2*cos(freealpha)*cos(freetheta);
        camera->at[1] = camera->eye[1] + camera->far/2*sin(freetheta);
        camera->at[2] = camera->eye[2] + camera->far/2*sin(-freealpha)*cos(freetheta);
        camera->dirty = 1;
        // printf("x=%2d, y=%2d, sx=%2d, sy=%2d\n", x, y, savedx, savedy);
        savedx = x;
        savedy = y;
        // camera->info();
        // printf("a=%f t%f\n", freealpha, freetheta);
        // glutWarpPointer(width / 2, height / 2);
        return 1;
    }
    return 0;
}

int CameraController::keyboard(unsigned char key, int x, int y)
{
    printf("keyboard [%c] %d %d %d\n", key, key, x, y);
    switch(key){
    case 'r':
    case 'R':
        memcpy(camera, &reset, sizeof(reset));
        camera->dirty=1;
        return 1;
    case 'i':
    case 'I':
        camera->info();
        return 0;
    }

    float up[3], front[3], left[3];
    if(camera->mode == CAMERA_PERSPECTIVE || camera->mode == CAMERA_ORTHO) {
        up[0] = camera->up[0];
        up[1] = camera->up[1];
        up[2] = camera->up[2];
        front[0] = camera->at[0] - camera->eye[0];
        front[1] = camera->at[1] - camera->eye[1];
        front[2] = camera->at[2] - camera->eye[2];
        vecCross3(left, up, front);
        vecCross3(up, front, left);
        vecNormalize3(left);
        vecNormalize3(front);
        vecNormalize3(up);
        switch(key){
        case ' ':
            camera->eye[0] += up[0]*deltastep[1];
            camera->eye[1] += up[1]*deltastep[1];
            camera->eye[2] += up[2]*deltastep[1];
            camera->dirty = 1;
            return 1;
        case 9: // tab
            camera->eye[0] -= up[0]*deltastep[1];
            camera->eye[1] -= up[1]*deltastep[1];
            camera->eye[2] -= up[2]*deltastep[1];
            camera->dirty = 1;
            return 1;
        case 'A':
        case 'a':
            camera->eye[0] += left[0]*deltastep[0];
            camera->eye[1] += left[1]*deltastep[0];
            camera->eye[2] += left[2]*deltastep[0];
            camera->dirty = 1;
            return 1;
        case 'D':
        case 'd':
            camera->eye[0] -= left[0]*deltastep[0];
            camera->eye[1] -= left[1]*deltastep[0];
            camera->eye[2] -= left[2]*deltastep[0];
            camera->dirty = 1;
            return 1;
        case 'W':
        case 'w':
            camera->eye[0] += front[0]*deltastep[0];
            camera->eye[1] += front[1]*deltastep[0];
            camera->eye[2] += front[2]*deltastep[0];
            camera->dirty = 1;
            return 1;
        case 'S':
        case 's':
            camera->eye[0] -= front[0]*deltastep[0];
            camera->eye[1] -= front[1]*deltastep[0];
            camera->eye[2] -= front[2]*deltastep[0];
            camera->dirty = 1;
            return 1;
        }
    } else if (camera->mode == CAMERA_FREE) {
        camera->up[0] = up[0] = 0;
        camera->up[1] = up[1] = 1;
        camera->up[2] = up[2] = 0;
        front[0] = camera->at[0] - camera->eye[0];
        front[1] = camera->at[1] - camera->eye[1];
        front[2] = camera->at[2] - camera->eye[2];
        vecNormalize3(front);
        vecCross3(left, up, front);
        vecCross3(front, left, up);
        switch(key){
        case ' ':
            camera->eye[0] += up[0]*deltastep[1];
            camera->eye[1] += up[1]*deltastep[1];
            camera->eye[2] += up[2]*deltastep[1];
            camera->at[0] += up[0]*deltastep[1];
            camera->at[1] += up[1]*deltastep[1];
            camera->at[2] += up[2]*deltastep[1];
            camera->dirty = 1;
            return 1;
        case 9: // tab
            camera->eye[0] -= up[0]*deltastep[1];
            camera->eye[1] -= up[1]*deltastep[1];
            camera->eye[2] -= up[2]*deltastep[1];
            camera->at[0] -= up[0]*deltastep[1];
            camera->at[1] -= up[1]*deltastep[1];
            camera->at[2] -= up[2]*deltastep[1];
            camera->dirty = 1;
            return 1;
        case 'A':
        case 'a':
            camera->eye[0] += left[0]*deltastep[0];
            camera->eye[1] += left[1]*deltastep[0];
            camera->eye[2] += left[2]*deltastep[0];
            camera->at[0] += left[0]*deltastep[0];
            camera->at[1] += left[1]*deltastep[0];
            camera->at[2] += left[2]*deltastep[0];
            camera->dirty = 1;
            return 1;
        case 'D':
        case 'd':
            camera->eye[0] -= left[0]*deltastep[0];
            camera->eye[1] -= left[1]*deltastep[0];
            camera->eye[2] -= left[2]*deltastep[0];
            camera->at[0] -= left[0]*deltastep[0];
            camera->at[1] -= left[1]*deltastep[0];
            camera->at[2] -= left[2]*deltastep[0];
            camera->dirty = 1;
            return 1;
        case 'W':
        case 'w':
            camera->eye[0] += front[0]*deltastep[0];
            camera->eye[1] += front[1]*deltastep[0];
            camera->eye[2] += front[2]*deltastep[0];
            camera->at[0] += front[0]*deltastep[0];
            camera->at[1] += front[1]*deltastep[0];
            camera->at[2] += front[2]*deltastep[0];
            camera->dirty = 1;
            return 1;
        case 'S':
        case 's':
            camera->eye[0] -= front[0]*deltastep[0];
            camera->eye[1] -= front[1]*deltastep[0];
            camera->eye[2] -= front[2]*deltastep[0];
            camera->at[0] -= front[0]*deltastep[0];
            camera->at[1] -= front[1]*deltastep[0];
            camera->at[2] -= front[2]*deltastep[0];
            camera->dirty = 1;
            return 1;
        }
    }
    // camera->info();
    return 0;
}

int CameraController::special(int key, int x, int y)
{
    printf("special %d %d %d\n", key, x, y);
    switch(key){
    case GLUT_KEY_F1:
        printf("hello\n");
        return 0;
    case GLUT_KEY_F2:
        camera->mode = CAMERA_FREE;
        reshape(camera->width, camera->height);
        // from camera to angle
        float shee[3];
        shee[0] = (camera->at[0]-camera->eye[0]);
        shee[1] = (-camera->at[2]+camera->eye[2]);
        // shee[2] = (camera->at[2]-camera->eye[2]);
        vecNormalize2(shee);
        freealpha = acos(shee[0]);
        if(shee[1] < 0) freealpha = 2*PI - freealpha;
        shee[0] = (camera->at[0]-camera->eye[0]);
        shee[1] = (camera->at[1]-camera->eye[1]);
        shee[2] = (camera->at[2]-camera->eye[2]);
        vecNormalize3(shee);
        freetheta = asin(shee[1]);
        return 1;
    case GLUT_KEY_F3:
        camera->mode = CAMERA_PERSPECTIVE;
        reshape(camera->width, camera->height);
        return 1;
    case GLUT_KEY_F4:
        camera->mode = CAMERA_ORTHO;
        reshape(camera->width, camera->height);
        return 1;
    }
    return 0;
}