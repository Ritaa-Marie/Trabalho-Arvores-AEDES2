#include <iostream>
#include <vector>
#include <cassert>
#include "KdTree/KdTree.hpp"

using namespace std;

int main() {
    KdTree arvore(2);

    // estado 1: inserções 
    arvore.inserirElemento({3, 6});   
    arvore.inserirElemento({17, 15}); 
    arvore.inserirElemento({13, 15}); 
    arvore.inserirElemento({6, 12});  
    arvore.inserirElemento({9, 1});   
    arvore.inserirElemento({2, 7});   
    arvore.inserirElemento({10, 19}); 
    arvore.gerarDOT("data/output/dot/kd1.dot");

    // estado 2 : busca
    arvore.inserirElemento({7, 13});
    arvore.gerarDOT("data/output/dot/kd2.dot");

    // estado 3: remoção
    arvore.deletarElemento({17, 15});
    arvore.gerarDOT("data/output/dot/kd3.dot");
    
    long long numComp = arvore.getNumComparacoes();
    cout << "Número de comparações: " << numComp << endl;

    long long mem = arvore.getConsumoMemoria();
    cout << "Memória: " << mem << endl;

    int h = arvore.alturaKdTree();
    cout << "Altura da árvore: " << h << endl;

    return 0;
}