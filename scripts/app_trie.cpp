#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>
#include "Trie/Trie.hpp"

using namespace std;

// Teste de Aplicação: Trie  como Autocompletar
void testarTrieAutocompletar(Trie& trie, const string& prefixo) {
    filesystem::create_directories("data/output");
    
    string caminhoSaida = "data/output/aplicacoes/trie_aplicacao_autocompletar.dat";
    ofstream arqSaida(caminhoSaida);

    auto logPrint = [&](const string& msg) {
        cout << msg;
        if (arqSaida.is_open()) {
            arqSaida << msg;
        }
    };

    logPrint("\n");
    logPrint("=================================================\n");
    logPrint("  APLICAÇÃO: Trie Autocompletar\n");
    logPrint("=================================================\n");
    logPrint("Buscando sugestões para o prefixo: '" + prefixo + "'...\n\n");

    auto inicio = chrono::high_resolution_clock::now();

    vector<string> sugestoes = trie.autocompletar(prefixo); 

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::microseconds>(fim - inicio).count();

    logPrint("-> Sugestões encontradas (" + to_string(sugestoes.size()) + "):\n");
    int limiteExibicao = 10; // Exibe no máximo 10 palavras no log para não poluir
    for (size_t i = 0; i < sugestoes.size() && i < static_cast<size_t>(limiteExibicao); i++) {
        logPrint("   - " + sugestoes[i] + "\n");
    }

    if (sugestoes.size() > static_cast<size_t>(limiteExibicao)) {
        logPrint("   ... e mais " + to_string(sugestoes.size() - limiteExibicao) + " palavras.\n");
    }

    logPrint("\n-> Tempo de execução da busca por prefixo: " + to_string(duracao) + " us (" + to_string(static_cast<double>(duracao) / 1000.0) + " ms)\n");
    logPrint("=================================================\n\n");

    if (arqSaida.is_open()) {
        arqSaida.close();
        cout << "-> Saída gravada com sucesso em '" << caminhoSaida << "'\n";
    }
}

int main() {
    string caminhoDataset = "data/datasets/string_aleatorio.dat";
    ifstream arquivo(caminhoDataset);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << caminhoDataset << endl;
        cerr << "Verifique se o arquivo existe em 'data/datasets/'." << endl;
        return 1;
    }

    cout << "Lendo dataset: " << caminhoDataset << "...\n";
    vector<string> palavras;
    string palavra;

    while (arquivo >> palavra) {
        palavras.push_back(palavra);
    }
    arquivo.close();

    cout << "Total de " << palavras.size() << " palavras carregadas.\n";

    Trie trie;
    for (const string& p : palavras) {
        trie.inserirElemento(p); 
    }

    cout << "Trie populada com sucesso!\n\n";

    string prefixoTeste = "algoritmo"; 
    testarTrieAutocompletar(trie, prefixoTeste);

    return 0;
}