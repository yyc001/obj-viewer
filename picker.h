#ifndef __PICKER__
#define __PICKER__

#include "camera.h"
#include "mesh.h"

void getPixelObj(int x, int y, float *objx, float *objy, float *objz);

class Picker{
public:
    std::vector<Trimesh*> meshes;
    Camera *camera;
    int mode;
    Trimesh *picked_save;

    Trimesh *pick(float pos[3], float eye[3]);
    void mouse(int button, int state, int x, int y);
    void special(int key, int x, int y);
};

#endif