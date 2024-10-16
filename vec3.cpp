#include "vec3.h"
#include <cmath>

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
Vec3 Vec3::operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
Vec3 Vec3::operator*(float s) const { return Vec3(x * s, y * s, z * s); }

Vec3 Vec3::prod_vetorial(const Vec3& v) const {
    return Vec3(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}

float Vec3::prod_interno(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

Vec3 Vec3::normalize() const {
    float len = sqrt(x * x + y * y + z * z);
    return Vec3(x / len, y / len, z / len);
}

float Vec3::len() const {
    return sqrt(x * x + y * y + z * z);
}