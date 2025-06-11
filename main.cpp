#include <iostream>
#include <vector>

// macOS specific OpenGL includes
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include "Vector3.h"
#include "Matrix4x4.h"
#include "Object3D.h"
#include "TransformationPipeline.h"
#include "TextureLoader.h"

// Global variables for application state
int windowWidth = 800;
int windowHeight = 600;
bool wireframeMode = true;
bool depthTestEnabled = true;
bool lightingEnabled = true;
bool texturesEnabled = true;

// Object transformation parameters
Vector3 objectPosition(0.0f, 0.0f, 0.0f);
Vector3 objectRotation(0.0f, 0.0f, 0.0f);
Vector3 objectScale(1.0f, 1.0f, 1.0f);

// Camera parameters
Vector3 cameraPosition(0.0f, 0.0f, 5.0f);
Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
Vector3 cameraUp(0.0f, 1.0f, 0.0f);

// Light parameters
Vector3 lightPosition(3.0f, 3.0f, 3.0f);
float ambientIntensity = 0.2f;
float diffuseIntensity = 0.7f;
float specularIntensity = 0.5f;
float shininess = 32.0f;

// Current object selection
int currentObjectIndex = 0;
std::vector<Object3D> objects;
std::vector<GLuint> textures;

// TransformationPipeline instance
TransformationPipeline pipeline;

// Initialize lighting
void setupLighting() {
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        
        // Set ambient lighting
        GLfloat ambientLight[] = { ambientIntensity, ambientIntensity, ambientIntensity, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
        
        // Configure light source 0
        GLfloat lightPos[] = { lightPosition.x, lightPosition.y, lightPosition.z, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        
        GLfloat diffuseLight[] = { diffuseIntensity, diffuseIntensity, diffuseIntensity, 1.0f };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        
        GLfloat specularLight[] = { specularIntensity, specularIntensity, specularIntensity, 1.0f };
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    } else {
        glDisable(GL_LIGHTING);
    }
}

// Function to handle rendering
void display() {
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
    
    // Set up lighting
    setupLighting();
    
    // Set model transformation
    pipeline.setModelTransform(objectPosition, objectRotation, objectScale);
    
    // Set camera position
    pipeline.setViewTransform(cameraPosition, cameraTarget, cameraUp);
    
    // Set projection
    pipeline.setProjection(45.0f, static_cast<float>(windowWidth) / windowHeight, 0.1f, 100.0f);
    
    // Render the current object
    const Object3D& object = objects[currentObjectIndex];
    
    // Enable texturing if not in wireframe mode
    if (!wireframeMode && texturesEnabled) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[currentObjectIndex]);
    } else {
        glDisable(GL_TEXTURE_2D);
    }
    
    // Set material properties
    GLfloat materialAmbient[] = { object.color[0] * 0.2f, object.color[1] * 0.2f, object.color[2] * 0.2f, 1.0f };
    GLfloat materialDiffuse[] = { object.color[0], object.color[1], object.color[2], 1.0f };
    GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // White specular highlight
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    // Draw edges in wireframe mode
    if (wireframeMode) {
        // Disable lighting for wireframe rendering
        glDisable(GL_LIGHTING);
        // Set the object's color directly
        glColor3f(object.color[0], object.color[1], object.color[2]);
        
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
        
        // Re-enable lighting if it was on
        if (lightingEnabled) {
            glEnable(GL_LIGHTING);
        }
    } else {
        // Draw filled polygons
        for (const auto& face : object.faces) {
            if (face.size() == 3) {
                glBegin(GL_TRIANGLES);
            } else {
                glBegin(GL_POLYGON);
            }
            
            // For each vertex in the face
            for (size_t i = 0; i < face.size(); i++) {
                int vertexIndex = face[i];
                
                // Set normal vector for lighting calculations
                if (lightingEnabled && !object.normals.empty()) {
                    const Vector3& normal = object.normals[vertexIndex];
                    glNormal3f(normal.x, normal.y, normal.z);
                }
                
                // Set texture coordinates if texturing is enabled
                if (texturesEnabled && !object.texCoords.empty()) {
                    // If vertexIndex is out of range for texCoords, use a default
                    if (vertexIndex < object.texCoords.size()) {
                        glTexCoord2f(object.texCoords[vertexIndex].first, 
                                    object.texCoords[vertexIndex].second);
                    } else {
                        glTexCoord2f(0.0f, 0.0f);
                    }
                }
                
                const Vector3& vertex = object.vertices[vertexIndex];
                Vector3 transformed = pipeline.applyMVP(vertex);
                glVertex3f(transformed.x, transformed.y, transformed.z);
            }
            
            glEnd();
        }
    }
    
    // Disable texturing
    glDisable(GL_TEXTURE_2D);
    
    // Restore default polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // Swap buffers
    glutSwapBuffers();
}

// Handle window resize
void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

// Handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.1f;
    float rotateSpeed = 5.0f;
    float scaleSpeed = 0.05f;
    
    switch (key) {
        // Object translation
        case 'w': objectPosition.z -= moveSpeed; break;  // Move forward
        case 's': objectPosition.z += moveSpeed; break;  // Move backward
        case 'a': objectPosition.x -= moveSpeed; break;  // Move left
        case 'd': objectPosition.x += moveSpeed; break;  // Move right
        case 'q': objectPosition.y += moveSpeed; break;  // Move up
        case 'e': objectPosition.y -= moveSpeed; break;  // Move down
        
        // Object rotation (using lowercase for Z axis since special keys are handled separately)
        case 'z': objectRotation.z += rotateSpeed; break;  // Rotate around Z axis
        case 'x': objectRotation.z -= rotateSpeed; break;  // Rotate around Z axis
        
        // Object scaling
        case '+': case '=':  // Scale up
            objectScale.x += scaleSpeed;
            objectScale.y += scaleSpeed;
            objectScale.z += scaleSpeed;
            break;
        case '-': case '_':  // Scale down
            if (objectScale.x > scaleSpeed && objectScale.y > scaleSpeed && objectScale.z > scaleSpeed) {
                objectScale.x -= scaleSpeed;
                objectScale.y -= scaleSpeed;
                objectScale.z -= scaleSpeed;
            }
            break;
        
        // Reset transformations
        case 'r':
            objectPosition = Vector3(0.0f, 0.0f, 0.0f);
            objectRotation = Vector3(0.0f, 0.0f, 0.0f);
            objectScale = Vector3(1.0f, 1.0f, 1.0f);
            break;
        
        // Toggle wireframe mode
        case 'f':
            wireframeMode = !wireframeMode;
            break;
        
        // Toggle depth testing
        case 't':
            depthTestEnabled = !depthTestEnabled;
            break;
            
        // Toggle lighting
        case 'l':
            lightingEnabled = !lightingEnabled;
            break;
            
        // Toggle textures
        case 'g':
            texturesEnabled = !texturesEnabled;
            break;
        
        // Change object
        case '\t':  // Tab key
            currentObjectIndex = (currentObjectIndex + 1) % objects.size();
            break;
        
        // Exit application
        case 27:  // Escape key
            TextureLoader::cleanup();  // Clean up textures
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

// Handle special keys (arrow keys)
void specialKeys(int key, int x, int y) {
    float rotateSpeed = 5.0f;
    
    switch (key) {
        case GLUT_KEY_UP:
            objectRotation.x += rotateSpeed;  // Rotate around X axis
            break;
        case GLUT_KEY_DOWN:
            objectRotation.x -= rotateSpeed;  // Rotate around X axis
            break;
        case GLUT_KEY_LEFT:
            objectRotation.y += rotateSpeed;  // Rotate around Y axis
            break;
        case GLUT_KEY_RIGHT:
            objectRotation.y -= rotateSpeed;  // Rotate around Y axis
            break;
    }
    
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Transformation and Rendering");
    
    // Set up callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    // Set background color
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    // Set up OpenGL features
    glShadeModel(GL_SMOOTH);  // Enable smooth shading
    glEnable(GL_DEPTH_TEST);  // Enable depth testing
    glEnable(GL_NORMALIZE);   // Normalize normals for proper lighting
    
    // Create 3D objects
    objects.push_back(Object3D::createCube(1.0f));
    objects.push_back(Object3D::createPyramid(1.0f, 1.5f));
    objects.push_back(Object3D::createTetrahedron(1.0f));
    objects.push_back(Object3D::createSphere(1.0f, 12));
    
    // Set object colors
    objects[0].setColor(1.0f, 0.0f, 0.0f);  // Red cube
    objects[1].setColor(0.0f, 1.0f, 0.0f);  // Green pyramid
    objects[2].setColor(0.0f, 0.0f, 1.0f);  // Blue tetrahedron
    objects[3].setColor(1.0f, 1.0f, 0.0f);  // Yellow sphere
    
    // Create textures for each object
    textures.push_back(TextureLoader::createProceduralTexture("checkerboard"));
    textures.push_back(TextureLoader::createProceduralTexture("brick"));
    textures.push_back(TextureLoader::createProceduralTexture("gradient"));
    textures.push_back(TextureLoader::createProceduralTexture("checkerboard"));
    
    // Initialize transformation pipeline
    pipeline = TransformationPipeline();
    
    // Print instructions
    std::cout << "==== 3D Transformation and Rendering ====" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD: Move object in X/Z plane" << std::endl;
    std::cout << "  Q/E: Move object up/down" << std::endl;
    std::cout << "  Arrow keys: Rotate object around X/Y axis" << std::endl;
    std::cout << "  Z/X: Rotate object around Z axis" << std::endl;
    std::cout << "  +/-: Scale object up/down" << std::endl;
    std::cout << "  R: Reset transformations" << std::endl;
    std::cout << "  F: Toggle wireframe mode" << std::endl;
    std::cout << "  T: Toggle depth test" << std::endl;
    std::cout << "  L: Toggle lighting" << std::endl;
    std::cout << "  G: Toggle textures" << std::endl;
    std::cout << "  TAB: Switch between objects" << std::endl;
    std::cout << "  ESC: Exit application" << std::endl;
    
    // Start main loop
    glutMainLoop();
    
    return 0;
}