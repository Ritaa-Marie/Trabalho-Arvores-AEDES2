#include <iostream>
#include <string>
#include "AVL.hpp"

using namespace std;

int main(){

    AVL arvore;
    
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


    long long numComp = arvore.getNumComparacoes();
    cout << "Número de comparações: " << numComp << endl;

    long long mem = arvore.getConsumoMemoria();
    cout << "Memória: " << mem << endl;

    int h = arvore.calcularAlturaAVL();
    cout << "Altura da árvore: " << h << endl;
    
    cout << "\nTotal de rotações: " << arvore.getNumRotacoes() << endl; 

    return 0;
}


