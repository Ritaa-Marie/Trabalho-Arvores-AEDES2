#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include "AVL/AVL.hpp"
#include "VisualizacaoArvore/VisualizacaoArvore.hpp"

using namespace std;

int main(){

    AVL arvore;
    VisualizacaoArvore vis;

    auto inicio = std::chrono::high_resolution_clock::now();

    // estado 1: inserção
    arvore.inserirElemento(10);
    arvore.inserirElemento(5);
    arvore.gerarDOT("data/output/dot/AVL0.dot");
    vis.registrarPassoAVL(arvore.getRaiz(), "Inserção das chaves 10 e 5 (Balanceada)", 5,"#fde047");

    arvore.inserirElemento(8);
    vis.registrarPassoAVL(arvore.getRaiz(), "Inserção da chave 8 (Rotação para a direita)", 8,"#fde047");
    arvore.inserirElemento(16);
    arvore.inserirElemento(-8);
    arvore.inserirElemento(56);
    vis.registrarPassoAVL(arvore.getRaiz(), "Inserção das chaves 16, -8, 56 (Rotação para a esquerda)", 56,"#fde047");
    arvore.inserirElemento(6);
    vis.registrarPassoAVL(arvore.getRaiz(), "Estado após a inserção de todas as chaves", -1,"#fde047");
    arvore.gerarDOT("data/output/dot/AVL1.dot");

    // estado 2: inserção
    arvore.inserirElemento(100);
    arvore.inserirElemento(150);
    vis.registrarPassoAVL(arvore.getRaiz(), "Inserção das chaves 100 e 150 (Rotação para a esquerda)", 150,"#fde047");
    arvore.gerarDOT("data/output/dot/AVL2.dot");

    //estado 3: remoção
    arvore.deletarElemento(8);
    vis.registrarPassoAVL(arvore.getRaiz(), "Remoção do 8 (Busca pelo sucessor e rebalanceamento dos nós ancestrais)", 10,"#fde047");
    arvore.gerarDOT("data/output/dot/AVL3.dot");

    auto fim = std::chrono::high_resolution_clock::now();
    double duracao_ms = std::chrono::duration<double, std::milli>(fim - inicio).count();

    long long numComp = arvore.getNumComparacoes();
    long long mem = arvore.getConsumoMemoria();
    int alt = arvore.calcularAlturaAVL();
    int rotacoes = arvore.getNumRotacoes(); 
    int n = 9;

    vis.exportarEGerarImagem("data/output/dot/rastreio_avl.dot", "data/output/img/rastreio/rastreio_avl.png");

    std::cout << "\nRastreio da AVL gerado em rastreio_avl.png com sucesso!\n" << std::endl;

    std::cout << "\n============================================================\n";
    std::cout << "        MÉTRICAS DO EXPERIMENTO INDIVIDUAL DA AVL        \n";
    std::cout << "==============================================================\n";
    std::cout << "  Tamanho da Entrada (N): " << std::setw(15) << n << "\n";
    std::cout << "  Comparações de Chaves : " << std::setw(15) << numComp << "\n";
    std::cout << "  Número de rotações    : " << std::setw(15) << rotacoes << "\n";
    std::cout << "  Consumo de Memória    : " << std::setw(15) << mem << " bytes\n";
    std::cout << "  Altura da Árvore      : " << std::setw(15) << alt << "\n";
    std::cout << "  Tempo de Execução     : " << std::setw(12) << std::fixed << std::setprecision(3) << duracao_ms << " ms\n";
    std::cout << "==================================================\n\n";

    return 0;
}



