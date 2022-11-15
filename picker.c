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
        float diam = mesh->diam();
        if(vecLen32(middle[0]-pos[0], middle[1]-pos[1], middle[2]-pos[2]) < diam*diam){
            float nowdis = vecLen32(middle[0]-eye[0], middle[1]-eye[1], middle[2]-eye[2]);
            if(nowdis < distance) {
                distance = nowdis;
                ret = mesh;
            }
        }
    }
    return ret;
}

const float RGBA_RED[4] = {1.0,0.0,0.0,1.0};
const float RGBA_WHITE[4] = {1.0,1.0,1.0,1.0};

void Picker::mouse(int button, int state, int x, int y)
{
    if(button==0 && state==1 && (mode&1)){
        // printf("zmhsn\n");
        if(picked_save) {
            memcpy(picked_save->rgba, RGBA_WHITE, sizeof(RGBA_WHITE));
        }
        // printf("eek\n");
        float obj[3];
        getPixelObj(x, y, obj, obj+1, obj+2);
        Trimesh *picked = pick(obj, camera->eye);
        printf("Pick %f %f %f\n", obj[0], obj[1], obj[2]);
        if(picked){
            printf("Pick!\n");
            memcpy(picked->rgba, RGBA_RED, sizeof(RGBA_RED));
            picked_save = picked;
        }
    }
}

void Picker::special(int key, int x, int y)
{
    switch(key){
        case GLUT_KEY_F2:
        mode = 1;
        break;
        case GLUT_KEY_F3:
        case GLUT_KEY_F4:
        mode = 0;
        break;
    }
}