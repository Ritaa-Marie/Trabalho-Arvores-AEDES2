#include <iostream>
#include <vector>
#include <chrono>
#include <filesystem>
#include "Treap.hpp"

using namespace std;

void testarTreapFilaPrioridade(Treap& treap, int numInsercoes) {
    filesystem::create_directories("data/output/aplicacoes");
    
    string caminhoSaida = "data/output/aplicacoes/treap_aplicacao_fila_prioridade.dat";
    ofstream arqSaida(caminhoSaida);

    if (!arqSaida.is_open()) {
        cerr << "Erro ao criar o arquivo de saida: " << caminhoSaida << endl;
    }

    auto logPrint = [&](const string& msg) {
        cout << msg;
        if (arqSaida.is_open()) {
            arqSaida << msg;
        }
    };

    logPrint("=================================================\n");
    logPrint("  APLICAÇÃO: Treap (Fila de Prioridade)\n");
    logPrint("=================================================\n\n");

    treap.setNumComparacoes(0);

    auto inicio = chrono::high_resolution_clock::now();

    int tarefaTopo;
    uint64_t prioridadeTopo;

    bool sucesso = treap.obterMaiorPrioridade(tarefaTopo, prioridadeTopo);

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::microseconds>(fim - inicio).count();

    long long totalComparacoes = treap.getNumComparacoes();

    if (sucesso) {
        logPrint("-> Elemento no Topo da Fila (Maior Prioridade):\n");
        logPrint("   - Chave/Tarefa: " + to_string(tarefaTopo) + "\n");
        logPrint("   - Prioridade:   " + to_string(prioridadeTopo) + "\n\n");
    } else {
        logPrint("-> A fila de prioridades está vazia.\n\n");
    }

    logPrint("-> Total de tarefas na Treap: " + to_string(numInsercoes) + "\n");
    logPrint("-> Tempo para consultar topo da fila: " + to_string(duracao) + " us (" + to_string(static_cast<double>(duracao) / 1000.0) + " ms)\n");
    logPrint("-> Total de Comparações: " + to_string(totalComparacoes) + "\n");
    logPrint("=================================================\n\n");

    if (arqSaida.is_open()) {
        arqSaida.close();
        cout << "-> Saída gravada com sucesso em '" << caminhoSaida << "'\n";
    }
}

int main() {
    string caminhoDataset = "data/datasets/int_aleatorio.dat";
    ifstream arquivo(caminhoDataset);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << caminhoDataset << endl;
        return 1;
    }

    cout << "Lendo dataset: " << caminhoDataset << "...\n";
    vector<int> tarefas;
    int item;

    while (arquivo >> item) {
        tarefas.push_back(item);
    }
    arquivo.close();

    cout << "Total de " << tarefas.size() << " tarefas carregadas.\n";

    Treap treap;

    for (int t : tarefas) {
        treap.inserirElemento(t);
    }

    cout << "Treap populada com sucesso!\n\n";

    testarTreapFilaPrioridade(treap, tarefas.size());

    return 0;
}