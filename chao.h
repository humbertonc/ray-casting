#ifndef CHAO_H
#define CHAO_H

#include "objeto.h"

class Chao : public Objeto {
public:
    float y, xmin, xmax, zmin, zmax;
    Chao(float y, float xmin, float xmax, float zmin, float zmax, const Vec3& cor);
    Interseccao intersect(const Vec3& origem_raio, const Vec3& direcao_raio) const override;
};

#endif