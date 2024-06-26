// This program is a flyby around the RGB color cube. An interesting note
// is that because the cube is a convex polyhedron and it is the only thing
// in the scene, we can render it using backface culling only, meaning
// there is no need for a depth buffer. The scene also includes thin rectangle
// planes to enhance the visual effect.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include <algorithm>

bool isAnimating = true;         // Initially set to true to start the animation
bool isRotating = false;         // Controls the rotation of the cube
GLfloat verticalPosition = 0.0f; // Initial vertical position of the cube
GLfloat u = 0.0;                 // Global variable to control camera position
GLfloat zoomLevel = 60.0f;       // Initial FOV for the perspective camera

// The cube has opposite corners at (0,0,0) and (1,1,1), which are black and
// white respectively. The x-axis is the red gradient, the y-axis is the
// green gradient, and the z-axis is the blue gradient. The cube's position
// and colors are fixed.
namespace Cube
{
  bool isAnimating = true; // Used to control the animation state
  const int NUM_VERTICES = 8;
  const int NUM_FACES = 6;

  GLint vertices[NUM_VERTICES][3] = {
      {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};

  GLint faces[NUM_FACES][4] = {
      {1, 5, 7, 3}, {5, 4, 6, 7}, {4, 0, 2, 6}, {3, 7, 6, 2}, {0, 1, 3, 2}, {0, 4, 5, 1}};

  GLfloat vertexColors[NUM_VERTICES][3] = {
      {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};

  // More Cubes

  const int NUM_CUBES = 3; // Total cubes including the original
  GLfloat cubePositions[NUM_CUBES][3] = {
      {0.5, 0.5, 0.5},  // Original cube
      {-3.0, 0.5, 0.5}, // Additional cube 1
      {3.0, 0.5, 0.5}   // Additional cube 2
  };
  GLfloat cubeMovement[NUM_CUBES][3] = {
      {0.0, 0.0, 0.0}, // Original cube (stationary)
      {0.0, 0.0, 0.1}, // Additional cube 1 movement direction
      {0.0, 0.0, -0.1} // Additional cube 2 movement direction
  };

  // Plane definitions
  const int NUM_PLANES = 2;
  GLint planeVertices[NUM_PLANES][4][3] = {
      // Plane 1: Positioned in front of the cube, parallel to the XY plane
      {{-5, -5, -5}, {5, -5, -5}, {5, 5, -5}, {-5, 5, -5}},
      // Plane 2: Positioned behind the cube, parallel to the XY plane
      {{-5, -5, 5}, {5, -5, 5}, {5, 5, 5}, {-5, 5, 5}}};
  GLfloat planeColor[3] = {0.5, 0.5, 0.5}; // Gray color for the planes

  void drawPlanes()
  {
    glColor3fv(planeColor);
    for (int i = 0; i < NUM_PLANES; i++)
    {
      glBegin(GL_QUADS);
      for (int j = 0; j < 4; j++)
      {
        glVertex3iv(planeVertices[i][j]);
      }
      glEnd();
    }
  }

  void draw()
  {
    for (int c = 0; c < NUM_CUBES; c++)
    {
      // Draw each cube
      glPushMatrix();
      glTranslatef(cubePositions[c][0], cubePositions[c][1], cubePositions[c][2]);
      glBegin(GL_QUADS);
      for (int i = 0; i < NUM_FACES; i++)
      {
        for (int j = 0; j < 4; j++)
        {
          glColor3fv((GLfloat *)&vertexColors[faces[i][j]]);
          glVertex3iv((GLint *)&vertices[faces[i][j]]);
        }
      }
      glEnd();
      glPopMatrix();
    }

    // Draw planes
    drawPlanes();
  }

  void updateCubes() {
    for (int i = 1; i < NUM_CUBES; i++) { // Start from 1 to skip the original cube
        // Update cube position along the z-axis
        cubePositions[i][2] += cubeMovement[i][2];

        // Check collision with the planes along the z-axis and reverse direction if necessary
        if (cubePositions[i][2] <= -3.5 || cubePositions[i][2] >= 3.5) {
            cubeMovement[i][2] = -cubeMovement[i][2];
        }
    }
}


}

void updateCamera()
{
  glLoadIdentity();
  // Update the camera position based on the current value of 'u'
  gluLookAt(30 * cos(u), 8.5 * cos(u) - 1, 5 * cos(u / 3) + 2,
            0.5, 0.5, 0.5, // Look at the center of the cube
            0, 0.1, 0);    // Up vector
}

void timer(int v)
{
  if (isAnimating)
  {
    u += 0.01;

    updateCamera(); // Update camera position

    Cube::updateCubes(); // Update the positions of the cubes

    glutPostRedisplay(); // Redraw the scene with the updated positions
  }
  glutTimerFunc(1000 / 60, timer, 0); // Continue the timer
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(zoomLevel, GLfloat(glutGet(GLUT_WINDOW_WIDTH)) / GLfloat(glutGet(GLUT_WINDOW_HEIGHT)), 0.5, 40.0);

  glMatrixMode(GL_MODELVIEW);
  updateCamera(); // Set up the camera

  Cube::draw(); // Draw the cube and planes

  glFlush();
  glutSwapBuffers();
}

// When the window is reshaped, we have to recompute the camera settings to
// match the new window shape. We set the viewport to (0,0)-(w,h) and adjust
// the camera to maintain the specified field of view and aspect ratio.
void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(zoomLevel, GLfloat(w) / GLfloat(h), 0.5, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

// Application-specific initialization: Enable back face culling and depth testing
// to optimize rendering as the scene contains a convex polyhedron (the cube).
void init()
{
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
}

// This function toggles the animation state based on the key pressed,
// allowing control over the animation, cube rotation, and camera zoom.
void handleKeyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 's': // Stop the animation
    isAnimating = false;
    break;
  case 'c': // Continue the animation
    isAnimating = true;
    break;
  case 'r': // Toggle rotation
    isRotating = !isRotating;
    break;
  case 'u': // Move up
    verticalPosition += 0.1f;
    break;
  case 'd': // Move down
    verticalPosition -= 0.1f;
    break;
  case '+': // Zoom in
    zoomLevel = std::max(10.0f, zoomLevel - 5.0f);
    break;
  case '-': // Zoom out
    zoomLevel = std::min(120.0f, zoomLevel + 5.0f);
    break;
  }
  glutPostRedisplay();
}

// Entry point of the program: Initializes the rendering window, sets up callback functions,
// and enters the main loop where events are processed.
int main(int argc, char **argv)
{
  // Initialize GLUT with command-line parameters
  glutInit(&argc, argv);

  // Set the display mode to double buffering and RGB color
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Set the initial size of the window
  glutInitWindowSize(1500, 1500);

  // Create the window with a title
  glutCreateWindow("The RGB Color Cube");

  // Register the reshape callback function, which is called
  // whenever the window is resized
  glutReshapeFunc(reshape);

  // Register the display callback function, which is called
  // when the window needs to be redrawn
  glutDisplayFunc(display);

  // Register the keyboard callback function, which is called
  // when a key is pressed
  glutKeyboardFunc(handleKeyboard);

  // Application-specific initialization
  init();

  // Set the timer function to be called after 100 milliseconds
  glutTimerFunc(100, timer, 0);

  // Enter the GLUT main loop, processing events and triggering callbacks
  glutMainLoop();
}
