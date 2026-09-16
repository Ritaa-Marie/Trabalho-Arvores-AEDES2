#include "Trie/Trie.hpp"

using namespace std;

int main() {
    Trie trie;

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

    long long numComp = trie.getNumComparacoes();
    cout << "Número de comparações: " << numComp << endl;

    long long mem = trie.getConsumoMemoria();
    cout << "Memória: " << mem << endl;

    cout << "\nAltura: " << trie.alturaTrie() << endl;

    return 0;
}

    
    

