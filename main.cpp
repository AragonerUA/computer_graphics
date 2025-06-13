#include <iostream>
#include <vector>
#include <string>

// macOS specific OpenGL includes
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include "Vector3.h"
#include "Matrix4x4.h"
#include "Object3D.h"
#include "TransformationPipeline.h"
#include "TextureLoader.h"

int windowWidth = 800;
int windowHeight = 600;
bool wireframeMode = true;
bool depthTestEnabled = true;
bool lightingEnabled = true;
bool texturesEnabled = true;
bool showInstructions = true;

Vector3 objectPosition(0.0f, 0.0f, 0.0f);
Vector3 objectRotation(0.0f, 0.0f, 0.0f);
Vector3 objectScale(1.0f, 1.0f, 1.0f);

Vector3 cameraPosition(0.0f, 0.0f, 5.0f);
Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
Vector3 cameraUp(0.0f, 1.0f, 0.0f);

Vector3 lightPosition(3.0f, 3.0f, 3.0f);
float ambientIntensity = 0.2f;
float diffuseIntensity = 0.7f;
float specularIntensity = 0.5f;
float shininess = 32.0f;

int currentObjectIndex = 0;
std::vector<Object3D> objects;
std::vector<GLuint> textures;

std::vector<std::string> objectNames = {"Cube", "Pyramid", "Tetrahedron", "Sphere"};

TransformationPipeline pipeline;

