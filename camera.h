#ifndef __CAMERA__
#define __CAMERA__

#define CAMERA_PERSPECTIVE 1
#define CAMERA_ORTHO 2
#define CAMERA_FREE 3

struct Camera{
    float eye[3], at[3], up[3];
    int mode, width, height, dirty;
    double fovy, near, far;
    float savedmat[16];
    
    void info();
};

class CameraController{
    public:
    struct Camera *camera;
    struct Camera reset;
    float freealpha, freetheta;
    float deltastep[3];
    int savedx, savedy;

    CameraController();
    
    void setCameraMat();

    void reshape(int width, int height);

    int motion(int x, int y, int pressed);

    int keyboard(unsigned char key, int x, int y);

    int special(int key, int x, int y);

};

#endif