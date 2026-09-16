#include <iostream>
#include <string>
#include "Treap/Treap.hpp"

using namespace std;

int main(){

    Treap arvore;

    // estado 1: inserções 
    arvore.inserirElemento(10);
    arvore.inserirElemento(5);
    arvore.inserirElemento(8);
    arvore.inserirElemento(16);
    arvore.inserirElemento(-8);
    arvore.inserirElemento(56);
    arvore.inserirElemento(6);
    arvore.gerarDOT("data/output/dot/treap1.dot");

    // estado 2 : busca
    arvore.inserirElemento(60);
    arvore.gerarDOT("data/output/dot/treap2.dot");

    // estado 3: remoção
    arvore.deletarElemento(8);
    arvore.gerarDOT("data/output/dot/treap3.dot");
    
    long long numComp = arvore.getNumComparacoes();
    cout << "Número de comparações: " << numComp << endl;

    long long mem = arvore.getConsumoMemoria();
    cout << "Memória: " << mem << endl;

    int h = arvore.alturaTreap();
    cout << "Altura da árvore: " << h << endl;
    
    cout << "\nTotal de rotações: " << arvore.getNumRotacoes() << endl; 

    return 0;
}


