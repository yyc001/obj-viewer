#include <GL/glut.h>

class Windowing{
    public:
    camera camera;
    std::vector<*Trimesh> meshes;
    int active_mesh;

    void display(){
	    //cout << " ??? " << endl;
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
        // myReshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // glPushMatrix();
        // glPopMatrix();
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        Trimesh* mesh = NULL;
        if(meshes.size() > 0)
        {
            mesh = meshes.back();
        }

        // void gluLookAt(
        //     GLdouble eyex, GLdouble eyey,GLdouble eyez,
        //     GLdouble centerx, GLdouble centery, GLdouble centerz,
        //     GLdouble upx, GLdouble upy,GLdouble upz
        // );

        if(this->camera.mode == CAMERA_FREE) {
            gluLookAt(
                this->camera.eye.x, this->camera.eye.y, this->camera.eye.z,
                this->camera.at.x, this->camera.at.y, this->camera.at.z,
                0.0, 1.0, 0.0
            );
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            // gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
            gluPerspective(this->camera.fovy, 1.0*this->camera.width/this->camera.height, this->camera.near, this->camera.far);
            glMatrixMode(GL_MODELVIEW);
        } else if(this->camera.mode == CAMERA_PERSPECTIVE) {
            gluLookAt(
                this->camera.eye.x, this->camera.eye.y, this->camera.eye.z,
                0.0, 0.0, 0.0,
                this->camera.up.x, this->camera.up.y, this->camera.up.z
            );
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            // gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
            gluPerspective(this->camera.fovy, 1.0*this->camera.width/this->camera.height, this->camera.near, this->camera.far);
            glMatrixMode(GL_MODELVIEW);
            // gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
            // glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);

        } else if(this->camera.mode == CAMERA_ORTHO) {
            gluLookAt(
                this->camera.eye.x, this->camera.eye.y, this->camera.eye.z,
                0.0, 0.0, 0.0,
                this->camera.up.x, this->camera.up.y, this->camera.up.z
            );
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            // gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
            gluPerspective(this->camera.fovy, 1.0*this->camera.width/this->camera.height, this->camera.near, this->camera.far);
            glMatrixMode(GL_MODELVIEW);
            glOrtho(left, right, bottom, top, zNear, nFar);
            // glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble nFar);
        }

        if(mesh != NULL)
            mesh->drawFaces();
        glDisable(GL_LIGHTING);
        glFlush();
        glutSwapBuffers();
    }

    void reshape(int width, int height){
        this->camera.width = width;
        this->camere.height = height;

        glViewport(0, 0, width, height);
        // glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
    }
    void mainLoop(){
        glutReshapeFunc(this->reshape);
	    glutDisplayFunc(this->display);
	    // glutMouseFunc(this->mouse);
	    // glutKeyboardFunc(this->keyboard);
	    // glutMotionFunc(this->mouseMotion);
	    // glutIdleFunc(this->idle);
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_COLOR_MATERIAL);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	    glutMainLoop();	
    }
};

Windowing *mWindow;

void windowingDisplay(){
    mWindow->display();
}

void windowingMouse(){
    mWindow->mouse();
}