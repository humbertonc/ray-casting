#include <GL/gl.h>
#include <GL/glut.h>
#include <algorithm>
#include <cstdio>
#include <vector>
#include <memory>
#include <cmath>
#include "vec3.h"
#include "objeto.h"
#include "esfera.h"
#include "chao.h"

using namespace std;

const int largura = 500;
const int altura = 500;
Vec3 pos_luz(0, 12, -5);
Vec3 pos_camera(0, 0, 0);
Vec3 dir_camera(0, 0, -1);
float rot_horizontal = 0.0f;
float rot_vertical = 0.0f;

vector<Objeto *> arr_objetos;

// Função para movimentar a câmera
void keyboard_handle(unsigned char key, int x, int y) {
    const float vel_rotacao = 0.05;
    const float vel_movimento = 0.1;

    // Vetor apontando para a direita da câmera
    Vec3 vec_direita = Vec3(0, 1, 0).prod_vetorial(dir_camera).normalize();

    switch (key) {
    case 'w': // Ir para frente
        pos_camera = pos_camera + dir_camera * vel_movimento;
        break;
    case 's': // Ir para trás
        pos_camera = pos_camera - dir_camera * vel_movimento;
        break;
    case 'a': // Ir para a esquerda
        pos_camera = pos_camera - vec_direita * vel_movimento;
        break;
    case 'd': // Ir para a direita
        pos_camera = pos_camera + vec_direita * vel_movimento;
        break;
    case 'i': // Virar para cima
        rot_vertical += vel_rotacao;
        break;
    case 'k': // Virar para baixo
        rot_vertical -= vel_rotacao;
        break;
    case 'j': // Virar para a esquerda
        rot_horizontal += vel_rotacao;
        break;
    case 'l': // Virar para a direita
        rot_horizontal -= vel_rotacao;
        break;
    case 32: // Ir para cima
        pos_camera.y += vel_movimento;
        break;
    case 'x': // Ir para baixo
        pos_camera.y -= vel_movimento;
        break;
    default: 
        break;
    }

    // Não permite a movimentação vertical "dar a volta"
    rot_vertical = clamp(rot_vertical, static_cast<float>(-M_PI_2 + 0.01), static_cast<float>(M_PI_2 - 0.01));

    // Calcula o look at a partir dos ângulos de rotação
    dir_camera.x = cos(rot_vertical) * sin(rot_horizontal);
    dir_camera.y = sin(rot_vertical);
    dir_camera.z = -cos(rot_vertical) * cos(rot_horizontal);
    dir_camera = dir_camera.normalize();
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Calcula os vetores xc e yc, a partir do look at e do vup
    Vec3 x_camera = Vec3(0, 1, 0).prod_vetorial(dir_camera).normalize();
    Vec3 y_camera = dir_camera.prod_vetorial(x_camera).normalize();

    glBegin(GL_POINTS);
    // Iterando sobre todos os pixels
    for (int x = 0; x < largura; x++) {
        for (int y = 0; y < altura; y++) {
            // Normaliza valores dos pixels entre -1 e 1
            float x_normalizado = (2.0f * x / largura) - 1.0f;
            float y_normalizado = 1.0f - (2.0f * y / altura);

            // Calcula a direção do raio a ser projetado
            Vec3 direcao_raio = (dir_camera + x_camera * x_normalizado + y_camera * y_normalizado).normalize();

            float dist_min = INFINITY;
            Objeto* obj_mais_prox = nullptr;
            Vec3 ponto_mais_prox;
            Vec3 normal_ponto_mais_prox;

            // Itera sobre cada objeto, calculando se o raio passa por ele
            for (int i = 0; i < arr_objetos.size(); ++i)  {
                Interseccao hit = arr_objetos[i]->intersect(pos_camera, direcao_raio);
                // Se encontrar um objeto e a distância dele for menor que a atual, ele vira o objeto mais próximo da câmera
                if (hit.encontrou && hit.distancia < dist_min) {
                    dist_min = hit.distancia;
                    obj_mais_prox = arr_objetos[i];
                    ponto_mais_prox = hit.ponto_encontro;
                    normal_ponto_mais_prox = hit.normal;
                }
            }

            // Se encontrar objeto, calcular a intensidade da luz no ponto, dependendo do angulo e sombreamento
            if (obj_mais_prox) {
                Vec3 dir_luz = (pos_luz - ponto_mais_prox).normalize();
                // Cálculo da intensidade a partir do ângulo de incidência (mínimo de 0.1 como luz ambiente)
                float intensidade = max(0.1f, normal_ponto_mais_prox.prod_interno(dir_luz));

                // Calcular se raio do ponto de intersecção até luz encontra algum objeto no caminho
                for (int i = 0; i < arr_objetos.size(); ++i) {
                    if (dynamic_cast<Esfera*>(arr_objetos[i])) {
                        Interseccao hit_sombra = arr_objetos[i]->intersect(ponto_mais_prox, dir_luz);

                        // Se encontrar objeto entre o ponto e a fonte de luz, ponto está na sombra e a intensidade da cor é reduzida em 10 vezes
                        if (hit_sombra.encontrou && hit_sombra.distancia < (pos_luz - ponto_mais_prox).len()) {
                            intensidade *= 0.1;
                            break;
                        }
                    }
                }
                // Calcula cor do pixel baseado na cor do objeto mais próximo e se ele possui sombra
                glColor3f(obj_mais_prox->cor.x * max(0.1f, intensidade), obj_mais_prox->cor.y * max(0.1f, intensidade), obj_mais_prox->cor.z * max(0.1f, intensidade));
            } else {
                // Se raio não encontra nenhum objeto, o pixel é preto (background)
                glColor3f(0.0, 0.0, 0.0);
            }
            glVertex2f(x_normalizado, y_normalizado);
        }
    }
    glEnd();
    glFlush();
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    // Instancia os objetos da cena
    arr_objetos.push_back(new Esfera(Vec3(2, 1.5, -5), 1, Vec3(1.0, 0.0, 0.0)));
    arr_objetos.push_back(new Esfera(Vec3(-1.0, 0, -5), 1, Vec3(0.0, 1.0, 0.0)));
    arr_objetos.push_back(new Esfera(Vec3(0, 3, -5), 1, Vec3(0.0, 0.0, 1.0)));
    arr_objetos.push_back(new Chao(-1.5, -8.0, 8.0, -10.0, -3.0, Vec3(1.0, 1.0, 1.0)));
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(largura, altura);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Ray Casting");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard_handle);

    glutMainLoop();
    return 0;
}