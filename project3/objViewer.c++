/* recursive subdivision of a tetrahedron to form 3D Sierpinski gasket */
/* number of recursive steps given on command line */

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "loader.h"
#include <iostream>

static GLdouble eyex = -200.0;
static GLdouble eyey = 200.0;
static GLdouble eyez = 200.0;
static GLdouble radius = 1000.0;
static GLdouble targetx = 0.0;
static GLdouble targety = 0.0;
static GLdouble targetz = 0.0;

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

void drawFaces() {
  int i, j;
  Face f;
  Vertex *v;
  for(i = 0; i < t->fs.size(); ++i) {
    f = t->fs[i];
    glNormal3f((f.vs[0])->normal[0], (f.vs[0])->normal[1], (f.vs[0])->normal[2]);
    glVertex3f((f.vs[0])->x, (f.vs[0])->y, (f.vs[0])->z);
    glNormal3f((f.vs[1])->normal[0], (f.vs[1])->normal[1], (f.vs[1])->normal[2]);
    glVertex3f((f.vs[1])->x, (f.vs[1])->y, (f.vs[1])->z);
    glNormal3f((f.vs[2])->normal[0], (f.vs[2])->normal[1], (f.vs[2])->normal[2]);
    glVertex3f((f.vs[2])->x, (f.vs[2])->y, (f.vs[2])->z);
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
      drawFaces();
    glEnd();
    glFlush();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1.0, 100, 1500);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    // cout << "entered main" << endl;
    // cout.flush();
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
    glEnable(GL_NORMALIZE);
    TrimeshLoader tl = TrimeshLoader();
    tl.loadOBJ("models/mannequin.obj", t);
    t->calculateNormals();
    
    // cout << "vertices:" << endl;
    // int i = 0;
    // Vertex *v;
    // for(i; i < t->vs.size(); ++i){
    //   cout << i+1 << " ";
    //   v = t->vs[i];
    //   cout << v->x << " " << v->y << " " << v->z << "\t";
    //   cout << "normal: " << v->normal[0] << " " << v->normal[1] << " " << v->normal[2] << endl;
    // }

    // cout << endl;

    // cout << "faces:" << endl;
    // i = 0;
    // Face f;
    // for(i; i < t->fs.size(); ++i){
    //   cout << "face " << i+1 << endl;
    //   f = t->fs[i];
    //   cout << (f.vs[0])->x << " " << (f.vs[0])->y << " " << (f.vs[0])->z << endl;
    //   cout << (f.vs[1])->x << " " << (f.vs[1])->y << " " << (f.vs[1])->z << endl;
    //   cout << (f.vs[2])->x << " " << (f.vs[2])->y << " " << (f.vs[2])->z << endl;
    //   cout << "normal: " << f.normal[0] << " " << f.normal[1] << " " << f.normal[2] << endl << endl;
    // }

    glutMainLoop();
    delete[] t;
}
