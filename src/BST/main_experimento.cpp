#include <iostream>
#include <string>
#include "BST.hpp"

using namespace std;

int main(){

    BST arvore;
    
    // estado 1: inserção
    arvore.inserirElemento(10);
    arvore.inserirElemento(5);
    arvore.inserirElemento(8);
    arvore.inserirElemento(16);
    arvore.inserirElemento(-8);
    arvore.inserirElemento(56);
    arvore.inserirElemento(6);
    arvore.gerarDOT("data/output/dot/BST1.dot");

    // estado 1: inserção
    arvore.inserirElemento(60);
    arvore.gerarDOT("data/output/dot/BST2.dot");

    //estado 3: remoção
    arvore.deletarElemento(5);
    arvore.gerarDOT("data/output/dot/BST3.dot");


    long long numComp = arvore.getNumComparacoes();
    cout << "Número de comparações: " << numComp << endl;

    long long mem = arvore.getConsumoMemoria();
    cout << "Memória: " << mem << endl;

    int h = arvore.calcularAlturaBST();
    cout << "Altura da árvore: " << h << endl;

    return 0;
}