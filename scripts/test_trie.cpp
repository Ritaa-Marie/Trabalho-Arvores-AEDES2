#include "Trie/Trie.hpp"
#include <iomanip>
#include <chrono>

using namespace std;

int main() {
    Trie trie;

    auto inicio = std::chrono::high_resolution_clock::now();

    // estado 1: inserções
    trie.inserirElemento("casa");
    trie.inserirElemento("casaco");
    trie.inserirElemento("carro");
    trie.inserirElemento("café");         
    trie.inserirElemento("castelo");
    trie.inserirElemento("casulo");
    trie.inserirElemento("carroça");
    trie.inserirElemento("cão");
    trie.inserirElemento("coracão");
    trie.inserirElemento("Ação");
    trie.inserirElemento("computacao"); 
    trie.inserirElemento("copo");  
    trie.gerarDOT("data/output/dot/trie1.dot");   
    
    // estado 2 : inserir
    trie.inserirElemento("computador");
    trie.gerarDOT("data/output/dot/trie2.dot");
    
    // estado 3: remoção
    trie.deletarElemento("castelo");
    trie.gerarDOT("data/output/dot/trie3.dot");

    auto fim = std::chrono::high_resolution_clock::now();
    double duracao_ms = std::chrono::duration<double, std::milli>(fim - inicio).count();

    long long numComp = trie.getNumComparacoes();
    long long mem = trie.getConsumoMemoria();
    int alt = trie.alturaTrie();
    int n = 13;

    std::cout << "\n============================================================\n";
    std::cout << "        MÉTRICAS DO EXPERIMENTO INDIVIDUAL DA TRIE        \n";
    std::cout << "==============================================================\n";
    std::cout << "  Tamanho da Entrada (N): " << std::setw(15) << n << "\n";
    std::cout << "  Comparações de Chaves : " << std::setw(15) << numComp << "\n";
    std::cout << "  Consumo de Memória    : " << std::setw(15) << mem << " bytes\n";
    std::cout << "  Altura da Árvore      : " << std::setw(15) << alt << "\n";
    std::cout << "  Tempo de Execução     : " << std::setw(12) << std::fixed << std::setprecision(3) << duracao_ms << " ms\n";
    std::cout << "==================================================\n\n";

    return 0;
}

    
    

