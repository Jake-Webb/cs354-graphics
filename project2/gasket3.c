/* recursive subdivision of a tetrahedron to form 3D Sierpinski gasket */
/* number of recursive steps given on command line */

#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>

static int window;
int n;
int recursionMenuNum;
int viewMenuNum;
static GLdouble eyex = -3.0;
static GLdouble eyey = 3.0;
static GLdouble eyez = 3.0;

/* initial tetrahedron */

GLfloat v[4][3]={{0.0, 0.0, 1.0}, {0.0, 0.942809, -0.33333},
      {-0.816497, -0.471405, -0.333333}, {0.816497, -0.471405, -0.333333}};
GLfloat c[8][3] = {{0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}};

GLfloat colors[4][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
                        {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}};
GLfloat color[6][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.62745, 0.0}, {0.7451, 0.0, 1.0}, {1.0, 1.0, 0.0}};


void triangle(GLfloat *va, GLfloat *vb, GLfloat *vc)
{
       glVertex3fv(va);
       glVertex3fv(vb);
       glVertex3fv(vc);
}

void square(GLfloat *va, GLfloat *vb, GLfloat *vc, GLfloat *vd)
{
    glVertex3fv(va);
    glVertex3fv(vb);
    glVertex3fv(vc);
    glVertex3fv(vd);
}

void tetra(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d)
{
    glColor3fv(colors[0]);
    triangle(a, b, c);
    glColor3fv(colors[1]);
    triangle(a, c, d);
    glColor3fv(colors[2]);
    triangle(a, d, b);
    glColor3fv(colors[3]);
    triangle(b, d, c);
}

void cube(GLfloat *a, GLfloat *g) {
    GLfloat b[3] = {g[0], a[1], a[2]};
    GLfloat c[3] = {g[0], g[1], a[2]};
    GLfloat d[3] = {a[0], g[1], a[2]};
    GLfloat e[3] = {a[0], a[1], g[2]};
    GLfloat f[3] = {g[0], a[1], g[2]};
    GLfloat h[3] = {a[0], g[1], g[2]};
    glColor3fv(color[0]);
    square(a, b, c, d);
    glColor3fv(color[1]);
    square(a, d, h, e);
    glColor3fv(color[2]);
    square(a, e, f, b);
    glColor3fv(color[3]);
    square(e, h, g, f);
    glColor3fv(color[4]);
    square(f, g, c, b);
    glColor3fv(color[5]);
    square(g, c, d, h);
}

void divide_tetra(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, int m)
{

    GLfloat mid[6][3];
    int j;
    if(m>0)
    {
        /* compute six midpoints */

        for(j=0; j<3; j++) mid[0][j]=(a[j]+b[j])/2;
        for(j=0; j<3; j++) mid[1][j]=(a[j]+c[j])/2;
        for(j=0; j<3; j++) mid[2][j]=(a[j]+d[j])/2;
        for(j=0; j<3; j++) mid[3][j]=(b[j]+c[j])/2;
        for(j=0; j<3; j++) mid[4][j]=(c[j]+d[j])/2;
        for(j=0; j<3; j++) mid[5][j]=(b[j]+d[j])/2;

        /* create 4 tetrahedrons by subdivision */

        divide_tetra(a, mid[0], mid[1], mid[2], m-1);
        divide_tetra(mid[0], b, mid[3], mid[5], m-1);
        divide_tetra(mid[1], mid[3], c, mid[4], m-1);
        divide_tetra(mid[2], mid[5], mid[4], d, m-1);

    }
    else(tetra(a,b,c,d)); /* draw tetrahedron at end of recursion */
}

