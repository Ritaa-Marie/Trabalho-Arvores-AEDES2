#include <iostream>
#include <string>
#include "BST.hpp"

using namespace std;

int main(){

    BST arvore;
    
    arvore.inserirElemento(10);
    //arvore.gerarDOT("data/output/img/dot/arvore1.dot");
    arvore.inserirElemento(5);
    //arvore.gerarDOT("data/output/img/dot/arvore2.dot");
    arvore.inserirElemento(8);
    //arvore.gerarDOT("data/output/img/dot/arvore3.dot");
    arvore.inserirElemento(16);
    arvore.inserirElemento(-8);
    arvore.inserirElemento(56);
    arvore.inserirElemento(10);
    arvore.inserirElemento(6);
    //arvore.gerarDOT("data/output/img/dot/arvore4.dot");
    arvore.exibirBSTInOrdem();
    arvore.deletarElemento(45);
    arvore.exibirBSTInOrdem();
    //arvore.gerarDOT("data/output/img/dot/arvore5.dot");
    arvore.deletarElemento(5);
    arvore.gerarDOT("data/output/img/dot/arvoreBST.dot");
    arvore.exibirBSTInOrdem();

    int numComp = arvore.getNumComparacoes();
    cout << "Número de comparações: " << numComp << endl;

    bool num5 = arvore.buscarElemento(5);
    cout << "Número 5 está na árvore: " << num5 << endl;

    bool num10 = arvore.buscarElemento(10);
    cout << "Número 10 está na árvore: " << num10 << endl;

    return 0;
}