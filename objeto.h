#ifndef OBJETO_H
#define OBJETO_H

#include <string>
#include "Vec3.h"

// Struct com característica de intersecção entre objeto e raio
struct Interseccao {
    bool encontrou;
    float distancia;
    Vec3 ponto_encontro;
    Vec3 normal;
};

// Classe abstrata para ser herdada pelos objetos utilizados
class Objeto {
public:
    Vec3 cor;
    Objeto(const Vec3& cor);
    virtual ~Objeto() {}
    virtual Interseccao intersect(const Vec3& origem_raio, const Vec3& direcao_raio) const = 0;
};

#endif