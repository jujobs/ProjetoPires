#define CGLTF_IMPLEMENTATION
#include "d:\jifim\projeto pires\cgltf-master\cgltf.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    cgltf_options options = {};
    cgltf_data* data = NULL;

    // 1. Carrega e faz o parse do arquivo .glb
    cgltf_result result = cgltf_parse_file(&options, "person_0.glb", &data);
    if (result != cgltf_result_success) {
        printf("Erro ao carregar o arquivo GLB!\n");
        return 1;
    }

    // Carrega os buffers binários associados ao arquivo GLB
    cgltf_load_buffers(&options, data, "person_0.glb");

    // 2. Cria/Abre o arquivo .txt de saída
    FILE *arquivo_txt = fopen("juntas_extraidas.txt", "w");
    if (!arquivo_txt) {
        printf("Erro ao criar o arquivo TXT de saida!\n");
        cgltf_free(data);
        return 1;
    }

    printf("Extraindo posicoes das juntas do arquivo GLB...\n");

    // 3. Percorre os nós da cena e grava apenas as coordenadas X Y Z no arquivo
    for (cgltf_size i = 0; i < data->nodes_count; ++i) {
        cgltf_node* node = &data->nodes[i];

        float x = node->translation[0];
        float y = node->translation[1];
        float z = node->translation[2];

        // Se o nó utilizar matriz de transformação 4x4 em vez de vetor de translação
        if (node->has_matrix) {
            x = node->matrix[12];
            y = node->matrix[13];
            z = node->matrix[14];
        }

        // Escreve apenas os números flutuantes X Y Z por linha para facilitar o parsing futuro
        fprintf(arquivo_txt, "%f %f %f\n", x, y, z);
    }

    // 4. Limpeza de recursos e finalização
    fclose(arquivo_txt);
    cgltf_free(data);

    printf("Concluido! As posicoes das juntas foram salvas em 'juntas_extraidas.txt'.\n");
    return 0;
}