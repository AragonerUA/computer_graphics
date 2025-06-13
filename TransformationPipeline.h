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
        resetTransformations();
    }
    
    void resetTransformations() {
        modelMatrix = Matrix4x4();
        viewMatrix = Matrix4x4();
        projectionMatrix = Matrix4x4();
    }
    
    void setModelTransform(const Vector3& translation, 
                           const Vector3& rotation, 
                           const Vector3& scale) {
        Matrix4x4 translationMatrix = Matrix4x4::translation(translation.x, translation.y, translation.z);
        Matrix4x4 rotationMatrixX = Matrix4x4::rotationX(rotation.x);
        Matrix4x4 rotationMatrixY = Matrix4x4::rotationY(rotation.y);
        Matrix4x4 rotationMatrixZ = Matrix4x4::rotationZ(rotation.z);
        Matrix4x4 scaleMatrix = Matrix4x4::scaling(scale.x, scale.y, scale.z);
        
        Matrix4x4 rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;
        
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    }
    
    void setViewTransform(const Vector3& cameraPos, const Vector3& target, const Vector3& up) {
        viewMatrix = Matrix4x4::lookAt(cameraPos, target, up);
    }
    
    void setProjection(float fov, float aspectRatio, float near, float far) {
        projectionMatrix = Matrix4x4::perspective(fov, aspectRatio, near, far);
    }
    
    Vector3 applyMVP(const Vector3& vertex) const {
        Matrix4x4 mvp = projectionMatrix * viewMatrix * modelMatrix;
        return mvp.transform(vertex);
    }
    
    Vector3 clipToScreen(const Vector3& clipSpaceCoord, int screenWidth, int screenHeight) const {
        float screenX = (clipSpaceCoord.x + 1.0f) * 0.5f * screenWidth;
        float screenY = (1.0f - clipSpaceCoord.y) * 0.5f * screenHeight;
        
        return Vector3(screenX, screenY, clipSpaceCoord.z);
    }
    
    Vector3 transformVertexToScreen(const Vector3& vertex, int screenWidth, int screenHeight) const {
        Vector3 clipSpaceCoord = applyMVP(vertex);
        return clipToScreen(clipSpaceCoord, screenWidth, screenHeight);
    }
};

#endif