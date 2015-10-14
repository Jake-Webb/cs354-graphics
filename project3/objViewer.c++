#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "loader.h"
#include <iostream>
#include <string>
#include <stack>

using namespace std;

static GLdouble eyex = 0;
static GLdouble eyey = 0;
static GLdouble eyez = 0;
static GLdouble radius = 0;
static GLdouble targetx = 0;
static GLdouble targety = 0;
static GLdouble targetz = 0;
static GLdouble frontDistance = 0;
static GLdouble backDistance = 0;
void takeInput();

int oldX, oldY;
int viewingNum = 0;
bool rotate, zoom, pan;
float theta = 0, phi = 0;
Trimesh *t = new Trimesh();
stack<Trimesh*> *theStack = new stack<Trimesh*>();

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
        theta += (x-oldX)*0.005f;
        phi += (y-oldY)*0.005f;
   }
   else if(zoom) {
        radius += (y-oldY)*(frontDistance*0.02);
   }
   else if(pan) {
        targetx -= (x-oldX)*0.005f;
        targety += (y-oldY)*0.005f;
   }
   oldX = x; 
   oldY = y; 
   glutPostRedisplay(); 
}

void drawFaces() {
  int i;
  Face f;
  glColor4ub(255, 255, 255, 255); 
  for(i = 0; i < t->fs.size(); ++i) {
    f = t->fs[i];
    glNormal3f((f.vs[0])->normal[0], (f.vs[0])->normal[1], (f.vs[0])->normal[2]);
    glVertex3f((f.vs[0])->x, (f.vs[0])->y, (f.vs[0])->z);
    glNormal3f((f.vs[1])->normal[0], (f.vs[1])->normal[1], (f.vs[1])->normal[2]);
    glVertex3f((f.vs[1])->x, (f.vs[1])->y, (f.vs[1])->z);
    glNormal3f((f.vs[2])->normal[0], (f.vs[2])->normal[1], (f.vs[2])->normal[2]);
    glVertex3f((f.vs[2])->x, (f.vs[2])->y, (f.vs[2])->z);
    //cout << "I just drew face number: " << i << endl;
  }
}

void drawFaceNormals() {
  int i;
  Face f;
  glColor4ub(255, 0, 0, 255); 
  glBegin(GL_LINES);
  for(i = 0; i < t->fs.size(); ++i) {
    f = t->fs[i];
    glVertex3f(f.center.x, f.center.y, f.center.z);
    glVertex3f(f.center.x + f.normal[0], f.center.y + f.normal[1], f.center.z + f.normal[2]);
  }
  glEnd();
}

void drawVertexNormals() {
  int i;
  Vertex *v;
  glColor4ub(0, 0, 255, 255); 
  glBegin(GL_LINES);
  for(i = 0; i < t->vs.size(); ++i) {
    v = t->vs[i];
      glVertex3f(v->x, v->y, v->z);
      glVertex3f(v->x + v->normal[0], v->y + v->normal[1], v->z + v->normal[2]);
  }
  glEnd();
}

void display()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat lp[4] = {frontDistance*2, frontDistance*2, frontDistance*2, 1.0};
    switch(viewingNum) {
      case 0:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
      case 1:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
      case 2:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
      case 3:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glLightfv(GL_LIGHT0, GL_POSITION, lp);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);
        break;
      case 4:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glLightfv(GL_LIGHT0, GL_POSITION, lp);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);
        drawFaceNormals();
        break;
      case 5:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glLightfv(GL_LIGHT0, GL_POSITION, lp);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_NORMALIZE);
        drawFaceNormals();
        drawVertexNormals();
        break; 
    }
    eyex = targetx + radius*cos(phi)*sin(theta);
    eyey = targety + radius*sin(phi)*sin(theta);
    eyez = targetz + radius*cos(theta);
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, targetx, targety, targetz, 0.0, 1.0, 0.0);
    glColor4ub(255, 255, 255, 255); 
    glBegin(GL_TRIANGLES);
      drawFaces();
    glEnd();
    glDisable(GL_LIGHTING);
    glFlush();
    //cout << "eyex = " << eyex << " eyey = " << eyey << " eyez = " << eyez << endl;
    //cout << "targetx = " << targetx << " targety = " << targety << " targetz = " << targetz << endl;
    //cout << "radius = " << radius << endl;
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key)
  {
  case 27:  
    exit (0);
    break;
  case ',':
    if(viewingNum == 0)
      viewingNum = 5;
    else
      --viewingNum;
    break;
  case '.':
    ++viewingNum;
    viewingNum %= 6;
    break;
  case ':':
    takeInput();
    break;
  }
  display();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1.0, 0.1*frontDistance, backDistance);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void takeInput() {
  Trimesh *tt;
  TrimeshLoader tl;
  const char *c;
  float d, dx, dy, dz;
  string input;
  cout << "Input command: ";
  cin >> input;
  switch(input[0]) {
    case 'L':
      //cout << "entered L case" << endl;
      cin >> input;
      //cout << input << endl;
      //cout.flush();
      tt = new Trimesh();
      tl = TrimeshLoader();
      c = input.c_str();
      tl.loadOBJ(c, tt);
      theStack->push(tt);
      t = tt;
      t->calculateNormals();
      dz = t->bounds[5] - t->bounds[4];
      dx = t->bounds[1] - t->bounds[0];
      dy = t->bounds[3] - t->bounds[2];
      d = max(max(dz, dx), dy);
      targetx = (t->bounds[1] + t->bounds[0]) / 2;
      targety = (t->bounds[3] + t->bounds[2]) / 2;
      targetz = (t->bounds[4] + t->bounds[5]) / 2;
      backDistance = 5*d;
      frontDistance = 0.5*d;
      radius = 3*d;
      //cout << "exiting L" << endl;
      break;
    case 'D':
      theStack->pop();
      t = theStack->top();
      dz = t->bounds[5] - t->bounds[4];
      dx = t->bounds[1] - t->bounds[0];
      dy = t->bounds[3] - t->bounds[2];
      d = max(max(dz, dx), dy);
      targetx = (t->bounds[1] + t->bounds[0]) / 2;
      targety = (t->bounds[3] + t->bounds[2]) / 2;
      targetz = (t->bounds[4] + t->bounds[5]) / 2;
      backDistance = 5*d;
      frontDistance = 0.5*d;
      radius = 3*d;
      break;
    case 'I':
      break;
    case 'T':
      break;
    case 'S':
      break;
    case 'R':
      break;
    case 'V':
      break;
    case 'W':
      break;
  }
  myReshape(500, 500);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow(".obj File Loader/viewer");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutMouseFunc(OnMouseDown);
    glutMotionFunc(OnMouseMove);
    glutKeyboardFunc(Keyboard);
    //glutIdleFunc(idle);
    glEnable(GL_DEPTH_TEST);
    //theStack.push(t);
    //TrimeshLoader tl = TrimeshLoader();
    //tl.loadOBJ("models/sphere.obj", t);
    // t->calculateNormals();
    // float dz = t->bounds[5] - t->bounds[4];
    // float dx = t->bounds[1] - t->bounds[0];
    // float dy = t->bounds[3] - t->bounds[2];
    // float d = max(max(dz, dx), dy);
    // targetx = (t->bounds[1] + t->bounds[0]) / 2;
    // targety = (t->bounds[3] + t->bounds[2]) / 2;
    // targetz = (t->bounds[4] + t->bounds[5]) / 2;
    // backDistance = 5*d;
    // frontDistance = 0.5*d;
    // radius = 3*d;
    glutMainLoop();
}
