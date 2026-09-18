#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include "AVL/AVL.hpp"

using namespace std;

int main(){

    AVL arvore;

    auto inicio = std::chrono::high_resolution_clock::now();

    // estado 1: inserção
    arvore.inserirElemento(10);
    arvore.inserirElemento(5);
    arvore.gerarDOT("data/output/dot/AVL0.dot");

    arvore.inserirElemento(8);
    arvore.inserirElemento(16);
    arvore.inserirElemento(-8);
    arvore.inserirElemento(56);
    arvore.inserirElemento(6);
    arvore.gerarDOT("data/output/dot/AVL1.dot");

    // estado 1: inserção
    arvore.inserirElemento(100);
    arvore.inserirElemento(150);
    arvore.gerarDOT("data/output/dot/AVL2.dot");

    //estado 3: remoção
    arvore.deletarElemento(8);
    arvore.gerarDOT("data/output/dot/AVL3.dot");

    auto fim = std::chrono::high_resolution_clock::now();
    double duracao_ms = std::chrono::duration<double, std::milli>(fim - inicio).count();

    long long numComp = arvore.getNumComparacoes();
    long long mem = arvore.getConsumoMemoria();
    int alt = arvore.calcularAlturaAVL();
    int rotacoes = arvore.getNumRotacoes(); 
    int n = 9;

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


