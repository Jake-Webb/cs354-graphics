/* recursive subdivision of a tetrahedron to form 3D Sierpinski gasket */
/* number of recursive steps given on command line */

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "loader.h"
#include <iostream>

int n;
int recursionMenuNum;
int viewMenuNum;
static GLdouble eyex = -3.0;
static GLdouble eyey = 3.0;
static GLdouble eyez = 3.0;
static GLdouble radius = 5.0;
static GLdouble targetx = 0.0;
static GLdouble targety = 0.0;
static GLdouble targetz = 0.0;

/* initial cube */
GLfloat c[8][3] = {{0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}};

GLfloat color[6][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.62745, 0.0}, {0.7451, 0.0, 1.0}, {1.0, 1.0, 0.0}};

int oldX, oldY;
bool rotate, zoom, pan;
float theta = 0, phi = 0;
Trimesh *t = new Trimesh();

void OnMouseDown(int button, int state, int x, int y) {
   rotate = false;
   zoom = false;
   pan = false;
   if(button == GLUT_LEFT_BUTTON) {
        oldX = x;
        oldY = y;
        rotate = true;
   }
   else if(button == GLUT_RIGHT_BUTTON) {
        oldX = x;
        oldY = y;
        zoom = true;
   }
    else if(button == GLUT_MIDDLE_BUTTON) {
        oldX = x;
        oldY = y;
        pan = true;
   }
}

void OnMouseMove(int x, int y) {
   if(rotate) {
        theta += (x-oldX)*0.015f;
        phi += (y-oldY)*0.015f;
   }
   else if(zoom) {
        radius += (y-oldY)*0.015f;
   }
   else if(pan) {
        targetx -= (x-oldX)*0.015f;
        targety += (y-oldY)*0.015f;
   }
   oldX = x; 
   oldY = y; 
   glutPostRedisplay(); 
}

void placeVertices()
{
  int i = 0;
  Vertex v;
  for(i; i < t->vs.size(); ++i){
    v = t->vs[i];
    glNormal3f(v.normal[0], v.normal[1], v.normal[2]);
    glVertex3f(v.x, v.y, v.z);
  }
}

void display()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    eyex = targetx + radius*cos(phi)*sin(theta);
    eyey = targety + radius*sin(phi)*sin(theta);
    eyez = targetz + radius*cos(theta);
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, targetx, targety, targetz, 0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
      placeVertices();
    glEnd();
    glFlush();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1.0, 1.5, 20);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    cout << "entered main" << endl;
    cout.flush();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D Gasket");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutMouseFunc(OnMouseDown);
    glutMotionFunc(OnMouseMove);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    TrimeshLoader tl = TrimeshLoader();
    tl.loadOBJ("models/triangle.obj", t);
    
  //   cout << "vertices:" << endl;
  //   int i = 0;
  //   Vertex v;
  //   for(i; i < t->vs.size(); ++i){
  //     cout << i+1 << " ";
  //     v = t->vs[i];
  //     cout << v.x << " " << v.y << " " << v.z << endl;
  // }

    glutMainLoop();
    delete[] t;
}
