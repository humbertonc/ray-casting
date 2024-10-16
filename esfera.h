#ifndef ESFERA_H
#define ESFERA_H

#include "objeto.h"

class Esfera : public Objeto {
public:
    Vec3 centro;
    float raio;
    Esfera(const Vec3& centro, float raio, const Vec3& cor);
    Interseccao intersect(const Vec3& origem_raio, const Vec3& direcao_raio) const override;
};

#endif