void setupLighting() {
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        
        GLfloat ambientLight[] = { ambientIntensity, ambientIntensity, ambientIntensity, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
        
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

void renderText(float x, float y, const char *text) {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glRasterPos2f(x, y);
    
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    if (lightingEnabled && !wireframeMode) {
        glEnable(GL_LIGHTING);
    }
    if (texturesEnabled && !wireframeMode) {
        glEnable(GL_TEXTURE_2D);
    }
}

void displayInstructions() {
    if (!showInstructions) return;
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    char buffer[100];
    sprintf(buffer, "Current object: %s", objectNames[currentObjectIndex].c_str());
    renderText(10, windowHeight - 20, buffer);
    
    sprintf(buffer, "Wireframe: %s | Depth Test: %s | Lighting: %s | Textures: %s",
            wireframeMode ? "ON" : "OFF",
            depthTestEnabled ? "ON" : "OFF",
            lightingEnabled ? "ON" : "OFF",
            texturesEnabled ? "ON" : "OFF");
    renderText(10, windowHeight - 40, buffer);
    
    renderText(10, windowHeight - 70, "Controls:");
    renderText(10, windowHeight - 90, "WASD: Move | Q/E: Up/Down | Arrows: Rotate X/Y | Z/X: Rotate Z");
    renderText(10, windowHeight - 110, "+/-: Scale | R: Reset | F: Wireframe | T: Depth Test");
    renderText(10, windowHeight - 130, "L: Lighting | G: Textures | TAB: Switch Object | H: Hide/Show Help");
    
    sprintf(buffer, "Position: (%.1f, %.1f, %.1f)", objectPosition.x, objectPosition.y, objectPosition.z);
    renderText(10, 50, buffer);
    
    sprintf(buffer, "Rotation: (%.1f, %.1f, %.1f)", objectRotation.x, objectRotation.y, objectRotation.z);
    renderText(10, 30, buffer);
    
    sprintf(buffer, "Scale: (%.2f, %.2f, %.2f)", objectScale.x, objectScale.y, objectScale.z);
    renderText(10, 10, buffer);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        cameraPosition.x, cameraPosition.y, cameraPosition.z,
        cameraTarget.x, cameraTarget.y, cameraTarget.z,
        cameraUp.x, cameraUp.y, cameraUp.z
    );
    
    glTranslatef(objectPosition.x, objectPosition.y, objectPosition.z);
    glRotatef(objectRotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(objectRotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(objectRotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(objectScale.x, objectScale.y, objectScale.z);
    
    if (depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    
    if (wireframeMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    setupLighting();
    
    const Object3D& object = objects[currentObjectIndex];
    
    if (!wireframeMode && texturesEnabled) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[currentObjectIndex]);
    } else {
        glDisable(GL_TEXTURE_2D);
    }
    
    GLfloat materialAmbient[] = { object.color[0] * 0.2f, object.color[1] * 0.2f, object.color[2] * 0.2f, 1.0f };
    GLfloat materialDiffuse[] = { object.color[0], object.color[1], object.color[2], 1.0f };
    GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    if (wireframeMode) {
        glDisable(GL_LIGHTING);
        glColor3f(object.color[0], object.color[1], object.color[2]);
        
        glBegin(GL_LINES);
        for (const auto& edge : object.edges) {
            const Vector3& v1 = object.vertices[edge.first];
            const Vector3& v2 = object.vertices[edge.second];
            
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
        }
        glEnd();
        
        if (lightingEnabled) {
            glEnable(GL_LIGHTING);
        }
    } else {
        for (const auto& face : object.faces) {
            if (face.size() == 3) {
                glBegin(GL_TRIANGLES);
            } else {
                glBegin(GL_POLYGON);
            }
            
            for (size_t i = 0; i < face.size(); i++) {
                int vertexIndex = face[i];
                
                if (lightingEnabled && !object.normals.empty() && vertexIndex < object.normals.size()) {
                    const Vector3& normal = object.normals[vertexIndex];
                    glNormal3f(normal.x, normal.y, normal.z);
                }
                
                if (texturesEnabled && !object.texCoords.empty() && vertexIndex < object.texCoords.size()) {
                    glTexCoord2f(object.texCoords[vertexIndex].first, 
                                object.texCoords[vertexIndex].second);
                }
                
                const Vector3& vertex = object.vertices[vertexIndex];
                glVertex3f(vertex.x, vertex.y, vertex.z);
            }
            
            glEnd();
        }
    }
    
    displayInstructions();
    
    glDisable(GL_TEXTURE_2D);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glutSwapBuffers();
}

void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.1f;
    float rotateSpeed = 5.0f;
    float scaleSpeed = 0.05f;
    
    switch (key) {
        case 'w': objectPosition.z -= moveSpeed; break;  // Move forward
        case 's': objectPosition.z += moveSpeed; break;  // Move backward
        case 'a': objectPosition.x -= moveSpeed; break;  // Move left
        case 'd': objectPosition.x += moveSpeed; break;  // Move right
        case 'q': objectPosition.y += moveSpeed; break;  // Move up
        case 'e': objectPosition.y -= moveSpeed; break;  // Move down
        
        case 'z': objectRotation.z += rotateSpeed; break;
        case 'x': objectRotation.z -= rotateSpeed; break;

        case '+': case '=':
            objectScale.x += scaleSpeed;
            objectScale.y += scaleSpeed;
            objectScale.z += scaleSpeed;
            break;
        case '-': case '_':
            if (objectScale.x > scaleSpeed && objectScale.y > scaleSpeed && objectScale.z > scaleSpeed) {
                objectScale.x -= scaleSpeed;
                objectScale.y -= scaleSpeed;
                objectScale.z -= scaleSpeed;
            }
            break;
        
        case 'r':
            objectPosition = Vector3(0.0f, 0.0f, 0.0f);
            objectRotation = Vector3(0.0f, 0.0f, 0.0f);
            objectScale = Vector3(1.0f, 1.0f, 1.0f);
            break;
        
        case 'f':
            wireframeMode = !wireframeMode;
            break;
        
        case 't':
            depthTestEnabled = !depthTestEnabled;
            break;
            
        case 'l':
            lightingEnabled = !lightingEnabled;
            break;
            
        case 'g':
            texturesEnabled = !texturesEnabled;
            break;
        
        case 'h': case 'H':
            showInstructions = !showInstructions;
            break;
        
        case '\t':
            currentObjectIndex = (currentObjectIndex + 1) % objects.size();
            break;
        
        case 27:
            TextureLoader::cleanup();
            exit(0);
            break;
    }
    
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    float rotateSpeed = 5.0f;
    
    switch (key) {
        case GLUT_KEY_UP:
            objectRotation.x += rotateSpeed;
            break;
        case GLUT_KEY_DOWN:
            objectRotation.x -= rotateSpeed;
            break;
        case GLUT_KEY_LEFT:
            objectRotation.y += rotateSpeed;
            break;
        case GLUT_KEY_RIGHT:
            objectRotation.y -= rotateSpeed;
            break;
    }
    
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Transformation and Rendering");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    objects.push_back(Object3D::createCube(1.0f));
    objects.push_back(Object3D::createPyramid(1.0f, 1.5f));
    objects.push_back(Object3D::createTetrahedron(1.0f));
    objects.push_back(Object3D::createSphere(1.0f, 12));
    
    objects[0].setColor(1.0f, 0.0f, 0.0f);  // Red cube
    objects[1].setColor(0.0f, 1.0f, 0.0f);  // Green pyramid
    objects[2].setColor(0.0f, 0.0f, 1.0f);  // Blue tetrahedron
    objects[3].setColor(1.0f, 1.0f, 0.0f);  // Yellow sphere
    
    textures.push_back(TextureLoader::createProceduralTexture("checkerboard"));
    textures.push_back(TextureLoader::createProceduralTexture("brick"));
    textures.push_back(TextureLoader::createProceduralTexture("gradient"));
    textures.push_back(TextureLoader::createProceduralTexture("checkerboard"));
    
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
    std::cout << "  H: Toggle on-screen instructions" << std::endl;
    std::cout << "  TAB: Switch between objects" << std::endl;
    std::cout << "  ESC: Exit application" << std::endl;
    
    glutMainLoop();
    
    return 0;
}