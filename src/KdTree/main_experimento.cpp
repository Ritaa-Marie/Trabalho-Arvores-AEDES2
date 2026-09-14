#include <iostream>
#include <vector>
#include <cassert>
#include "KdTree.hpp"

using namespace std;

int main() {
    KdTree tree2D(2);
    tree2D.inserirElemento({3.0, 4.0, 5.0}); // falha

    tree2D.inserirElemento({3.0, 6.0});
    tree2D.inserirElemento({17.0, 15.0});
    tree2D.inserirElemento({13.0, 15.0});
    tree2D.inserirElemento({6.0, 12.0});
    tree2D.inserirElemento({9.0, 1.0});
    tree2D.inserirElemento({2.0, 7.0});
    tree2D.inserirElemento({10.0, 19.0});
    tree2D.inserirElemento({3.0, 6.0}); // repete
    tree2D.exibirKdTreeInOrdem();

    cout << "-> Buscando ponto {6.0, 12.0}: " << tree2D.buscarElemento({6.0, 12.0}) << endl;
    cout << "-> Buscando ponto {10.0, 10.0}: " << tree2D.buscarElemento({10.0, 10.0})<< endl;

    tree2D.buscarElementoMaisProximo({9.0, 2.0});
    tree2D.buscarElementoMaisProximo({9.0, 2.0, 1.0}); // erro de dimensão
    tree2D.buscarPontosNoDelimitador({0.0, 0.0}, {10.0, 10.0});
    tree2D.buscarPontosNoDelimitador({100.0, 100.0}, {200.0, 200.0});

    tree2D.deletarElemento({2.0, 7.0});
    tree2D.deletarElemento({99.0, 99.0});
    tree2D.deletarElemento({3.0, 6.0});
    tree2D.exibirKdTreeInOrdem();
    cout << "memória: " << tree2D.getConsumoMemoria() << endl;

    KdTree treePequena(2);
    
    treePequena.buscarElementoMaisProximo({1.0, 1.0});
    treePequena.exibirKdTreeInOrdem();
    treePequena.inserirElemento({5.0, 5.0});
    treePequena.buscarElementoMaisProximo({10.0, 10.0}); 
    treePequena.deletarElemento({5.0, 5.0});

    cout << "-> Exibindo árvore após esvaziamento completo:\n";
    cout << "memória: " << treePequena.getConsumoMemoria() << endl;
    treePequena.exibirKdTreeInOrdem();

    KdTree tree3D(3);
    tree3D.inserirElemento({1.0, 2.0, 3.0});
    tree3D.inserirElemento({5.0, 1.0, 2.0});
    tree3D.inserirElemento({0.0, 8.0, 4.0});

    tree3D.exibirKdTreeInOrdem();
    tree3D.buscarElementoMaisProximo({1.1, 2.1, 3.1});
    cout << "memória: " << tree3D.getConsumoMemoria() << endl;
    cout << "altura: " << tree3D.alturaKdTree() << endl;
    cout << "comparações: " << tree3D.getNumComparacoes() << endl;


    //tree2D.gerarDOT("kdtree.dot");
    return 0;
}