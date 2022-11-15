#ifndef __ANIMATION__
#define __ANIMATION__

#include <vector>

struct Quaternion{
    float x,y,z,w;
    Quaternion(float x, float y, float z, float w);
    Quaternion();
    void toMatrix(float *matrix);

    Quaternion operator*(double r);
    Quaternion operator+(const Quaternion &b);

    Quaternion normal();
};

Quaternion QfromRotation(float x, float y, float z, float alpha);
double dot(const Quaternion &a, const Quaternion &b);


Quaternion slerp(Quaternion &a, Quaternion &b, float t);

struct FrameTransformation{
    Quaternion rotation;
    float position[3];
    // float shear[3];
    // float transformation_mat[16];
    void toMatrix(float *matrix);
    void display();

};

// FrameTransformation interp(FrameTransformation &a, FrameTransformation &b){
//     FrameTransformation ret;
//     ret.position = 
// }
FrameTransformation interp(FrameTransformation &a, FrameTransformation &b, float p);
#define ANIMATION_LINEAR 1
#define ANIMATION_BEZIER 2
class AnimationController{
    public:
    // Trimesh *mesh;
    std::vector<FrameTransformation> frames;
    FrameTransformation sframe, eframe;
    int rtime, btick, ntick, repeat;
    int type;
    float nmat[16];
    AnimationController();
    // int idle()
    void display_begin();
    void display_end();
    FrameTransformation getNowFrame();
};

#endif