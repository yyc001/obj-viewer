#ifndef __BASE__
#define __BASE__

void vecCross3(float *dst, const float *a, const float *b);
float vecLen32(float x, float y, float z);
void vecNormalize3(float *a);
void vecNormalize2(float *a);

void menv_begin();
void menv_end();
void menv_mul(float* matrix);
void menv_get(float* matrix);

extern double PI;

struct Quaternion{
    float x,y,z,w;
    Quaternion(float x, float y, float z, float w);
    Quaternion();
    void toMatrix(float *matrix);

    Quaternion operator*(double r);
    Quaternion operator*(const Quaternion &b);
    Quaternion operator+(const Quaternion &b);

    Quaternion normal();
};

Quaternion QfromRotation(float x, float y, float z, float alpha);
double dot(const Quaternion &a, const Quaternion &b);

Quaternion slerp(Quaternion &a, Quaternion &b, float t);

struct FrameTransformation{
    Quaternion rotation;
    float position[3];
    float shear[3];
    // float transformation_mat[16];
    void toMatrix(float *matrix);
    void display();
    void translate(float x, float y, float z);
    void rotate(Quaternion b);

};

// FrameTransformation interp(FrameTransformation &a, FrameTransformation &b){
//     FrameTransformation ret;
//     ret.position = 
// }
FrameTransformation interp(FrameTransformation &a, FrameTransformation &b, float p);

#endif