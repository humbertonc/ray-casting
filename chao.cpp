#include "chao.h"

Chao::Chao(float y, float xmin, float xmax, float zmin, float zmax, const Vec3& cor)
    : Objeto(cor), y(y), xmin(xmin), xmax(xmax), zmin(zmin), zmax(zmax) {}

Interseccao Chao::intersect(const Vec3& origem_raio, const Vec3& direcao_raio) const {
    Interseccao resultado = { false, 0, Vec3(), Vec3() };

    if (direcao_raio.y != 0) {
        // Calcula distancia do raio ao ponto
        float t = (y - origem_raio.y) / direcao_raio.y;
        // Se t for negativo, o raio está em um ângulo que não encontrará o chão
        if (t > 0) {
            Vec3 ponto_encontro = origem_raio + direcao_raio * t;
            // Se o ponto que o raio encontra o plano do chão está contido nas dimensões do objeto
            if (ponto_encontro.x >= xmin && ponto_encontro.x <= xmax && ponto_encontro.z >= zmin && ponto_encontro.z <= zmax) {
                resultado.encontrou = true;
                resultado.distancia = t;
                resultado.ponto_encontro = ponto_encontro;
                resultado.normal = Vec3(0, 1, 0);
            }
        }
    }
    return resultado;
}