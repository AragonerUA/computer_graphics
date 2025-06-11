#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <cmath>

class Vector3 {
public:
    float x, y, z;

    // Constructors
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    // Basic operations
    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    
    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    
    // Dot product
    float dot(const Vector3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    
    // Cross product
    Vector3 cross(const Vector3& v) const {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }
    
    // Magnitude (length)
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    // Normalize
    Vector3 normalize() const {
        float mag = magnitude();
        if (mag == 0.0f)
            return Vector3(0.0f, 0.0f, 0.0f);
        return Vector3(x / mag, y / mag, z / mag);
    }
    
    // For debugging
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    // Homogeneous coordinates conversion
    float* toHomogeneous() const {
        float* result = new float[4];
        result[0] = x;
        result[1] = y;
        result[2] = z;
        result[3] = 1.0f;
        return result;
    }
};

#endif // VECTOR3_H