#include <iostream>
#include <vector>
#include <string>
#include "Patricia.hpp"

using namespace std;

int main() {
    Patricia arvore;

    vector<string> palavras = {
         , "casamento", "",
        , "", "compilador", "aviao",
        "Computação"
    };

    for (const auto& p : palavras) {
        arvore.inserirElemento(p);
    }

    cout << "memória: " << arvore.getConsumoMemoria() << endl;
    arvore.exibirPatriciaInOrdem();
    cout << "Altura atual: " << arvore.alturaPatricia() << endl;

    vector<string> buscas = {"casa", "castelo", "computacao", "aviao", "algoritmo"};
    for (const auto& p : buscas) {
        bool encontrado = arvore.buscarElemento(p);
        cout << "Busca: '" << p << "': " << encontrado << endl;
    }

    arvore.deletarElemento("casa");
    arvore.deletarElemento("carro");
    arvore.deletarElemento("inexistente");
    cout << "memória: " << arvore.getConsumoMemoria() << endl;
    arvore.exibirPatriciaInOrdem();
    cout << "Nova altura: " << arvore.alturaPatricia() << endl;

    cout << "memória: " << arvore.getConsumoMemoria() << endl;
    
    arvore.gerarDOT("data/output/img/dot/patricia.dot");
    
    return 0;
}