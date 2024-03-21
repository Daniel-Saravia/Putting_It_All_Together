#include <GL/glut.h>    // Include the GLUT library for basic OpenGL functionality
#include <cstdlib>      // Standard library for general purpose functions
#include <cmath>        // Math library for mathematical operations
#include <algorithm>    // Standard library for algorithms like max/min
#include <GL/gl.h>      // OpenGL library for rendering 3D graphics
#include <GL/glu.h>     // OpenGL utility library for higher-level drawing routines
#include <GLFW/glfw3.h> // GLFW library for managing windows and user inputs
#include <iostream>     // Standard I/O library

// Global variables to control animation and positioning
bool isAnimating = true;           // Flag to control animation state
bool isRotating = true;            // Flag to control rotation state
GLfloat verticalPosition = 0.0f;   // Vertical position of the object
GLfloat u = 0.0;                   // Unused variable, possibly intended for future use
GLfloat zoomLevel = 60.0f;         // Zoom level for perspective view
GLfloat horizontalPosition = 0.0f; // Horizontal position of the object

// Color definitions using RGB values
#define red          \
  {                  \
    0xff, 0x00, 0x00 \
  } // Red color
#define yellow       \
  {                  \
    0xff, 0xff, 0x00 \
  } // Yellow color
#define magenta   \
  {               \
    0xff, 0, 0xff \
  } // Magenta color

// Texture data for the object, using predefined colors
GLubyte texture[][3] = {
    red,    // First pixel is red
    yellow, // Second pixel is yellow
    yellow, // Third pixel is yellow
    red,    // Fourth pixel is red
};

float angle = 0.0; // Angle for rotating the object

void reshape(int width, int height)
{
  // Set the viewport to cover the new window size
  glViewport(0, 0, width, height);

  // Set the projection matrix mode to manipulate how scene is projected to window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // Reset the projection matrix

  // Set the perspective of the scene with zoomLevel affecting the field of view
  gluPerspective(zoomLevel, GLfloat(width) / height, 1, 40);

  // Switch to model-view matrix to start drawing objects
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); // Reset the model-view matrix

  // Define position and direction of the camera
  gluLookAt(2, -1, 5, 0, 0, 0, 0, 1, 0);

  // Enable 2D texturing
  glEnable(GL_TEXTURE_2D);

  // Set texture memory alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Define the texture to be used
  glTexImage2D(GL_TEXTURE_2D,
               0,
               3,
               2, 2,
               0,
               GL_RGB,
               GL_UNSIGNED_BYTE,
               texture);

  // Set texture parameters for minification and magnification
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void display()
{
  // Clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset the model-view matrix
  glLoadIdentity();

  // Move the object based on the horizontal and vertical position
  glTranslatef(horizontalPosition, verticalPosition, -5.0f);

  // Rotate the object based on the angle
  glRotatef(angle, 0.0, 0.0, 1.0);

  // Begin drawing triangles to form the object
  glBegin(GL_TRIANGLES);
  // Define the vertices and texture coordinates for the triangles
  glTexCoord2f(0.5, 1.0);
  glVertex2f(-3, 3);
  glTexCoord2f(0.0, 0.0);
  glVertex2f(-3, 0);
  glTexCoord2f(1.0, 0.0);
  glVertex2f(0, 0);

  glTexCoord2f(4, 8);
  glVertex2f(3, 3);
  glTexCoord2f(0.0, 0.0);
  glVertex2f(0, 0);
  glTexCoord2f(8, 0.0);
  glVertex2f(3, 0);

  glTexCoord2f(5, 5);
  glVertex2f(0, 0);
  glTexCoord2f(0.0, 0.0);
  glVertex2f(-1.5, -3);
  glTexCoord2f(4, 0.0);
  glVertex2f(1.5, -3);
  glEnd(); // End drawing triangles

  // Swap the buffers to display the current drawing frame
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

// The update function, called periodically based on timer
void update(int value)
{
  // Only animate if isAnimating flag is true
  if (isAnimating)
  {
    angle += -3.0;   // Increment the angle for rotation
    if (angle > 360) // Keep the angle within 0-360 degrees
    {
      angle -= 360;
    }
  }
  glutPostRedisplay();          // Mark the current window as needing to be redisplayed
  glutTimerFunc(16, update, 0); // Set the timer to call update again in 16 milliseconds
}

// Initialize OpenGL settings
void init()
{
  glEnable(GL_CULL_FACE);  // Enable face culling
  glCullFace(GL_BACK);     // Specify which faces to cull (back faces)
  glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
}

// Entry point of the program
int main(int argc, char **argv)
{
  glutInit(&argc, argv);                                    // Initialize the GLUT library
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Set the display mode
  glutInitWindowSize(520, 390);                             // Set the window size
  glutCreateWindow("Textured Triangles");                   // Create the window with a title

  // Check if the window was successfully created
  if (!glutGetWindow())
  {
    std::cerr << "Failed to create window" << std::endl;
    return -1; // Return error code if window creation failed
  }

  // Register callback functions
  glutDisplayFunc(display);         // Rendering
  glutReshapeFunc(reshape);         // Window resizing
  glutKeyboardFunc(handleKeyboard); // Keyboard input

  init();                      // Initialize OpenGL settings
  glutTimerFunc(0, update, 0); // Start the timer for the first call to update

  glutMainLoop(); // Enter the GLUT event processing loop
  return 0;       // Exit the program
}