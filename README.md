# README

## Overview

This repository contains two OpenGL projects demonstrating basic 3D graphics rendering and interaction. The projects are:

- `CheckeredTriangles.cpp`: An application that displays a rotating object textured with a checkered pattern, with user interaction to control the object's movement and camera's view.
- `ColorCubeFlyby.cpp`: A visualization of a flyby around an RGB color cube, showcasing color gradients and camera movement to create a dynamic 3D scene.

## Getting Started

### Prerequisites

- OpenGL: You must have OpenGL installed on your system.
- GLUT: The OpenGL Utility Toolkit (GLUT) is required for window management and event handling.
- C++ Compiler: A C++ compiler is needed to compile the programs.

### Installation

1. Clone the repository or download the source files.
2. Ensure you have the necessary libraries (OpenGL, GLUT) installed on your system.

### Compilation

To compile the programs, use the following commands in your terminal (make sure you are in the same directory as the source files):

For `CheckeredTriangles.cpp`:

```bash
g++ CheckeredTriangles.cpp -lGL -lGLU -lglut -o CheckeredTriangles

For `ColorCubeFlyby.cpp`:

```bash
g++ ColorCubeFlyby.cpp -lGL -lGLU -lglut -o ColorCubeFlyby

Running the Programs
After compilation, run the executable generated by the compiler:

For CheckeredTriangles.cpp:

./CheckeredTriangles


For ColorCubeFlyby.cpp:

./ColorCubeFlyby

Features
CheckeredTriangles.cpp
Displays a continuously rotating checkered triangle object.
Allows user to move the object up, down, left, and right using keyboard controls ('u', 'd', 'l', 'r').
Zoom in and out functionality with '+' and '-' keys.
Object rotation can be paused and resumed.
ColorCubeFlyby.cpp
Simulates a camera flyby around a 3D RGB color cube.
Includes additional cubes and planes to enhance the 3D effect.
Camera and object movement are controlled through keyboard inputs.
Dynamic camera positioning creates a fly-through experience.
Controls
u: Move up
d: Move down
l: Move left
r: Move right
+: Zoom in
-: Zoom out
p: Pause animation
c: Continue animation
ESC: Exit the program
System Requirements
Operating System: Any OS that supports OpenGL and GLUT (e.g., Linux, macOS, Windows via Cygwin or other compatibility layers).
Hardware: GPU with OpenGL support.
License
This project is open-source and available under the MIT License.
