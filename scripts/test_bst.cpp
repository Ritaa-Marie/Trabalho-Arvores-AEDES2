#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include "BST/BST.hpp"
#include "VisualizacaoArvore/VisualizacaoArvore.hpp"

using namespace std;

int main(){

    BST arvore;
    VisualizacaoArvore vis;

    auto inicio = std::chrono::high_resolution_clock::now();
    
    // estado 1: inserção
    arvore.inserirElemento(10);
    arvore.inserirElemento(5);
    arvore.inserirElemento(8);
    arvore.inserirElemento(16);
    arvore.inserirElemento(-8);
    arvore.inserirElemento(56);
    arvore.inserirElemento(6);
    vis.registrarPassoBST(arvore.getRaiz(), "Inserção das chaves 10, 5, 8, 16, -8, 56, 6", 10,"#fde047");
    arvore.gerarDOT("data/output/dot/BST1.dot");

    // estado 1: inserção
    arvore.inserirElemento(60);
    vis.registrarPassoBST(arvore.getRaiz(), "Inserção da chave 60", 60,"#fde047");
    arvore.gerarDOT("data/output/dot/BST2.dot");

    //estado 3: remoção
    arvore.deletarElemento(5);
    vis.registrarPassoBST(arvore.getRaiz(), "Remoção do 5 (Busca pelo sucessor)", 6,"#fde047");
    arvore.gerarDOT("data/output/dot/BST3.dot");

    auto fim = std::chrono::high_resolution_clock::now();
    double duracao_ms = std::chrono::duration<double, std::milli>(fim - inicio).count();

    long long numComp = arvore.getNumComparacoes();
    long long mem = arvore.getConsumoMemoria();
    int alt = arvore.calcularAlturaBST();
    int n = 8;

    vis.exportarEGerarImagem("data/output/dot/rastreio_bst.dot", "data/output/img/rastreio/rastreio_bst.png");
    std::cout << "\nRastreio da BST gerado em rastreio_bst.png com sucesso!\n" << std::endl;

    std::cout << "\n============================================================\n";
    std::cout << "        MÉTRICAS DO EXPERIMENTO INDIVIDUAL DA BST        \n";
    std::cout << "==============================================================\n";
    std::cout << "  Tamanho da Entrada (N): " << std::setw(15) << n << "\n";
    std::cout << "  Comparações de Chaves : " << std::setw(15) << numComp << "\n";
    std::cout << "  Consumo de Memória    : " << std::setw(15) << mem << " bytes\n";
    std::cout << "  Altura da Árvore      : " << std::setw(15) << alt << "\n";
    std::cout << "  Tempo de Execução     : " << std::setw(12) << std::fixed << std::setprecision(3) << duracao_ms << " ms\n";
    std::cout << "==================================================\n\n";

    return 0;
}