#include <iostream>
#include <fstream>
#include <vector>

struct Ponto3D {
    float x, y, z;
};

int main() {
    std::ifstream arq("convertidos/juntas_quadro_0.txt");
    std::vector<Ponto3D> juntas;
    
    Ponto3D p;
    // O primeiro ponto lido vai para juntas[0], o segundo para juntas[1], etc.
    while (arq >> p.x >> p.y >> p.z) {
        juntas.push_back(p);
    }
    arq.close();

    // Agora você acessa cada junta pelo seu índice fixo!
    Ponto3D quadril  = juntas[0]; // Junta 0
    Ponto3D joelho   = juntas[4]; // Supondo junta 4 para o joelho
    Ponto3D tornozelo = juntas[7]; // Supondo junta 7 para o tornozelo

    std::cout << "Total de juntas lidas: " << juntas.size() << std::endl;
    return 0;
}