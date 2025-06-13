# 3D Transformation and Rendering

This project implements a simple 3D renderer with support for transformations, lighting, and textures. It allows visualization of basic 3D objects, demonstrating fundamental concepts in computer graphics.

## Features

- **Multiple 3D Objects**: Cube, Pyramid, Tetrahedron, and Sphere
- **3D Transformations**: Translation, Rotation, and Scaling
- **Rendering Options**: Wireframe and Filled Polygon modes
- **Lighting**: Basic lighting model with ambient, diffuse, and specular components
- **Textures**: Procedurally generated patterns applied to 3D objects
- **Interactive Controls**: Keyboard-based user interface for manipulating objects and viewing options
- **On-screen Instructions**: Helpful display of controls and current object state

## Requirements

- macOS (the application uses macOS-specific OpenGL headers)
- C++ compiler with C++11 support
- OpenGL framework (included with macOS)
- GLUT framework (included with macOS)

## Building the Application

To compile the application, open Terminal and navigate to the project directory:

```bash
cd /path/to/computer_graphics
```

Then compile using the following command:

```bash
g++ -std=c++11 main.cpp -o 3d_renderer -framework OpenGL -framework GLUT
```

## Running the Application

After successful compilation, run the application with:

```bash
./3d_renderer
```

If you encounter library loading issues related to conda or other environments, you can try running with:

```bash
env -u DYLD_LIBRARY_PATH ./3d_renderer
```

## Controls

### Object Manipulation
- **WASD**: Move object in the X/Z plane
- **Q/E**: Move object up/down (Y-axis)
- **Arrow Keys**: Rotate object around X/Y axes
- **Z/X**: Rotate object around Z axis
- **+/-**: Scale object up/down
- **R**: Reset transformations

### Rendering Options
- **F**: Toggle wireframe mode
- **T**: Toggle depth test
- **L**: Toggle lighting
- **G**: Toggle textures

### User Interface
- **TAB**: Switch between objects (Cube, Pyramid, Tetrahedron, Sphere)
- **H**: Toggle on-screen instructions
- **ESC**: Exit application

## Project Structure

- **Vector3.h**: 3D vector class with mathematical operations
- **Matrix4x4.h**: 4x4 matrix class for transformations
- **Object3D.h**: 3D object representation including vertices, edges, and faces
- **TransformationPipeline.h**: Model-View-Projection transformation system
- **TextureLoader.h**: Procedural texture generation
- **main.cpp**: Application entry point and rendering loop

## Implementation Details

The implementation follows the standard computer graphics pipeline:

1. **Model Transformation**: Objects are translated, rotated, and scaled in object space
2. **View Transformation**: Camera position and orientation are defined
3. **Projection Transformation**: Perspective projection is applied
4. **Rasterization**: Lines or filled polygons are drawn based on the rendering mode
5. **Lighting**: Phong lighting model is applied if enabled
6. **Texturing**: Procedural textures are applied if enabled

## Extensions and Improvements

Potential improvements to the project include:

- Support for loading 3D models from files (e.g., OBJ, STL)
- More advanced lighting models and shadows
- Image-based textures loaded from files
- Animation capabilities
- Camera controls for navigation in the 3D space

## Troubleshooting

If you encounter issues running the application:

1. **Compilation Errors**: Ensure you have the correct paths to OpenGL and GLUT headers
2. **Runtime Errors**: Check if OpenGL and GLUT are properly installed on your system
3. **Display Issues**: Make sure your graphics drivers are up to date
4. **Library Conflicts**: Try running with `env -u DYLD_LIBRARY_PATH ./3d_renderer` to avoid conflicts with other libraries

## License

This project is provided for educational purposes. Feel free to modify and extend it for your own learning.