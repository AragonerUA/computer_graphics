#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include <cmath>
#include <iostream>
#include <stdexcept>
#include "Vector3.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Matrix4x4 {
public:
    float m[4][4];
    
    Matrix4x4() {
        identity();
    }
    
    Matrix4x4(const float values[4][4]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = values[i][j];
            }
        }
    }
    
    void identity() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }
    
    Matrix4x4 operator*(const Matrix4x4& other) const {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
    
    Matrix4x4 transpose() const {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = m[j][i];
            }
        }
        return result;
    }
    
    float det3x3(float a, float b, float c,
                 float d, float e, float f,
                 float g, float h, float i) const {
        return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
    }
    
    float determinant() const {
        float det = 0;
        det += m[0][0] * det3x3(m[1][1], m[1][2], m[1][3],
                                m[2][1], m[2][2], m[2][3],
                                m[3][1], m[3][2], m[3][3]);
        det -= m[0][1] * det3x3(m[1][0], m[1][2], m[1][3],
                                m[2][0], m[2][2], m[2][3],
                                m[3][0], m[3][2], m[3][3]);
        det += m[0][2] * det3x3(m[1][0], m[1][1], m[1][3],
                                m[2][0], m[2][1], m[2][3],
                                m[3][0], m[3][1], m[3][3]);
        det -= m[0][3] * det3x3(m[1][0], m[1][1], m[1][2],
                                m[2][0], m[2][1], m[2][2],
                                m[3][0], m[3][1], m[3][2]);
        return det;
    }
    
    Matrix4x4 inverse() const {
        Matrix4x4 result;
        float det = determinant();
        
        if (std::abs(det) < 1e-6f) {
            throw std::runtime_error("Matrix is not invertible");
        }
        
        float invDet = 1.0f / det;
        
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int sign = ((i + j) % 2 == 0) ? 1 : -1;
                
                float submatrix[9];
                int idx = 0;
                for (int k = 0; k < 4; k++) {
                    if (k == i) continue;
                    for (int l = 0; l < 4; l++) {
                        if (l == j) continue;
                        submatrix[idx++] = m[k][l];
                    }
                }
                
                float cofactor = sign * det3x3(
                    submatrix[0], submatrix[1], submatrix[2],
                    submatrix[3], submatrix[4], submatrix[5],
                    submatrix[6], submatrix[7], submatrix[8]
                );
                
                result.m[j][i] = cofactor * invDet;
            }
        }
        
        return result;
    }
    
    Vector3 transform(const Vector3& v) const {
        float x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3];
        float y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3];
        float z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3];
        float w = v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + m[3][3];
        
        if (w != 0.0f && w != 1.0f) {
            return Vector3(x / w, y / w, z / w);
        }
        
        return Vector3(x, y, z);
    }
    
    static Matrix4x4 translation(float tx, float ty, float tz) {
        Matrix4x4 result;
        result.m[0][3] = tx;
        result.m[1][3] = ty;
        result.m[2][3] = tz;
        return result;
    }
    
    static Matrix4x4 rotationX(float angleDegrees) {
        float angleRadians = angleDegrees * M_PI / 180.0f;
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);
        
        Matrix4x4 result;
        result.m[1][1] = c;
        result.m[1][2] = -s;
        result.m[2][1] = s;
        result.m[2][2] = c;
        return result;
    }
    
    static Matrix4x4 rotationY(float angleDegrees) {
        float angleRadians = angleDegrees * M_PI / 180.0f;
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);
        
        Matrix4x4 result;
        result.m[0][0] = c;
        result.m[0][2] = s;
        result.m[2][0] = -s;
        result.m[2][2] = c;
        return result;
    }
    
    static Matrix4x4 rotationZ(float angleDegrees) {
        float angleRadians = angleDegrees * M_PI / 180.0f;
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);
        
        Matrix4x4 result;
        result.m[0][0] = c;
        result.m[0][1] = -s;
        result.m[1][0] = s;
        result.m[1][1] = c;
        return result;
    }
    
    static Matrix4x4 scaling(float sx, float sy, float sz) {
        Matrix4x4 result;
        result.m[0][0] = sx;
        result.m[1][1] = sy;
        result.m[2][2] = sz;
        return result;
    }
    
    static Matrix4x4 perspective(float fovDegrees, float aspectRatio, float near, float far) {
        float fovRadians = fovDegrees * M_PI / 180.0f;
        float f = 1.0f / std::tan(fovRadians / 2.0f);
        
        Matrix4x4 result;
        result.identity();
        
        result.m[0][0] = f / aspectRatio;
        result.m[1][1] = f;
        result.m[2][2] = (far + near) / (near - far);
        result.m[2][3] = (2.0f * far * near) / (near - far);
        result.m[3][2] = -1.0f;
        result.m[3][3] = 0.0f;
        
        return result;
    }
    
    static Matrix4x4 lookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
        Vector3 forward = (target - eye).normalize();
        Vector3 right = forward.cross(up).normalize();
        Vector3 newUp = right.cross(forward);
        
        Matrix4x4 result;
        result.identity();
        
        result.m[0][0] = right.x;
        result.m[0][1] = right.y;
        result.m[0][2] = right.z;
        result.m[0][3] = -right.dot(eye);
        
        result.m[1][0] = newUp.x;
        result.m[1][1] = newUp.y;
        result.m[1][2] = newUp.z;
        result.m[1][3] = -newUp.dot(eye);
        
        result.m[2][0] = -forward.x;
        result.m[2][1] = -forward.y;
        result.m[2][2] = -forward.z;
        result.m[2][3] = forward.dot(eye);
        
        return result;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Matrix4x4& matrix) {
        for (int i = 0; i < 4; i++) {
            os << "[ ";
            for (int j = 0; j < 4; j++) {
                os << matrix.m[i][j] << " ";
            }
            os << "]" << std::endl;
        }
        return os;
    }
};

#endif