#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>
#include <chrono>
#include "KdTree/KdTree.hpp"
#include "VisualizacaoArvore/VisualizacaoArvore.hpp"

using namespace std;

int main() {
    KdTree arvore(2);
    VisualizacaoArvore vis;

    auto inicio = std::chrono::high_resolution_clock::now();

    // estado 1: inserções 
    arvore.inserirElemento({3, 6});   
    arvore.inserirElemento({17, 15}); 
    arvore.inserirElemento({13, 15}); 
    arvore.inserirElemento({6, 12});  
    arvore.inserirElemento({9, 1});   
    arvore.inserirElemento({2, 7});   
    arvore.inserirElemento({10, 19}); 
    arvore.gerarDOT("data/output/dot/kd1.dot");
    vis.registrarPassoKdTree(arvore.getRaiz(), "Inserção dos pontos 2D (Alternância de eixos X/Y)", nullptr,"#fde047");

    // estado 2 : busca
    arvore.inserirElemento({7, 13});
    arvore.gerarDOT("data/output/dot/kd2.dot");
    vis.registrarPassoKdTree(arvore.getRaiz(), "Inserção do ponto (7, 13)", nullptr, "#fde047");

    // estado 3: remoção
    arvore.deletarElemento({17, 15});
    arvore.gerarDOT("data/output/dot/kd3.dot");
    vis.registrarPassoKdTree(arvore.getRaiz(), "Remoção do ponto (17, 15) por substituição de mínimo no eixo", nullptr, "#ef4444");
    
    auto fim = std::chrono::high_resolution_clock::now();
    double duracao_ms = std::chrono::duration<double, std::milli>(fim - inicio).count();

    long long numComp = arvore.getNumComparacoes();
    long long mem = arvore.getConsumoMemoria();
    int alt = arvore.alturaKdTree();
    int n = 8;

    vis.exportarEGerarImagem("data/output/dot/rastreio_kd.dot", "data/output/img/rastreio/rastreio_kd.png");
    std::cout << "\nRastreio da KdTree gerado em rastreio_kd.png com sucesso!\n" << std::endl;

    std::cout << "\n============================================================\n";
    std::cout << "        MÉTRICAS DO EXPERIMENTO INDIVIDUAL DA KDTREE        \n";
    std::cout << "==============================================================\n";
    std::cout << "  Tamanho da Entrada (N): " << std::setw(15) << n << "\n";
    std::cout << "  Comparações de Chaves : " << std::setw(15) << numComp << "\n";
    std::cout << "  Consumo de Memória    : " << std::setw(15) << mem << " bytes\n";
    std::cout << "  Altura da Árvore      : " << std::setw(15) << alt << "\n";
    std::cout << "  Tempo de Execução     : " << std::setw(12) << std::fixed << std::setprecision(3) << duracao_ms << " ms\n";
    std::cout << "==================================================\n\n";

    return 0;
}