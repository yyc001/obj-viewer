#include <GL/glut.h>
#include "windowing.h"
#include <cstdio>
#include <time.h>
#include "base.h"
#include "mesh.h"


void Windowing::init()
{
	glutInitWindowSize(400.f, 300.f);
	glutInitWindowPosition(100,100);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glutCreateWindow("Object loader");
	glutInitDisplayMode(GLUT_DOUBLE);
	
    glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//glDisable(GL_CULL_FACE);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);

}

// void setCameraController(CameraController *cc)
// {
//     this->cc = cc;
// }

void Windowing::display()
{
    // return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    if(cc){
        this->cc->setCameraMat();
    } else {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1,1,-1,1,-1,1);
        glMatrixMode(GL_MODELVIEW);
    }

    for(Trimesh *mesh: this->meshes) {
        if(mesh != NULL){
            mesh->display();
        }
    }
    // printf("\n");

    // //要显示的字符
    // char *str = "current fps = ";
    // int n = strlen(str);
    // //设置要在屏幕上显示字符的起始位置
    // glRasterPos2i(0,0);
    // //逐个显示字符串中的每个字符
    // for (int i = 0; i < n; i++)
    //     glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *(str+i));
    glDisable(GL_LIGHTING);
    // glFlush();
    glutSwapBuffers();
    glFlush();
}

void Windowing::reshape(int width, int height){
    glViewport(0, 0, width, height);
    // glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
    this->cc->reshape(width,height);
}

void Windowing::keyboard(unsigned char key, int x, int y)
{
    // printf("keyboard %c %d %d\n", key, x, y);
    if(cc){
        if(cc->keyboard(key, x,y)){
            // display();
        }
    }
    if(key == '-'){
        // printf("")
        exit(0);
    }
}

void Windowing::motion(int x, int y)
{
    // printf("motion %d %d\n", x, y);
    if(cc){
        if(cc->motion(x,y, 1)){
            // display();
        }
    }
}

void Windowing::passiveMotion(int x, int y)
{
    // printf("passiveMotion %d %d\n", x, y);
    if(cc){
        if(cc->motion(x,y, 0)){
            // display();
        }
    }
}

void Windowing::mouse(int button, int state, int x, int y)
{
    printf("mouse %d %d %d %d\n", button, state, x, y);
    if(picker){
        // printf("picker.\n");
        picker->mouse(button, state, x, y);
    }
}

void Windowing::special(int key, int x, int y)
{
    printf("special %d %d %d\n", key, x, y);
    if(cc){
        if(cc->special(key, x, y)){
            // display();
        }
    }
    if(picker){
        picker->special(key, x, y);
    }
}

void Windowing::idle()
{
    // printf("idle\n");
    if(fps > 0){
        int tick = clock();
        // printf("%d %d %f\n", tick-ltick, CLOCKS_PER_SEC/fps, 1.0f*CLOCKS_PER_SEC/(tick-ltick));
        if(tick-ltick > CLOCKS_PER_SEC/fps) {
            display();
            ltick = tick;
        }
    }
}
Windowing *mWindow;
void Windowing::mainLoop() {
    mWindow = this;
    glutReshapeFunc( [](int w, int h){mWindow->reshape(w, h);} );
    glutDisplayFunc([](){mWindow->display();});
    glutMouseFunc([](int button, int state, int x, int y){mWindow->mouse(button, state, x, y);});
    glutKeyboardFunc([](unsigned char key, int x, int y){mWindow->keyboard(key, x, y);});
    glutSpecialFunc([](int key, int x, int y){mWindow->special(key, x, y);});
    glutMotionFunc([](int x, int y){mWindow->motion(x, y);});
    glutPassiveMotionFunc([](int x, int y){mWindow->passiveMotion(x, y);});
    glutIdleFunc([](){mWindow->idle();});
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glutMainLoop();	
}
