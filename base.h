#ifndef __BASE__
#define __BASE__
struct vec3d{
    float x,y,z;
};
// typedef float[3] vec3df;

#define CAMERA_PERSPECTIVE 1
#define CAMERA_ORTHO 2
#define CAMERA_FREE 3

struct Camera{
    vec3d eye, at, up;
    int mode, width, height;
    double fovy, near, far;
};

#endif