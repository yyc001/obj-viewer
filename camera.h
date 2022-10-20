#ifndef __CAMERA__
#define __CAMERA__

#include "base.h"
#include <math.h>
#include<stdio.h>

#define CAMERA_PERSPECTIVE 1
#define CAMERA_ORTHO 2
#define CAMERA_FREE 3

struct Camera{
    float eye[3], at[3], up[3];
    int mode, width, height, dirty;
    double fovy, near, far;
    float savedmat[16];
    
    void info(){
        printf("camera %p:\n", this);
        printf("   eye %f %f %f\n", eye[0], eye[1], eye[2]);
        printf("   at  %f %f %f\n", at[0], at[1], at[2]);
        printf("   up  %f %f %f\n", up[0], up[1], up[2]);
        printf("fovy=%f  [n,f]=[%f,%f]\n", fovy, near, far);
        printf("mode=%d  [w,h]=[%2d,%2d] ditry=%d\n", mode, width, height);
    }
};

class CameraController{
    public:
    struct Camera *camera;
    struct Camera reset;
    float freealpha, freetheta;
    float deltastep[3];
    int savedx, savedy;

    CameraController()
    {
        freealpha = freetheta = 1;
        deltastep[0] = 1;
        deltastep[1] = 1;
        deltastep[2] = 1;
        camera = nullptr;
    }
    
    void setCameraMat()
    {
        glMatrixMode(GL_MODELVIEW);
        if(1)
        // if(camera->dirty)
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

    void reshape(int width, int height)
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
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            // glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble nFar);
            glOrtho(
                -camera->width, camera->width,
                -camera->height, camera->height,
                camera->near, camera->far
                );
            glMatrixMode(GL_MODELVIEW);
        }
    }

    int motion(int x, int y, int pressed)
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

    int keyboard(unsigned char key, int x, int y)
    {
        printf("keyboard %d %d %d\n", key, x, y);
        switch(key){
        case 'r':
        case 'R':
            memcpy(camera, &reset, sizeof(reset));
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
                break;
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

    void special(int key, int x, int y)
    {
        printf("special %d %d %d\n", key, x, y);
        switch(key){
        case GLUT_KEY_F1:
            printf("hello\n");
            break;
        case GLUT_KEY_F2:
            camera->mode = CAMERA_FREE;
            // from camera to angle
            break;
        case GLUT_KEY_F3:
            camera->mode = CAMERA_PERSPECTIVE;
            break;
        case GLUT_KEY_F4:
            camera->mode = CAMERA_ORTHO;
            break;
        }
    }

};

#endif