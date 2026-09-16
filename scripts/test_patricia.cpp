#include <iostream>
#include <vector>
#include <string>
#include "Patricia/Patricia.hpp"

using namespace std;

int main() {
    Patricia arvore;

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

    long long numComp = arvore.getNumComparacoes();
    cout << "Número de comparações: " << numComp << endl;

    long long mem = arvore.getConsumoMemoria();
    cout << "Memória: " << mem << endl;

    cout << "\nAltura: " << arvore.alturaPatricia() << endl;

    
    return 0;
}