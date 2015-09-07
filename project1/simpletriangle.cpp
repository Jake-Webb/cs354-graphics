#include <GL/glut.h>  // includes necessary OpenGL headers

void display() { 
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor4ub(0, 0, 0, 255);  
    glBegin(GL_TRIANGLES);  // every 3 vertexes makes a triangle
        glVertex3f(-0.9,  0.8, 0); 
        glVertex3f(-0.75,  0.8, 0);  
        glVertex3f(-0.45, -0.8, 0); 

        glVertex3f(-0.75,  0.8, 0); 
        glVertex3f(-0.425, -0.5, 0); 
        glVertex3f(-0.45, -0.8, 0);  

        glVertex3f(0, -0.1, 0); 
        glVertex3f(-0.425, -0.5, 0);  
        glVertex3f(-0.45, -0.8, 0);  

        glVertex3f(0, -0.1, 0); 
        glVertex3f(-0.425, -0.5, 0); 
        glVertex3f(0, 0.1, 0);  

        glVertex3f(0, -0.1, 0); 
        glVertex3f(0.45, -0.8, 0); 
        glVertex3f(0, 0.1, 0); 

        glVertex3f(0.425, -0.5, 0);
        glVertex3f(0.45, -0.8, 0); 
        glVertex3f(0, 0.1, 0);  

        glVertex3f(0.425, -0.5, 0); 
        glVertex3f(0.45, -0.8, 0);  
        glVertex3f(0.9, 0.8, 0); 

        glVertex3f(0.425, -0.5, 0); 
        glVertex3f(0.75, 0.8, 0);
        glVertex3f(0.9, 0.8, 0); 
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char **argv) {  // request double-buffered color window with depth buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInit(&argc, argv);
    glutCreateWindow("Jacob Webb");
    glShadeModel(GL_SMOOTH);  // smooth color interpolation
    glEnable(GL_DEPTH_TEST);  // enable hidden surface removal
    glClearColor(238/255.0, 118/255.0, 0, 1);
    glutDisplayFunc(display); // function to render window
    glutMainLoop();
}
