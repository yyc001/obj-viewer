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
    int save_x, save_y;

    Trimesh *pick(float pos[3], float eye[3]);
    void mouse(int button, int state, int x, int y);
    void special(int key, int x, int y);
    void motion(int x, int y, int pressed);
};

#endif