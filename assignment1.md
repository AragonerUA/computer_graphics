# Computer Graphics - Assignment 1: 3D Transformations and Rendering

## Overview
This assignment will help you understand 3D transformations and basic rendering techniques. You will implement a simple 3D renderer that can display and transform objects in a 3D space.

## Learning Objectives
- Understand and implement 3D transformations (translation, rotation, scaling)
- Implement a simple rendering pipeline
- Gain experience with vector and matrix operations in computer graphics
- Visualize 3D objects in a 2D display

## Tasks

### 1. Vector and Matrix Library
Develop a simple vector and matrix library that supports the following operations:
- Vector addition, subtraction, dot product, cross product
- Matrix multiplication, transposition, and inversion
- Creating transformation matrices for translation, rotation, and scaling

### 2. 3D Object Representation
Create a representation for 3D objects using:
- Vertex data (points in 3D space)
- Edge data (connections between vertices)
- Face data (groups of edges forming a face)

Implement at least three basic shapes:
- Cube
- Pyramid
- Custom shape of your choice

### 3. Transformation Pipeline
Implement a transformation pipeline that:
- Takes a 3D object and applies combinations of translation, rotation, and scaling
- Converts 3D coordinates to 2D screen coordinates using a simple perspective projection
- Handles the model-view-projection transformation sequence

### 4. Simple Renderer
Create a simple wireframe renderer that:
- Draws the edges of your objects
- Applies transformations based on user input
- Implements a simple viewer that can display the transformed objects

### 5. User Interface
Implement basic user controls to:
- Rotate objects along X, Y, and Z axes
- Translate objects in 3D space
- Scale objects larger or smaller
- Reset transformations

## Implementation Requirements
You may use any programming language of your choice (Python with Matplotlib/PyGame, C++ with OpenGL/GLFW, JavaScript with WebGL, etc.).

Your implementation should:
1. Be well-organized and well-documented
2. Clearly separate the different components (vector/matrix math, object representation, transformations, rendering)
3. Include a README file explaining how to build and run your code
4. Not rely on existing 3D graphics libraries for the transformation logic (you must implement the matrix operations yourself)

## Bonus Challenges
For extra credit, implement one or more of the following:
- Hidden line/surface removal (e.g., using a z-buffer)
- Simple shading model (e.g., flat shading)
- Additional objects (sphere, cylinder, etc.)
- Texture mapping
- Scene graph for hierarchical transformations

## Deliverables
1. Source code implementing all required components
2. README.md file with build/run instructions
3. Brief report (1-2 pages) explaining your implementation and any design decisions
4. Screenshots demonstrating your renderer displaying transformed objects

## Grading
- Vector and Matrix Library: 20%
- 3D Object Representation: 15%
- Transformation Pipeline: 25%
- Renderer: 25%
- User Interface: 10%
- Code Organization and Documentation: 5%
- Bonus Challenges: Up to 15% extra

## Due Date
Submit your complete implementation by [Due Date].