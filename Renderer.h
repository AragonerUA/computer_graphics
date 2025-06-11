#ifndef RENDERER_H
#define RENDERER_H

// macOS specific OpenGL includes
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include <vector>
#include "Vector3.h"
#include "Object3D.h"
#include "TransformationPipeline.h"

class Renderer {
private:
    int width;
    int height;
    TransformationPipeline pipeline;
    bool wireframeMode = true;
    bool depthTestEnabled = true;
    
public:
    Renderer(int width, int height) : width(width), height(height) {
        // Initialize the transformation pipeline
        pipeline.resetTransformations();
        
        // Set default camera position
        pipeline.setViewTransform(
            Vector3(0.0f, 0.0f, 5.0f), // Camera position
            Vector3(0.0f, 0.0f, 0.0f), // Target (looking at)
            Vector3(0.0f, 1.0f, 0.0f)  // Up direction
        );
        
        // Set default projection
        pipeline.setProjection(45.0f, static_cast<float>(width) / height, 0.1f, 100.0f);
    }
    
    // Set the model transformation for rendered objects
    void setModelTransform(const Vector3& translation, const Vector3& rotation, const Vector3& scale) {
        pipeline.setModelTransform(translation, rotation, scale);
    }
    
    // Set the camera position and orientation
    void setCameraPosition(const Vector3& position, const Vector3& target, const Vector3& up) {
        pipeline.setViewTransform(position, target, up);
    }
    
    // Toggle wireframe mode
    void toggleWireframe() {
        wireframeMode = !wireframeMode;
    }
    
    // Toggle depth testing
    void toggleDepthTest() {
        depthTestEnabled = !depthTestEnabled;
    }

    // Accessor methods
    bool isWireframeMode() const {
        return wireframeMode;
    }

    bool isDepthTestEnabled() const {
        return depthTestEnabled;
    }
    
    // Set up OpenGL state before rendering
    void beginFrame() {
        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set up depth testing
        if (depthTestEnabled) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
        
        // Set wireframe or filled polygon mode
        if (wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
    
    // Render a 3D object
    void renderObject(const Object3D& object) {
        // Set the object's color
        glColor3f(object.color[0], object.color[1], object.color[2]);
        
        // Draw edges in wireframe mode
        if (wireframeMode) {
            glBegin(GL_LINES);
            for (const auto& edge : object.edges) {
                // Get the vertices at each end of the edge
                const Vector3& v1 = object.vertices[edge.first];
                const Vector3& v2 = object.vertices[edge.second];
                
                // Transform the vertices and pass them to OpenGL
                Vector3 transformedV1 = pipeline.applyMVP(v1);
                Vector3 transformedV2 = pipeline.applyMVP(v2);
                
                glVertex3f(transformedV1.x, transformedV1.y, transformedV1.z);
                glVertex3f(transformedV2.x, transformedV2.y, transformedV2.z);
            }
            glEnd();
        } else {
            // Draw filled polygons
            for (const auto& face : object.faces) {
                if (face.size() == 3) {
                    glBegin(GL_TRIANGLES);
                } else {
                    glBegin(GL_POLYGON);
                }
                
                // For each vertex in the face
                for (int vertexIndex : face) {
                    const Vector3& vertex = object.vertices[vertexIndex];
                    Vector3 transformed = pipeline.applyMVP(vertex);
                    glVertex3f(transformed.x, transformed.y, transformed.z);
                }
                
                glEnd();
            }
        }
    }
    
    // Clean up after frame is done
    void endFrame() {
        // Reset to default modes
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
};

#endif // RENDERER_H