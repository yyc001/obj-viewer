#include <GL/glut.h>
#include "picker.h"
#include "mesh.h"
#include "base.h"
#include <cstdio>

void getPixelObj(int x, int y, float *objx, float *objy, float *objz)
{
    // printf("1\n");
    double modelview[16], projection[16];//模型投影矩阵
    int viewport[4];//视口
    float ScreenZ;//深度值
    double dobjx, dobjy, dobjz;
    glGetDoublev( GL_PROJECTION_MATRIX, projection );//获得投影矩阵
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );//获得模型矩阵
    glGetIntegerv( GL_VIEWPORT, viewport );	//获得视口
    glReadPixels( x, viewport[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &ScreenZ ); //获得屏幕像素对应的世界坐标深度值  
    // printf("2\n");
    gluUnProject( x, viewport[3]-y, ScreenZ , modelview, projection, viewport, &dobjx, &dobjy, &dobjz);//获得屏幕坐标对应的世界坐标
    // printf("3\n");
    *objx = dobjx;
    *objy = dobjy;
    *objz = dobjz;
}

Trimesh *Picker::pick(float pos[3], float eye[3])
{
    Trimesh *ret = NULL; 
    float distance = 1e9;
    for(Trimesh *mesh: meshes){
        vector<float> middle = mesh->middlePoint();
        // printf("middle %f %f %f\n", middle[0], middle[1], middle[2]);
        float diam = mesh->diam()/2;
        if(vecLen32(middle[0]-pos[0], middle[1]-pos[1], middle[2]-pos[2]) < diam*diam){
            float nowdis = vecLen32(middle[0]-eye[0], middle[1]-eye[1], middle[2]-eye[2]);
            if(nowdis < distance) {
                distance = nowdis;
                ret = mesh;
            }
        }
    }
    // printf("\n");
    return ret;
}

const float RGBA_RED[4] = {1.0,0.0,0.0,1.0};
const float RGBA_WHITE[4] = {1.0,1.0,1.0,1.0};

void Picker::mouse(int button, int state, int x, int y)
{
    if(button==0 && state==1 && (mode&1)){
        // printf("zmhsn\n");
        if(picked_save) {
            // memcpy(picked_save->rgba, RGBA_WHITE, sizeof(RGBA_WHITE));
            picked_save->rgba[0] = RGBA_WHITE[0];
            picked_save->rgba[1] = RGBA_WHITE[1];
            picked_save->rgba[2] = RGBA_WHITE[2];
            picked_save->rgba[3] = RGBA_WHITE[3];
        }
        // printf("eek\n");
        float obj[3];
        getPixelObj(x, y, obj, obj+1, obj+2);
        Trimesh *picked = pick(obj, camera->eye);
        printf("Pick %f %f %f\n", obj[0], obj[1], obj[2]);
        if(picked){
            picked->rgba[0] = RGBA_RED[0];
            picked->rgba[1] = RGBA_RED[1];
            picked->rgba[2] = RGBA_RED[2];
            picked->rgba[3] = RGBA_RED[3];
        }
        picked_save = picked;
    }
}

void Picker::special(int key, int x, int y)
{
    switch(key){
        case GLUT_KEY_F2:
        mode = 1;
        break;
        case GLUT_KEY_F3:
        mode = 2;
        break;
        case GLUT_KEY_F4:
        mode = 4;
        break;
    }
}

void Picker::motion(int x, int y, int pressed)
{
    
    if(pressed && picked_save){
        float up[3], front[3], left[3];
        up[0] = camera->up[0];
        up[1] = camera->up[1];
        up[2] = camera->up[2];
        front[0] = camera->at[0] - camera->eye[0];
        front[1] = camera->at[1] - camera->eye[1];
        front[2] = camera->at[2] - camera->eye[2];
        float dp = sin(camera->fovy/180*PI)*camera->near/2;
        float dx = (save_x - x)*2.0f/(dp*camera->width);
        float dy = (save_y - y)*2.0f/(dp*camera->height);
        vecCross3(left, up, front);
        vecCross3(up, front, left);
        vecNormalize3(left);
        vecNormalize3(front);
        vecNormalize3(up);
        if(mode&1){
            // picked_save->prepareTransformation();
        }
        if(mode&2){ // row
            float rotate[3], rt[3];
            rotate[0] = left[0]*dx+up[0]*dy;
            rotate[1] = left[1]*dx+up[1]*dy;
            rotate[2] = left[2]*dx+up[2]*dy;
            vecCross3(rt, rotate, front);
            picked_save->frame.rotate(QfromRotation(rt[0], rt[1], rt[2], (abs(dx)+abs(dy))*(0.1f)));
        }
        if(mode&4){ // otrho
            // printf("left  %f %f %f\n", left[0], left[1], left[2]);
            // printf("up    %f %f %f\n", up[0], up[1], up[2]);
            // printf("front %f %f %f\n", front[0], front[1], front[2]);
            // printf("apply %f %f %f\n", left[0]*dx+up[0]*dy, left[1]*dx+up[1]*dy, left[2]*dx+up[2]*dy);
            picked_save->frame.translate(left[0]*dx+up[0]*dy, left[1]*dx+up[1]*dy, left[2]*dx+up[2]*dy);
        }
    }
    save_x = x;
    save_y = y;
}