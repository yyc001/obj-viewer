
struct vec3d{
    float x,y,z;
};
// typedef float[3] vec3df;

struct triangle{
    vec3d v1,v2,v3;
};

struct camera{
    vec3d eye, at, up;
    int mode, width, height;
    double fogy, near, far;
};
#define CAMERA_PERSPECTIVE 1
#define CAMERA_ORTHO 2
#define CAMERA_FREE 3