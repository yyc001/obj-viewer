#ifndef __WINDOWING__
#define __WINDOWING__

#include "base.h"
#include "mesh.h"
#include "camera.h"
#include "picker.h"

class Windowing{
    public:
    CameraController *cc;
    std::vector< Trimesh* > meshes;
    int active_mesh, fps, ltick;
    Picker *picker;
    int light;

    void init();

    // void setCameraController(CameraController *cc);

    void display();

    void reshape(int width, int height);

    void keyboard(unsigned char key, int x, int y);

    void motion(int x, int y);

    void passiveMotion(int x, int y);

    void mouse(int button, int state, int x, int y);

    void special(int key, int x, int y);

    void idle();

    void mainLoop();
};

// extern Windowing *mWindow;

// void windowingDisplay();

// void windowingReshape(int w, int h);

// void windowingMouse(int button, int state, int x, int y);

// void windowingMotion(int x, int y);

// void windowingPassiveMotion();

// void windowingKeyboard(unsigned char key, int x, int y);

// void windowingIdle();

#endif
