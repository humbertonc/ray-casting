#ifndef VEC3_H
#define VEC3_H

struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0);

    Vec3 operator-(const Vec3& v) const;
    Vec3 operator+(const Vec3& v) const;
    Vec3 operator*(float s) const;

    Vec3 prod_vetorial(const Vec3& v) const;
    float prod_interno(const Vec3& v) const;
    Vec3 normalize() const;
    float len() const;
};

#endif