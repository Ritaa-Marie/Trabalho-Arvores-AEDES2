#include <iostream>
#include <string>
#include "Treap.hpp"

using namespace std;

int main(){

    Treap arvore;
    
    /*arvore.inserirElemento(10);
    arvore.gerarDOT("data/output/img/dot/arvore1.dot");
    arvore.inserirElemento(5);
    arvore.gerarDOT("data/output/img/dot/arvore2.dot");
    arvore.inserirElemento(8);
    arvore.gerarDOT("data/output/img/dot/arvore3.dot");
    arvore.inserirElemento(16);
    arvore.inserirElemento(-8);
    arvore.inserirElemento(56);
    arvore.inserirElemento(10);
    arvore.inserirElemento(6);
    arvore.gerarDOT("data/output/img/dot/arvore4.dot");
    arvore.exibirAVLInOrdem();
    arvore.deletarElemento(45);
    arvore.exibirAVLInOrdem();
    arvore.gerarDOT("data/output/img/dot/arvore5.dot");
    arvore.deletarElemento(5);
    arvore.gerarDOT("data/output/img/dot/arvore6.dot");
    arvore.exibirAVLInOrdem();

    int numComp = arvore.getNumComparacoes();
    cout << "Número de comparações: " << numComp << endl;

    int numRot = arvore.getNumRotacoes();
    cout << "Número de rotações: " << numRot << endl;

    bool num5 = arvore.buscarElemento(5);
    cout << "Número 5 está na árvore: " << num5 << endl;

    bool num10 = arvore.buscarElemento(10);
    cout << "Número 10 está na árvore: " << num10 << endl;*/

    arvore.inserirElemento(50); 
    arvore.inserirElemento(25); 
    arvore.inserirElemento(10); 
    arvore.inserirElemento(5);  
    arvore.inserirElemento(1);  
    arvore.inserirElemento(40); 
    arvore.inserirElemento(30); 
    arvore.exibirTreapInOrdem();
    arvore.deletarElemento(1); 
    arvore.deletarElemento(30);
    arvore.deletarElemento(50); 
    arvore.inserirElemento(8);
    arvore.deletarElemento(40);
    
    cout << "\nTotal de rotações: " << arvore.getNumRotacoes() << endl; 
    cout << "\nAltura da árvore: " << arvore.alturaTreap() << endl; 
    cout << "\nConsumo de memória: " << arvore.getConsumoMemoria() << endl; 
    arvore.gerarDOT("data/output/img/dot/treap.dot");

    return 0;
}


