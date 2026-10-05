#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Ponto3D {
    float x, y, z;
};

// 1. Função para calcular o Ângulo de Flexão do Joelho (Quadril -> Joelho -> Tornozelo)
float calcularAnguloJoelho(Ponto3D quadril, Ponto3D joelho, Ponto3D tornozelo) {
    Ponto3D u = { quadril.x - joelho.x,  quadril.y - joelho.y,  quadril.z - joelho.z };
    Ponto3D v = { tornozelo.x - joelho.x, tornozelo.y - joelho.y, tornozelo.z - joelho.z };

    float produtoEscalar = u.x * v.x + u.y * v.y + u.z * v.z;
    float normaU = std::sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
    float normaV = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    if (normaU == 0.0f || normaV == 0.0f) return 0.0f;

    float cosTheta = produtoEscalar / (normaU * normaV);
    if (cosTheta > 1.0f) cosTheta = 1.0f;
    if (cosTheta < -1.0f) cosTheta = -1.0f;

    return std::acos(cosTheta) * (180.0f / M_PI);
}

// 2. Função Infalível para medir a Inclinação do Tronco em Graus
// Retorna 0° quando a pessoa está 100% ereta e aproxima-se de 90° se ela deitar o tronco para a frente
float calcularInclinacaoTronco(Ponto3D quadril, Ponto3D ombro) {
    // Vetor do tronco: Quadril -> Ombro
    float dy = ombro.y - quadril.y; // Variação de Altura
    float dz = ombro.z - quadril.z; // Variação de Profundidade
    float dx = ombro.x - quadril.x; // Variação Lateral

    float projecaoHorizontal = std::sqrt(dz * dz + dx * dx);

    // std::atan2 calcula o ângulo exato do vetor sem problemas de sinal
    // std::abs garante valor positivo independente do lado que o paciente está virado
    float anguloRadianos = std::atan2(projecaoHorizontal, std::abs(dy));
    
    return anguloRadianos * (180.0f / M_PI);
}

int main() {
    std::ifstream arquivo("convertidos/juntas_quadro_0.txt");
    if (!arquivo.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo juntas_extraidas.txt!" << std::endl;
        return 1;
    }

    std::vector<Ponto3D> juntas;
    Ponto3D p;
    while (arquivo >> p.x >> p.y >> p.z) {
        juntas.push_back(p);
    }
    arquivo.close();

    if (juntas.size() < 10) {
        std::cerr << "Erro: Poucas juntas lidas no arquivo." << std::endl;
        return 1;
    }

    // Mapeamento dos pontos do esqueleto
    Ponto3D quadril   = juntas[0];
    Ponto3D joelho    = juntas[4];
    Ponto3D tornozelo = juntas[7];
    Ponto3D ombro     = juntas[3];

    // Cálculos
    float anguloJoelho = calcularAnguloJoelho(quadril, joelho, tornozelo);
    float inclinacaoTronco = calcularInclinacaoTronco(quadril, ombro);

    std::cout << "\n==========================================" << std::endl;
    std::cout << "Angulo de Flexao do Joelho: " << anguloJoelho << " graus" << std::endl;
    std::cout << "Inclinacao do Tronco:        " << inclinacaoTronco << " graus" << std::endl;
    std::cout << "==========================================" << std::endl;

    // 1. AVALIAÇÃO DO AGACHAMENTO (JOELHO)
    if (anguloJoelho <= 95.0f) {
        std::cout << "[ESTADO]: Agachamento Valido! Profundidade de <= 90 deg atingida." << std::endl;
    } else if (anguloJoelho >= 165.0f) {
        std::cout << "[ESTADO]: Pessoa em pe." << std::endl;
    } else {
        std::cout << "[ESTADO]: Movimento intermediario (Se subir antes de 90 deg, nao conta)." << std::endl;
    }

    // 2. AVALIAÇÃO DE POSTURA (INCLINAÇÃO)
    // 0° a 40° = Postura normal e equilibrada do agachamento.
    // Acima de 60° = Tronco excessivamente dobrado para a frente (Erro grave de coluna).
    if (inclinacaoTronco > 60.0f) {
        std::cout << "[ALERTA DE POSTURA]: Inclinacao excessiva do tronco! (> 60 graus)." << std::endl;
    } else {
        std::cout << "[POSTURA OK]: Tronco alinhado e seguro." << std::endl;
    }

    return 0;
}