#define CGLTF_IMPLEMENTATION
#include "d:\jifim\projeto pires\cgltf-master\cgltf.h"
#include <cstdio>
#include <cstdlib>
#include <string>

// Função reutilizável para converter qualquer modelo GLB para TXT
bool converterGlbParaTxt(const std::string& caminhoGlb, const std::string& caminhoTxt) {
    cgltf_options options = {};
    cgltf_data* data = NULL;

    // 1. Carrega o arquivo GLB específico passado no parâmetro
    cgltf_result result = cgltf_parse_file(&options, caminhoGlb.c_str(), &data);
    if (result != cgltf_result_success) {
        std::printf("Erro ao carregar o arquivo: %s\n", caminhoGlb.c_str());
        return false;
    }

    cgltf_load_buffers(&options, data, caminhoGlb.c_str());

    // 2. Abre o arquivo TXT de saída específico
    FILE *arquivo_txt = std::fopen(caminhoTxt.c_str(), "w");
    if (!arquivo_txt) {
        std::printf("Erro ao criar o arquivo TXT: %s\n", caminhoTxt.c_str());
        cgltf_free(data);
        return false;
    }

    // 3. Extrai as posições X, Y, Z de todas as juntas
    for (cgltf_size i = 0; i < data->nodes_count; ++i) {
        cgltf_node* node = &data->nodes[i];

        float x = node->translation[0];
        float y = node->translation[1];
        float z = node->translation[2];

        if (node->has_matrix) {
            x = node->matrix[12];
            y = node->matrix[13];
            z = node->matrix[14];
        }

        std::fprintf(arquivo_txt, "%f %f %f\n", x, y, z);
    }

    std::fclose(arquivo_txt);
    cgltf_free(data);
    return true;
}

int main() {
    // Exemplo: convertendo 3 modelos GLB diferentes de quadros sequenciais
    // quando precisar converter mais quadros, basta copiar e colar essa linha com o nome do arquivo certo. converte vários quadros de uma só vez.
    converterGlbParaTxt("modelos_pessoas/person_0.glb", "convertidos/juntas_quadro_0.txt");
    converterGlbParaTxt("modelos_pessoas/person_1.glb", "convertidos/juntas_quadro_1.txt");
    converterGlbParaTxt("modelos_pessoas/person_2.glb", "convertidos/juntas_quadro_2.txt");
    converterGlbParaTxt("modelos_pessoas/person_3.glb", "convertidos/juntas_quadro_3.txt");


    std::printf("Todos os arquivos GLB foram convertidos com sucesso!\n");
    return 0;
}