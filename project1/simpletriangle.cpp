#include <GL/glut.h>  // includes necessary OpenGL headers

void display() {
    glShadeModel(GL_SMOOTH);  // smooth color interpolation
    glEnable(GL_DEPTH_TEST);  // enable hidden surface removal
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor4ub(238, 118, 0, 255);    // RGBA=(1,0,0,100%)
    glBegin(GL_TRIANGLES);  // every 3 vertexes makes a triangle
        glVertex3f(-0.9,  0.8,  0);  // XYZ=(-8/10,8/10,3/10)
        glVertex3f(-0.75,  0.8, 0);  // XYZ=(8/10,8/10,-2/10)
        glVertex3f(-0.45, -0.8, 0);  // XYZ=(0,-8/10,-2/10)

        glVertex3f(-0.75,  0.8,  0);  // XYZ=(-8/10,8/10,3/10)
        glVertex3f(-0.425, -0.5, 0);  // XYZ=(8/10,8/10,-2/10)
        glVertex3f(-0.45, -0.8, 0);  // XYZ=(0,-8/10,-2/10)

        glVertex3f(0, -0.1,  0);  // XYZ=(-8/10,8/10,3/10)
        glVertex3f(-0.425, -0.5, 0);  // XYZ=(8/10,8/10,-2/10)
        glVertex3f(-0.45, -0.8, 0);  // XYZ=(0,-8/10,-2/10)

        glVertex3f(0, -0.1,  0);  // XYZ=(-8/10,8/10,3/10)
        glVertex3f(-0.425, -0.5, 0);  // XYZ=(8/10,8/10,-2/10)
        glVertex3f(0, 0.1, 0);  // XYZ=(0,-8/10,-2/10)

        glVertex3f(0, -0.1,  0);  // XYZ=(-8/10,8/10,3/10)
        glVertex3f(0.45, -0.8, 0);  // XYZ=(8/10,8/10,-2/10)
        glVertex3f(0, 0.1, 0);  // XYZ=(0,-8/10,-2/10)

        glVertex3f(0.425, -0.5,  0);  // XYZ=(-8/10,8/10,3/10)
        glVertex3f(0.45, -0.8, 0);  // XYZ=(8/10,8/10,-2/10)
        glVertex3f(0, 0.1, 0);  // XYZ=(0,-8/10,-2/10)

        glVertex3f(0.425, -0.5,  0);  // XYZ=(-8/10,8/10,3/10)
        glVertex3f(0.45, -0.8, 0);  // XYZ=(8/10,8/10,-2/10)
        glVertex3f(0.9, 0.8, 0);  // XYZ=(0,-8/10,-2/10)

        glVertex3f(0.425, -0.5,  0);  // XYZ=(-8/10,8/10,3/10)
        glVertex3f(0.75, 0.8, 0);  // XYZ=(8/10,8/10,-2/10)
        glVertex3f(0.9, 0.8, 0);  // XYZ=(0,-8/10,-2/10)
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char **argv) {  // request double-buffered color window with depth buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInit(&argc, argv);
    glutCreateWindow("Jacob Webb");
    glutDisplayFunc(display); // function to render window
    glutMainLoop();
}
