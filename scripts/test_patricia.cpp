#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include "Patricia/Patricia.hpp"

using namespace std;

int main() {
    Patricia arvore;

    auto inicio = std::chrono::high_resolution_clock::now();

    // estado 1: inserções
    arvore.inserirElemento("casa");
    arvore.inserirElemento("casaco");
    arvore.inserirElemento("carro");
    arvore.inserirElemento("café");         
    arvore.inserirElemento("castelo");
    arvore.inserirElemento("casulo");
    arvore.inserirElemento("carroça");
    arvore.inserirElemento("cão");
    arvore.inserirElemento("coracão");
    arvore.inserirElemento("Ação");
    arvore.inserirElemento("computacao"); 
    arvore.inserirElemento("copo");  
    arvore.gerarDOT("data/output/dot/pat1.dot");   
    
    // estado 2 : inserir
    arvore.inserirElemento("computador");
    arvore.gerarDOT("data/output/dot/pat2.dot");
    
    // estado 3: remoção
    arvore.deletarElemento("castelo");
    arvore.gerarDOT("data/output/dot/pat3.dot");

    auto fim = std::chrono::high_resolution_clock::now();
    double duracao_ms = std::chrono::duration<double, std::milli>(fim - inicio).count();

    long long numComp = arvore.getNumComparacoes();
    long long mem = arvore.getConsumoMemoria();
    int alt = arvore.alturaPatricia();
    int n = 14;

    std::cout << "\n============================================================\n";
    std::cout << "        MÉTRICAS DO EXPERIMENTO INDIVIDUAL DA PATRÍCIA        \n";
    std::cout << "==============================================================\n";
    std::cout << "  Tamanho da Entrada (N): " << std::setw(15) << n << "\n";
    std::cout << "  Comparações de Chaves : " << std::setw(15) << numComp << "\n";
    std::cout << "  Consumo de Memória    : " << std::setw(15) << mem << " bytes\n";
    std::cout << "  Altura da Árvore      : " << std::setw(15) << alt << "\n";
    std::cout << "  Tempo de Execução     : " << std::setw(12) << std::fixed << std::setprecision(3) << duracao_ms << " ms\n";
    std::cout << "==================================================\n\n";

    
    return 0;
}