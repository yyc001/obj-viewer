#include <GL/glut.h>  
#include <stdlib.h> 
#include<stdio.h>
void display(void) 
{    
	printf("display.\n");
    glClearColor(1,1,1,0);
	glClear (GL_COLOR_BUFFER_BIT);  
	glColor3f (0, 0, 1.0); //画笔蓝色   
	glLoadIdentity();  //加载单位矩阵   
	gluLookAt(0.0,5.0,0.0,  0,0,0, 1.0,0.0,0);   
	glutWireTeapot(1.3);
	glutSwapBuffers();
}  

void reshape (int w, int h)
{    
    printf("reshape.\n");
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();   
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 4, 10.0);
	glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity(); 
	// gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}    

int main(int argc, char** argv)   
{    
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (500, 500);  
	glutInitWindowPosition (100, 100);  
	glutCreateWindow ("gluPerspective ( X, X, 4,10 )");   
	glutDisplayFunc(display);  
	glutReshapeFunc(reshape);  
	glutMainLoop();
	return 0;
} 