# Implementation Guide for Assignment 1

## Setting Up Your Environment

### Python Option
If using Python:
1. Install necessary libraries:
   ```bash
   pip install numpy matplotlib pygame
   ```
2. Create a basic structure with these files:
   - `vector_math.py` - Vector and matrix operations
   - `object_3d.py` - 3D object representation
   - `renderer.py` - Rendering pipeline
   - `ui.py` - User interface
   - `main.py` - Main application

### C++/OpenGL Option
If using C++:
1. Set up a development environment with OpenGL/GLFW
2. Structure your project with:
   - `Vector.h/cpp` - Vector classes
   - `Matrix.h/cpp` - Matrix classes
   - `Object3D.h/cpp` - 3D object representation
   - `Renderer.h/cpp` - Rendering pipeline
   - `UI.h/cpp` - User interface
   - `main.cpp` - Main application

## Task Implementations

### 1. Vector and Matrix Library

#### Vector Class (3D)
Implement a class with:
- Constructor: `Vector3(x, y, z)`
- Properties: `x`, `y`, `z`
- Methods:
  - Addition: `add(vector)`
  - Subtraction: `subtract(vector)`
  - Dot product: `dot(vector)`
  - Cross product: `cross(vector)`
  - Magnitude: `magnitude()`
  - Normalization: `normalize()`

#### Matrix Class (4x4 for homogeneous coordinates)
Implement a class with:
- Constructor: `Matrix4x4(values)` or default identity
- Methods:
  - Multiplication: `multiply(matrix)`
  - Transposition: `transpose()`
  - Inversion: `inverse()`
  - Creating transformation matrices:
    - `translation(tx, ty, tz)`
    - `rotationX(angle)`, `rotationY(angle)`, `rotationZ(angle)`
    - `scaling(sx, sy, sz)`
    - `perspective(fov, aspect, near, far)`

### 2. 3D Object Representation

Define a class for 3D objects:
```python
class Object3D:
    def __init__(self):
        self.vertices = []  # List of Vector3
        self.edges = []     # List of tuples (vertex_index1, vertex_index2)
        self.faces = []     # List of lists of vertex indices
```

Create factory methods for basic shapes:
- `create_cube(size)`
- `create_pyramid(base_size, height)`
- `create_custom_shape()` - Your custom shape

### 3. Transformation Pipeline

Implement a pipeline class:
```python
class TransformationPipeline:
    def __init__(self):
        self.model_matrix = Matrix4x4()  # Identity
        self.view_matrix = Matrix4x4()   # Identity
        self.projection_matrix = Matrix4x4()  # Identity
        
    def set_model_transform(self, translation, rotation, scale):
        # Combine transformations to create model matrix
        
    def set_view_transform(self, camera_pos, target, up):
        # Create view matrix (look-at)
        
    def set_projection(self, fov, aspect, near, far):
        # Create projection matrix
        
    def apply_pipeline(self, vertices):
        # Apply MVP transformation to all vertices
        # Convert to screen coordinates
        return transformed_vertices
```

### 4. Simple Renderer

Implement a renderer class:
```python
class Renderer:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.pipeline = TransformationPipeline()
        
    def render(self, object3d):
        # Transform vertices
        transformed_vertices = self.pipeline.apply_pipeline(object3d.vertices)
        
        # Draw edges
        for edge in object3d.edges:
            v1 = transformed_vertices[edge[0]]
            v2 = transformed_vertices[edge[1]]
            draw_line(v1, v2)  # Platform-specific drawing function
```

### 5. User Interface

Implement controls:
- Arrow keys: Rotate around X and Y axes
- WASD: Translate in X and Z axes
- Q/E: Translate in Y axis
- +/-: Scale object
- R: Reset transformations

## Testing Your Implementation

Test each component separately:
1. Test vector/matrix operations with known results
2. Verify basic shapes are constructed correctly
3. Test transformations on simple vertices
4. Ensure rendering works for simple cases
5. Combine everything and test the full pipeline

## Tips for Debugging

- Add visualization for intermediate steps (e.g., display object before projection)
- Output matrix values during transformations
- Implement simple wireframe rendering first before attempting more complex techniques
- Test with a simple cube before moving to more complex objects
- Use color to differentiate between objects or faces
- Add logging to track transformation state

## Resources

- Linear algebra refresher: [Khan Academy](https://www.khanacademy.org/math/linear-algebra)
- 3D transformation references: [Scratchapixel](https://www.scratchapixel.com/)
- Matrix operations: [Math is Fun](https://www.mathsisfun.com/algebra/matrix-introduction.html)
- OpenGL tutorials: [Learn OpenGL](https://learnopengl.com/)
- Python with Matplotlib: [Matplotlib 3D](https://matplotlib.org/stable/tutorials/toolkits/mplot3d.html)