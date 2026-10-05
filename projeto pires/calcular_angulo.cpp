#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Ponto3D {
    float x, y, z;
};

// Estados do Agachamento
enum EstadoAgachamento {
    EM_PE,
    AGACHANDO,
    AGACHADO_VALIDO
};

// 1. Função para calcular o ângulo 3D em GRAUS entre 3 articulações
float calcularAngulo3D(Ponto3D A, Ponto3D B, Ponto3D C) {
    // Vetores BA e BC saindo da articulação central B
    Ponto3D u = { A.x - B.x, A.y - B.y, A.z - B.z };
    Ponto3D v = { C.x - B.x, C.y - B.y, C.z - B.z };

    float produtoEscalar = u.x * v.x + u.y * v.y + u.z * v.z;
    float normaU = std::sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
    float normaV = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    if (normaU == 0.0f || normaV == 0.0f) return 0.0f;

    float cosTheta = produtoEscalar / (normaU * normaV);
    // Garante que o valor fique entre -1.0 e 1.0 para evitar erros no acos
    if (cosTheta > 1.0f) cosTheta = 1.0f;
    if (cosTheta < -1.0f) cosTheta = -1.0f;

    float anguloRadianos = std::acos(cosTheta);
    return anguloRadianos * (180.0f / M_PI);
}

// 2. Função para calcular a inclinação do tronco em relação ao eixo vertical (Y)
float calcularInclinacaoTronco(Ponto3D quadril, Ponto3D ombro) {
    // Vetor do tronco: Quadril -> Ombro
    Ponto3D tronco = { ombro.x - quadril.x, ombro.y - quadril.y, ombro.z - quadril.z };
    
    // Vetor apontando verticalmente para cima no eixo Y
    Ponto3D vertical = { 0.0f, 1.0f, 0.0f };

    float produtoEscalar = tronco.x * vertical.x + tronco.y * vertical.y + tronco.z * vertical.z;
    float normaTronco = std::sqrt(tronco.x * tronco.x + tronco.y * tronco.y + tronco.z * tronco.z);

    if (normaTronco == 0.0f) return 0.0f;

    float cosTheta = produtoEscalar / normaTronco;
    if (cosTheta > 1.0f) cosTheta = 1.0f;
    if (cosTheta < -1.0f) cosTheta = -1.0f;

    float anguloRadianos = std::acos(cosTheta);
    return anguloRadianos * (180.0f / M_PI);
}

int main() {
    std::ifstream arquivo("convertidos/juntas_quadro_3.txt"); //AQUI SE COLOCA O ARQUIVO A SER ANALISADO !!!!
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo !!" << std::endl;
        return 1;
    }

    std::vector<Ponto3D> juntas;
    Ponto3D p;

    // Leitura correta do arquivo de texto
    while (arquivo >> p.x >> p.y >> p.z) {
        juntas.push_back(p);
    }
    arquivo.close();

    if (juntas.size() < 10) {
        std::cerr << "Erro: O arquivo nao contem juntas suficientes!" << std::endl;
        return 1;
    }

    // Índices mapeados do esqueleto do modelo
    Ponto3D quadril   = juntas[0]; // Hips / Pelvis
    Ponto3D joelho    = juntas[4]; // Left Knee
    Ponto3D tornozelo = juntas[7]; // Left Ankle
    Ponto3D ombro     = juntas[3]; // Spine/Shoulder

    // Cálculos dos Ângulos
    float anguloJoelho = calcularAngulo3D(quadril, joelho, tornozelo);
    float inclinacaoTronco = calcularInclinacaoTronco(quadril, ombro);

    std::cout << "--- AVALIACAO BIOMECANICA DO EXERCICIO ---" << std::endl;
    std::cout << "Angulo do Joelho: " << anguloJoelho << " graus" << std::endl;
    std::cout << "Inclinacao do Tronco: " << inclinacaoTronco << " graus" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    // 1. CHECAGEM DE POSTURA: Inclinação do Tronco
    // Opção 1: Ajustar o limite para o movimento livre do agachamento
    // No agachamento profundo, até ~50°-55° de inclinação do tronco em relação à vertical ainda é aceitável dependendo da anatomia.
    if (inclinacaoTronco > 55.0f) {
        std::cout << "[ALERTA DE POSTURA]: Inclinacao excessiva do tronco!" << std::endl;
    }

    // Opção 2: Medir o Ângulo do Quadril (Flexão entre Ombro -> Quadril -> Joelho)
    // Em um agachamento correto, o ângulo de flexão do quadril não deve colapsar abaixo de ~60°.
    float anguloQuadril = calcularAngulo3D(ombro, quadril, joelho);
    if (anguloQuadril < 60.0f) {
        std::cout << "[ALERTA DE POSTURA]: Tronco muito dobrado sobre as coxas!" << std::endl;
    }

    // 2. CHECAGEM DAS CONDIÇÕES DO AGACHAMENTO
    if (anguloJoelho >= 165.0f) {
        std::cout << "[ESTADO]: Pessoa em pe (Perna esticada ~170-180 deg)." << std::endl;
    } else if (anguloJoelho <= 90.0f) {
        std::cout << "[ESTADO]: Agachamento valido/Profundidade atingida (<= 90 deg)." << std::endl;
    } else {
        std::cout << "[ESTADO]: Em movimento intermediario (" << anguloJoelho << " deg)." << std::endl;
        std::cout << "[LIMITACAO]: Se a pessoa subir antes de atingir <= 90 deg, nao conta como agachamento valido!" << std::endl;
    }

    return 0;
}