#include <time.h>
#include <GL/glut.h>
#include<math.h>
#include "animation.h"
#include<cstdio>

AnimationController::AnimationController()
{
    btick = clock();
}
// int idle()

FrameTransformation AnimationController::getNowFrame()
{
    ntick = clock();
    // printf("ac!\n");
    float p;
    if(repeat){
        p = ((ntick - btick)%rtime)*1.0f/rtime;
    } else {
        p = (ntick - btick)*1.0f/rtime;
        if(p>1) p=1;
        // if(p<0) p=0;
    }
    if(type == ANIMATION_LINEAR) {
        return interp(sframe, eframe, p);
    }
    if(type == ANIMATION_BEZIER) {
        FrameTransformation f1,f2,f3,f4,f5,f6;
        f1 = interp(frames[0], frames[1], p);
        f2 = interp(frames[1], frames[2], p);
        f3 = interp(frames[2], frames[3], p);
        f4 = interp(f1, f2, p);
        f5 = interp(f2, f3, p);
        f6 = interp(f4, f5, p);
        return f6;
    }
    return sframe;
}

void AnimationController::display()
{
    // glLoadIdentity();
    FrameTransformation now = getNowFrame();
    now.display();
    // interp(frames[0], frames[1], p).display();
}
