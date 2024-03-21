#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

bool isAnimating = true;
bool isRotating = true;
GLfloat verticalPosition = 0.0f;
GLfloat u = 0.0;
GLfloat zoomLevel = 60.0f;
GLfloat horizontalPosition = 0.0f;  // Global variable to track horizontal movement

#define red          \
  {                  \
    0xff, 0x00, 0x00 \
  }
#define yellow       \
  {                  \
    0xff, 0xff, 0x00 \
  }
#define magenta   \
  {               \
    0xff, 0, 0xff \
  }
GLubyte texture[][3] = {
    red,
    yellow,
    yellow,
    red,
};

float angle = 0.0;

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(zoomLevel, GLfloat(width) / height, 1, 40);  // Use zoomLevel for FOV
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(2, -1, 5, 0, 0, 0, 0, 1, 0);
  glEnable(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               3,
               2, 2,
               0,
               GL_RGB,
               GL_UNSIGNED_BYTE,
               texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(horizontalPosition, verticalPosition, -5.0f); // Use horizontalPosition for left/right movement
    glRotatef(angle, 0.0, 0.0, 1.0);

    glBegin(GL_TRIANGLES);
        // Define the vertices and texture coordinates for the triangles
        glTexCoord2f(0.5, 1.0); glVertex2f(-3, 3);
        glTexCoord2f(0.0, 0.0); glVertex2f(-3, 0);
        glTexCoord2f(1.0, 0.0); glVertex2f(0, 0);

        glTexCoord2f(4, 8); glVertex2f(3, 3);
        glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
        glTexCoord2f(8, 0.0); glVertex2f(3, 0);

        glTexCoord2f(5, 5); glVertex2f(0, 0);
        glTexCoord2f(0.0, 0.0); glVertex2f(-1.5, -3);
        glTexCoord2f(4, 0.0); glVertex2f(1.5, -3);
    glEnd();

    glutSwapBuffers();
}



void handleKeyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27: // ESC key
    exit(0);
    break;
  case 'p':
    isAnimating = false;
    break;
  case 'c':
    isAnimating = true;
    break;
  case 'u': // Move up
    verticalPosition += 0.1f;
    break;
  case 'd': // Move down
    verticalPosition -= 0.1f;
    break;
  case 'l': // Move left
    horizontalPosition -= 0.1f;
    break;
  case 'r': // Move right
    horizontalPosition += 0.1f;
    break;
  case '+': // Zoom in
  case '=': // Often the same key as '+' without shift
    zoomLevel = std::max(10.0f, zoomLevel - 5.0f);
    reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    break;
  case '-': // Zoom out
  case '_': // Often the same key as '-' without shift
    zoomLevel = std::min(120.0f, zoomLevel + 5.0f);
    reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    break;
  }
  glutPostRedisplay();
}




void update(int value)
{
  if (isAnimating)
  {
    angle += -3.0;
    if (angle > 360)
    {
      angle -= 360;
    }
  }
  glutPostRedisplay();
  glutTimerFunc(16, update, 0);
}

void init()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(520, 390);
  glutCreateWindow("Textured Triangles");

  if (!glutGetWindow())
  {
    std::cerr << "Failed to create window" << std::endl;
    return -1;
  }

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(handleKeyboard); // Register keyboard callback

  init();
  glutTimerFunc(0, update, 0);

  glutMainLoop();
  return 0;
}