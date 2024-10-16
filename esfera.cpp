#include "esfera.h"
#include <cmath>

Esfera::Esfera(const Vec3& centro, float raio, const Vec3& cor)
    : Objeto(cor), centro(centro), raio(raio) {}

Interseccao Esfera::intersect(const Vec3& origem_raio, const Vec3& direcao_raio) const {
    Interseccao resultado = { false, 0, Vec3(), Vec3() };

    // Cálculo da intersecção entre o raio e a esfera
    Vec3 oc = origem_raio - centro;
    float a = direcao_raio.prod_interno(direcao_raio);
    float b = 2.0f * oc.prod_interno(direcao_raio);
    float c = oc.prod_interno(oc) - raio * raio;
    float delta = b * b - 4 * a * c;

    if (delta < 0) {
        return resultado;
    } else {
        float t = (-b - sqrt(delta)) / (2.0f * a);
        // Se t > 0, o raio atravessa a esfera, ou seja, será refletido nela
        if (t > 0) {
            resultado.encontrou = true;
            resultado.distancia = t;
            resultado.ponto_encontro = origem_raio + direcao_raio * t;
            resultado.normal = (resultado.ponto_encontro - centro).normalize();
        }
    }
    return resultado;
}