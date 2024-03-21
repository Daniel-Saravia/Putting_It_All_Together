#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <iostream>

using namespace std;

// The cube has opposite corners at (0,0,0) and (1,1,1), which are black and
// white respectively.  The x-axis is the red gradient, the y-axis is the
// green gradient, and the z-axis is the blue gradient.  The cube's position
// and colors are fixed.

float cameraSpeed = 0.1f;

bool rotateCamera = true; // change

GLfloat translationX = 0.0;
GLfloat translationY = 0.0;
GLfloat zoomFactor = 1.0;
GLfloat rotationAngle = 0.0; // rotation angle

GLfloat cubePositionX = 0.0;
GLfloat cubeVelocityX = 0.1;
GLfloat cubeSize = 1.0;

bool spinning = false;
//draw the different cubes on the scene
namespace Cube {

const int NUM_VERTICES = 8;
const int NUM_FACES = 6;

GLint vertices[NUM_VERTICES][3] = {
  {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
  {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};

GLint verticesCube1[NUM_VERTICES][3] = {
  {1, 1, 1}, {1, 1, 2}, {1, 2, 1}, {1, 2, 2},
  {2, 1, 1}, {2, 1, 2}, {2, 2, 1}, {2, 2, 2}};
  
GLint verticesCube2[NUM_VERTICES][3] = {
  {-1, -1,-1}, {-1,-1, 0}, {-1, 0, -1}, {-1, 0, 0},
  {0, -1, -1}, {0, -1, 0}, {0, 0, -1}, {0, 0, 0}};
  
GLint faces[NUM_FACES][4] = {
  {1, 5, 7, 3}, {5, 4, 6, 7}, {4, 0, 2, 6},
  {3, 7, 6, 2}, {0, 1, 3, 2}, {0, 4, 5, 1}};

GLfloat vertexColors[NUM_VERTICES][3] = {
  {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
  {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};
  
GLfloat vertexColors1[NUM_VERTICES][3] = {
  {0.0, 1.0, 0.0}, {0.0, 2.0, 1.0}, {1.0, 1.0, 0.0}, {2.0, 1.0, 1.0},
  {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};

GLfloat vertexColors2[NUM_VERTICES][3] = {
  {1.0, 1.0, 0.0}, {0.0, 2.0, 1.0}, {0.0, 1.0, 0.0}, {2.0, 1.0, 1.0},
  {1.0, 1.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 0.0, 1.0}};

void draw() {

  glBegin(GL_QUADS);
  for (int i = 0; i < NUM_FACES; i++) {
    for (int j = 0; j < 4; j++) {
      glColor3fv((GLfloat*)&vertexColors[faces[i][j]]);
      glVertex3iv((GLint*)&vertices[faces[i][j]]);
    }
  }
  glEnd();
  
  glPushMatrix();
  glTranslatef(cubePositionX, 0.0, 0.0);  // Adjust the Y and Z position as needed
  
  glBegin(GL_QUADS);
  for (int i = 0; i < NUM_FACES; i++) {
    for (int j = 0; j < 4; j++) {
      glColor3fv((GLfloat*)&vertexColors1[faces[i][j]]);
      glVertex3iv((GLint*)&verticesCube1[faces[i][j]]);
    }
  }
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(cubePositionX, 0.0, 0.0);  // Adjust the Y and Z position as needed
  
  glBegin(GL_QUADS);
  for (int i = 0; i < NUM_FACES; i++) {
    for (int j = 0; j < 4; j++) {
      glColor3fv((GLfloat*)&vertexColors2[faces[i][j]]);
      glVertex3iv((GLint*)&verticesCube2[faces[i][j]]);
    }
  }
  glEnd();
  glPopMatrix();
  
}
}
namespace Plane {
void drawPlane(){

    
    glBegin(GL_QUADS);
    glColor3f(22.0 / 255.0, 61.0 / 255.0, 188.0 / 255.0);
    glVertex3f(9.0, 25.0, 3.9);//Top Left Vertex
    glVertex3f(9.0, 25.0, -4.5);//Bottom Left Vertex
    glVertex3f(12.0, -25.0, -4.5);//Bottom Right Vertex
    glVertex3f(12.0, -25.0, 3.9);//Top Right Vertex
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0 / 255.0, 61.0 / 255.0, 188.0 / 255.0);
    glVertex3f(-9.0, 25.0, -4.5);//Top Left Vertex
    glVertex3f(-9.0, 25.0, 3.9);//Bottom Left Vertex
    glVertex3f(-12.0, -25.0, 3.9);//Bottom Right Vertex
    glVertex3f(-12.0, -25.0, -4.5);//Top Right Vertex
    glEnd();
    
}
}

// Display and Animation. To draw we just clear the window and draw the cube.
// Because our main window is double buffered we have to swap the buffers to
// make the drawing visible. Animation is achieved by successively moving our
// camera and drawing. The function nextAnimationFrame() moves the camera to
// the next point and draws. The way that we get animation in OpenGL is to
// register nextFrame as the idle function; this is done in main().
void display() {
  
  glClear(GL_COLOR_BUFFER_BIT);

  glTranslatef(translationX, translationY, 0.0);
  glRotatef(rotationAngle, 0.0, 0.0, 1.0);
  glScalef(zoomFactor, zoomFactor, 1.0);

  Cube::draw();
  Plane::drawPlane();
  glFlush();
  glutSwapBuffers();
  
}

// We'll be flying around the cube by moving the camera along the orbit of the
// curve u->(8*cos(u), 7*cos(u)-1, 4*cos(u/3)+2).  We keep the camera looking
// at the center of the cube (0.5, 0.5, 0.5) and vary the up vector to achieve
// a weird tumbling effect.
void timer(int v) {
  if (rotateCamera) { // change
    static GLfloat u = 0.0;
    u += 0.01;
    glLoadIdentity();
    //gluLookAt(12*cos(u), -12*cos(u), 12*cos(u) + 2, .5, .5, .5, cos(90), cos(180), 0);
    // gluLookAt(0, -12*cos(u), 12*cos(u) + 2, .5, .5, .5, 0, cos(180), 0);
    // gluLookAt(8 * cos(u), 7 * cos(u) - 1, 4 * cos(u / 3) + 2, .5, .5, .5, cos(180), 0, 0);
    gluLookAt(0, 12 * cos(u) - 4, -12 * cos(u), .5, .5, .5, 0, 1, 0);

    cubePositionX += cubeVelocityX;

    if (cubePositionX - cubeSize / 2 < -6.0) {
        cubePositionX = -6.0 + cubeSize / 2;
        cubeVelocityX = -cubeVelocityX;
    }

    if (cubePositionX + cubeSize / 2 > 6.0) {
        cubePositionX = 6.0 - cubeSize / 2;
        cubeVelocityX = -cubeVelocityX;
    }

  }
  if (spinning) {
      rotationAngle += 0.1;
  }

  glutPostRedisplay();
  glutTimerFunc(1000/60.0, timer, v);
  
}

// When the window is reshaped we have to recompute the camera settings to
// match the new window shape.  Set the viewport to (0,0)-(w,h).  Set the
// camera to have a 60 degree vertical field of view, aspect ratio w/h, near
// clipping plane distance 0.5 and far clipping plane distance 40.
void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, GLfloat(w) / GLfloat(h), 0.5, 40.0);
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
switch (key) {
    case 'p':   // pause movement
        rotateCamera = false;
        timer(0);
        break;
    case 'c':   // continue movement
        rotateCamera = true;
        timer(0);
        break;
    case 'u':  // Move up
        translationY += 0.01;
        break;
    case 'd':  // Move down
        translationY -= 0.01;
        break;
    case '+':  // Zoom in
        zoomFactor *= 1.01;
        break;
    case '-':  // Zoom out
        zoomFactor /= 1.01;
        break;
    case 'r':   // toggle rotation
        spinning = true;
        break;
    case 't':   // stop rotation
        spinning = false;
        rotationAngle = 0;
        break;
  }
}

// Application specific initialization:  The only thing we really need to do
// is enable back face culling because the only thing in the scene is a cube
// which is a convex polyhedron.
void init() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

// The usual main for a GLUT application.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow("The RGB Color Cube");
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
}

