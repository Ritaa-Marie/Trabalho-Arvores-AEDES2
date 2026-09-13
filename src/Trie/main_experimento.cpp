#include "Trie.hpp"

using namespace std;

int main() {
    Trie trie;

    trie.inserirElemento("casa");
    trie.inserirElemento("casaco");
    trie.inserirElemento("carro");
    trie.inserirElemento("café");      
    trie.inserirElemento("R$100");      
    trie.inserirElemento("casa");
    trie.inserirElemento("casulo");
    trie.inserirElemento("carroça");
    trie.inserirElemento("cão");
    trie.inserirElemento("Ação");
    trie.inserirElemento("casa");       

    trie.exibirTrieInOrdem();
    
    string busca1 = "carro";
    string busca2 = "caminhão";

    cout << "Buscando '" << busca1 << "': " << (trie.buscarElemento(busca1) ? "Encontrado" : "Não encontrado") << endl;
    cout << "Buscando '" << busca2 << "': " << (trie.buscarElemento(busca2) ? "Encontrado" : "Não encontrado") << endl;

    cout << "\naltura: " << trie.alturaTrie() << endl;
    
    trie.deletarElemento("casa");   
    trie.deletarElemento("teste");  
    
    cout << "casa: " << trie.buscarElemento("casa") << endl;
    cout << "CASA: " << trie.buscarElemento("CASA") << endl;
    cout << "cas: " << trie.buscarElemento("cas") << endl;
    cout << "cao: " << trie.buscarElemento("cao") << endl;
    cout << "acao: " << trie.buscarElemento("acao") << endl;
    cout << "inexistente: " << trie.buscarElemento("banana") << endl;

    cout << "\napos remocao:" << endl;
    trie.exibirTrieInOrdem();
    
     cout << "casa existe: " << trie.buscarElemento("casa") << endl;
     cout << "casulo existe: " << trie.buscarElemento("casulo") << endl;
     cout << "casaco ainda existe: " << trie.buscarElemento("casaco") << endl;

     trie.gerarDOT("data/output/img/dot/trie1.dot");

    return 0;
}

    
    

