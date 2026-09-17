#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include "Splay/Splay.hpp"

using namespace std;

// Teste de Aplicação: Splay Tree como Cache de Memória
void testarSplayCache(Splay& splay, const std::vector<int>& dataset) {
    if (dataset.empty()) {
        cout << "Erro: Dataset vazio!" << endl;
        return;
    }

    int totalBuscas = 10000;
    int tamanhoMaisAcessados = dataset.size() * 0.1; // 10% dos elementos mais frequentes

    ofstream arqSaida("data/output/aplicacoes/splay_aplicacao_cache.dat");
    
    // Função auxiliar para enviar a saída tanto ao terminal quanto ao arquivo
    auto logPrint = [&](const string& msg) {
        cout << msg;
        if (arqSaida.is_open()) {
            arqSaida << msg;
        }
    };

    logPrint("\n");
    logPrint("=================================================\n");
    logPrint("  APLICAÇÃO: Splay Tree como Cache de Memória\n");
    logPrint("=================================================\n");
    logPrint("Dataset total: " + to_string(dataset.size()) + " elementos.\n");
    logPrint("Tamanho dos 10% mais acessados: " + to_string(tamanhoMaisAcessados) + " elementos.\n");
    logPrint("Simulando " + to_string(totalBuscas) + " acessos com padrão 80/20...\n\n");

    auto inicio = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < totalBuscas; i++) {
        int chaveBusca;
        
        if (rand() % 100 < 80) {
            chaveBusca = dataset[rand() % tamanhoMaisAcessados]; // pega de dataset[0..9999]
        } else {
            chaveBusca = dataset[rand() % dataset.size()];
        }

        splay.buscarElemento(chaveBusca);
    }

    auto fim = std::chrono::high_resolution_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio).count();

    logPrint("-> Tempo total para 10.000 buscas: " + to_string(duracao) + " ms\n");
    logPrint("-> Tempo médio por busca: " + to_string(static_cast<double>(duracao) / totalBuscas) + " ms\n");
    
    int ultimoElementoBuscado = dataset[0];
    splay.buscarElemento(ultimoElementoBuscado);
    
    logPrint("-> Verificação de Cache: Elemento " + to_string(ultimoElementoBuscado) + " trazido para a Raiz com sucesso?\n");

    try {
        int valorRaiz = splay.getRaizValor();
        logPrint("   Valor na Raiz: " + to_string(valorRaiz) + "\n");
        
        if (valorRaiz == ultimoElementoBuscado) {
            logPrint("   [SUCESSO] O nó acessado subiu para a raiz via Splay.\n");
        } else {
            logPrint("   [ATENÇÃO] O valor na raiz é diferente do esperado.\n");
        }
    } catch (const std::runtime_error& e) {
        logPrint("   [ERRO] Exceção capturada ao acessar a raiz: " + string(e.what()) + "\n");
    }
    
    logPrint("=================================================\n\n");

    if (arqSaida.is_open()) {
        arqSaida.close();
        cout << "-> Saída gravada com sucesso em 'splay_aplicacao_cache.dat'\n";
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    string caminhoDataset = "data/datasets/int_aleatorio.dat";
    ifstream arquivo(caminhoDataset);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << caminhoDataset << endl;
        cerr << "Verifique se o arquivo existe no caminho 'data/datasets/'." << endl;
        return 1;
    }

    cout << "Lendo dataset: " << caminhoDataset << "...\n";
    vector<int> dataset;
    int valor;

    while (arquivo >> valor) {
        dataset.push_back(valor);
    }
    arquivo.close();

    cout << "Total de " << dataset.size() << " elementos carregados do arquivo.\n";

    Splay splay;
    for (int num : dataset) {
        splay.inserirElemento(num);
    }

    cout << "Árvore populada com sucesso!\n\n";
    testarSplayCache(splay, dataset);

    return 0;
}