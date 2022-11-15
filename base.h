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

#endif