divide_cube(GLfloat *a, GLfloat *g, int m) {
    GLfloat mid[38][3];
    GLfloat temp[3];
    int j, x, y;
    if(m > 0) {
        // Some calculation bullshit
        double s = (a[0] - g[0])/3.0;
        if(s < 0)
            s *= -1;
        for(j = 0; j < 3; ++j) 
            mid[0][j] = (a[j]-s);
        mid[1][0] = a[0]-s;
        mid[1][1] = a[1];
        mid[1][2] = a[2];
        for(j = 0; j < 3; ++j) 
            mid[2][j] = (mid[1][j]-s);
        mid[3][0] = a[0]-(2*s);
        mid[3][1] = a[1];
        mid[3][2] = a[2];
        for(j = 0; j < 3; ++j) 
            mid[4][j] = (mid[3][j]-s);
        mid[5][0] = a[0];
        mid[5][1] = a[1]-s;
        mid[5][2] = a[2];
        for(j = 0; j < 3; ++j) 
            mid[6][j] = (mid[5][j]-s);
        mid[7][0] = a[0];
        mid[7][1] = a[1]-(2*s);
        mid[7][2] = a[2];
        for(j = 0; j < 3; ++j) 
            mid[8][j] = (mid[7][j]-s);
        mid[9][0] = mid[1][0];
        mid[9][1] = mid[7][1]; 
        mid[9][2] = a[2];
        for(j = 0; j < 3; ++j) 
            mid[10][j] = (mid[9][j]-s);
        mid[11][0] = mid[3][0];
        mid[11][1] = mid[7][1];
        mid[11][2] = a[2];
        for(j = 0; j < 3; ++j) 
            mid[12][j] = (mid[11][j]-s);
        mid[13][0] = mid[3][0];
        mid[13][1] = mid[5][1];
        mid[13][2] = a[2];
        for(j = 0; j < 3; ++j) 
            mid[14][j] = (mid[13][j]-s);
        mid[15][0] = a[0];
        mid[15][1] = a[1];
        mid[15][2] = a[2]-s;
        for(j = 0; j < 3; ++j) 
            mid[16][j] = (mid[15][j]-s);
        mid[17][0] = mid[3][0];
        mid[17][1] = a[1];
        mid[17][2] = mid[15][2];
        for(j = 0; j < 3; ++j) 
            mid[18][j] = (mid[17][j]-s);
        mid[19][0] = a[0];
        mid[19][1] = mid[7][1];
        mid[19][2] = mid[15][2];
        for(j = 0; j < 3; ++j) 
            mid[20][j] = (mid[19][j]-s);
        mid[21][0] = mid[11][0];
        mid[21][1] = mid[11][1];
        mid[21][2] = mid[15][2];
        for(j = 0; j < 3; ++j) 
            mid[22][j] = (mid[21][j]-s);
        mid[23][0] = a[0];
        mid[23][1] = a[1];
        mid[23][2] = a[2]-(2*s);
        for(j = 0; j < 3; ++j) 
            mid[24][j] = (mid[23][j]-s);
        mid[25][0] = mid[1][0];
        mid[25][1] = a[1];
        mid[25][2] = mid[23][2];
        for(j = 0; j < 3; ++j) 
            mid[26][j] = (mid[25][j]-s);
        mid[27][0] = mid[3][0];
        mid[27][1] = a[1];
        mid[27][2] = mid[23][2];
        for(j = 0; j < 3; ++j) 
            mid[28][j] = (mid[27][j]-s);
        mid[29][0] = a[0];
        mid[29][1] = mid[5][1];
        mid[29][2] = mid[23][2];
        for(j = 0; j < 3; ++j) 
            mid[30][j] = (mid[29][j]-s);
        mid[31][0] = mid[13][0];
        mid[31][1] = mid[13][1];
        mid[31][2] = mid[23][2];
        for(j = 0; j < 3; ++j) 
            mid[32][j] = (mid[31][j]-s);
        mid[33][0] = mid[7][0];
        mid[33][1] = mid[7][1];
        mid[33][2] = mid[23][2];
        for(j = 0; j < 3; ++j) 
            mid[34][j] = (mid[33][j]-s);   
        mid[35][0] = mid[9][0];
        mid[35][1] = mid[9][1];
        mid[35][2] = mid[23][2];
        for(j = 0; j < 3; ++j) 
            mid[36][j] = (mid[35][j]-s);
        for(j = 0; j < 3; ++j) 
            mid[37][j] = (g[j]+s);

        divide_cube(a, mid[0], m-1);
        divide_cube(mid[1], mid[2], m-1);
        divide_cube(mid[3], mid[4], m-1);
        divide_cube(mid[5], mid[6], m-1);
        divide_cube(mid[7], mid[8], m-1);
        divide_cube(mid[9], mid[10], m-1);
        divide_cube(mid[11], mid[12], m-1);
        divide_cube(mid[13], mid[14], m-1);
        divide_cube(mid[15], mid[16], m-1);
        divide_cube(mid[17], mid[18], m-1);
        divide_cube(mid[19], mid[20], m-1);
        divide_cube(mid[21], mid[22], m-1);
        divide_cube(mid[23], mid[24], m-1);
        divide_cube(mid[25], mid[26], m-1);
        divide_cube(mid[27], mid[28], m-1);
        divide_cube(mid[29], mid[30], m-1);
        divide_cube(mid[31], mid[32], m-1);
        divide_cube(mid[33], mid[34], m-1);
        divide_cube(mid[35], mid[36], m-1);
        divide_cube(mid[37], g, m-1);
    }
    else
        cube(a, g);
}

void display()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eyex, eyey, eyez, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    divide_cube(c[0], c[6], n);
    //divide_tetra(v[0], v[1], v[2], v[3], n);
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

void recursionMenu(int num) {
    n = num;
    glutPostRedisplay();
    return;
}

void viewMenu(int choice) {
    glLoadIdentity();
    if(choice == 1) {
        eyex = 3.0;
        eyey = 3.0;
        eyez = 3.0;
    }
    else {
        eyex = -3.0;
        eyey = -3.0;
        eyez = -3.0;
    }
    glutPostRedisplay();
    return;
}

void BuildPopupMenu(void) {
    recursionMenuNum = glutCreateMenu(recursionMenu);
    glutAddMenuEntry("0", 0);
    glutAddMenuEntry("1", 1);
    glutAddMenuEntry("2", 2);
    glutAddMenuEntry("3", 3);
    glutAddMenuEntry("4", 4);
    viewMenuNum = glutCreateMenu(viewMenu);
    glutAddMenuEntry("Front Right", 1);
    glutAddMenuEntry("Back Left", 0);
    glutCreateMenu(recursionMenu);
    glutAddSubMenu("Levels of Recursion", recursionMenuNum);
    glutAddSubMenu("Viewing Angle", viewMenuNum);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    window = glutCreateWindow("3D Gasket");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glEnable(GL_DEPTH_TEST);
    BuildPopupMenu();
    glutMainLoop();
}
