#ifndef TRANSFORMATION_PIPELINE_H
#define TRANSFORMATION_PIPELINE_H

#include "Vector3.h"
#include "Matrix4x4.h"

class TransformationPipeline {
public:
    Matrix4x4 modelMatrix;
    Matrix4x4 viewMatrix;
    Matrix4x4 projectionMatrix;
    
    TransformationPipeline() {
        // Initialize with identity matrices
        resetTransformations();
    }
    
    void resetTransformations() {
        modelMatrix = Matrix4x4();
        viewMatrix = Matrix4x4();
        projectionMatrix = Matrix4x4();
    }
    
    // Set model transform (object's position, rotation, scale)
    void setModelTransform(const Vector3& translation, 
                           const Vector3& rotation, 
                           const Vector3& scale) {
        // Combine transformations (scale, then rotate, then translate)
        Matrix4x4 translationMatrix = Matrix4x4::translation(translation.x, translation.y, translation.z);
        Matrix4x4 rotationMatrixX = Matrix4x4::rotationX(rotation.x);
        Matrix4x4 rotationMatrixY = Matrix4x4::rotationY(rotation.y);
        Matrix4x4 rotationMatrixZ = Matrix4x4::rotationZ(rotation.z);
        Matrix4x4 scaleMatrix = Matrix4x4::scaling(scale.x, scale.y, scale.z);
        
        // Combine rotations
        Matrix4x4 rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;
        
        // Apply transformations in correct order: scale -> rotate -> translate
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    }
    
    // Set view transform (camera's position and orientation)
    void setViewTransform(const Vector3& cameraPos, const Vector3& target, const Vector3& up) {
        viewMatrix = Matrix4x4::lookAt(cameraPos, target, up);
    }
    
    // Set projection transform (how 3D is projected to 2D)
    void setProjection(float fov, float aspectRatio, float near, float far) {
        projectionMatrix = Matrix4x4::perspective(fov, aspectRatio, near, far);
    }
    
    // Apply the full Model-View-Projection matrix to a vertex
    Vector3 applyMVP(const Vector3& vertex) const {
        // Apply transformations in order: model -> view -> projection
        Matrix4x4 mvp = projectionMatrix * viewMatrix * modelMatrix;
        return mvp.transform(vertex);
    }
    
    // Convert 3D clip space to 2D screen coordinates
    Vector3 clipToScreen(const Vector3& clipSpaceCoord, int screenWidth, int screenHeight) const {
        // Convert from clip space [-1,1] to screen space [0, width/height]
        float screenX = (clipSpaceCoord.x + 1.0f) * 0.5f * screenWidth;
        float screenY = (1.0f - clipSpaceCoord.y) * 0.5f * screenHeight; // Y is flipped in screen space
        
        // Z is preserved for depth testing
        return Vector3(screenX, screenY, clipSpaceCoord.z);
    }
    
    // Transform a vertex all the way from model space to screen space
    Vector3 transformVertexToScreen(const Vector3& vertex, int screenWidth, int screenHeight) const {
        Vector3 clipSpaceCoord = applyMVP(vertex);
        return clipToScreen(clipSpaceCoord, screenWidth, screenHeight);
    }
};

#endif // TRANSFORMATION_PIPELINE_H