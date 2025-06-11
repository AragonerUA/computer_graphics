#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <vector>
#include <array>
#include <cmath>
#include "Vector3.h"

class Object3D {
public:
    std::vector<Vector3> vertices;
    std::vector<std::pair<int, int>> edges;
    std::vector<std::vector<int>> faces;
    std::array<float, 3> color;  // RGB color (values 0.0-1.0)

    Object3D() : color({1.0f, 1.0f, 1.0f}) {} // Default white color

    void setColor(float r, float g, float b) {
        color = {r, g, b};
    }

    // Factory method to create a cube
    static Object3D createCube(float size = 1.0f) {
        Object3D cube;
        float halfSize = size / 2.0f;
        
        // Define the 8 vertices of the cube
        cube.vertices = {
            {-halfSize, -halfSize, -halfSize},  // 0: back bottom left
            {halfSize, -halfSize, -halfSize},   // 1: back bottom right
            {halfSize, halfSize, -halfSize},    // 2: back top right
            {-halfSize, halfSize, -halfSize},   // 3: back top left
            {-halfSize, -halfSize, halfSize},   // 4: front bottom left
            {halfSize, -halfSize, halfSize},    // 5: front bottom right
            {halfSize, halfSize, halfSize},     // 6: front top right
            {-halfSize, halfSize, halfSize}     // 7: front top left
        };
        
        // Define the 12 edges of the cube
        cube.edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},  // back face
            {4, 5}, {5, 6}, {6, 7}, {7, 4},  // front face
            {0, 4}, {1, 5}, {2, 6}, {3, 7}   // connecting edges
        };
        
        // Define the 6 faces of the cube
        cube.faces = {
            {0, 1, 2, 3},  // back face
            {4, 5, 6, 7},  // front face
            {0, 1, 5, 4},  // bottom face
            {2, 3, 7, 6},  // top face
            {0, 3, 7, 4},  // left face
            {1, 2, 6, 5}   // right face
        };
        
        return cube;
    }
    
    // Factory method to create a pyramid
    static Object3D createPyramid(float baseSize = 1.0f, float height = 1.0f) {
        Object3D pyramid;
        float halfBase = baseSize / 2.0f;
        
        // Define the 5 vertices of the pyramid
        pyramid.vertices = {
            {-halfBase, -halfBase/2.0f, 0.0f},   // 0: front left base
            {halfBase, -halfBase/2.0f, 0.0f},    // 1: front right base
            {halfBase, halfBase/2.0f, 0.0f},     // 2: back right base
            {-halfBase, halfBase/2.0f, 0.0f},    // 3: back left base
            {0.0f, 0.0f, height}                 // 4: apex
        };
        
        // Define the 8 edges of the pyramid
        pyramid.edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},  // base
            {0, 4}, {1, 4}, {2, 4}, {3, 4}   // edges to apex
        };
        
        // Define the 5 faces of the pyramid
        pyramid.faces = {
            {0, 1, 2, 3},  // base
            {0, 1, 4},     // front face
            {1, 2, 4},     // right face
            {2, 3, 4},     // back face
            {3, 0, 4}      // left face
        };
        
        return pyramid;
    }
    
    // Factory method to create a custom tetrahedron
    static Object3D createTetrahedron(float size = 1.0f) {
        Object3D tetra;
        
        // Define the 4 vertices of the tetrahedron
        tetra.vertices = {
            {0.0f, 0.0f, 0.0f},                     // 0: base front
            {size, 0.0f, 0.0f},                     // 1: base right
            {size/2.0f, size * 0.866f, 0.0f},       // 2: base back
            {size/2.0f, size * 0.289f, size * 0.816f}  // 3: apex
        };
        
        // Define the 6 edges of the tetrahedron
        tetra.edges = {
            {0, 1}, {1, 2}, {2, 0},  // base
            {0, 3}, {1, 3}, {2, 3}   // edges to apex
        };
        
        // Define the 4 triangular faces
        tetra.faces = {
            {0, 1, 2},  // base
            {0, 1, 3},  // front face
            {1, 2, 3},  // right face
            {2, 0, 3}   // left face
        };
        
        return tetra;
    }
    
    // Factory method to create a sphere
    static Object3D createSphere(float radius = 1.0f, int resolution = 10) {
        Object3D sphere;
        
        // Generate vertices using spherical coordinates
        for (int i = 0; i <= resolution; i++) {
            float phi = M_PI * static_cast<float>(i) / resolution;  // 0 to π (latitude)
            for (int j = 0; j < resolution; j++) {
                float theta = 2.0f * M_PI * static_cast<float>(j) / resolution;  // 0 to 2π (longitude)
                
                float x = radius * sin(phi) * cos(theta);
                float y = radius * sin(phi) * sin(theta);
                float z = radius * cos(phi);
                
                sphere.vertices.push_back({x, y, z});
            }
        }
        
        // Generate edges and faces
        for (int i = 0; i < resolution; i++) {
            for (int j = 0; j < resolution; j++) {
                // Calculate vertex indices
                int current = i * resolution + j;
                int next_row = (i + 1) * resolution + j;
                int next_col = i * resolution + (j + 1) % resolution;
                int next_both = (i + 1) * resolution + (j + 1) % resolution;
                
                // Add edges
                if (i < resolution - 1) {
                    sphere.edges.push_back({current, next_row});
                }
                sphere.edges.push_back({current, next_col});
                
                // Add faces (quads, except at poles)
                if (i < resolution - 1) {
                    sphere.faces.push_back({current, next_col, next_both, next_row});
                }
            }
        }
        
        return sphere;
    }
};

#endif // OBJECT3D_H