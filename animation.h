#ifndef __ANIMATION__
#define __ANIMATION__

#include <vector>
#include "base.h"

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
    void display();
    FrameTransformation getNowFrame();
};

#endif