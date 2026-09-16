#include <iostream>
#include <chrono>
#include <filesystem>
#include "KdTree/KdTree.hpp"

using namespace std;

void testarKdTreeAplicacao(KdTree& kdtree, const vector<double>& pontoAlvo, int k) {
    filesystem::create_directories("data/output/aplicacoes");
    
    string caminhoSaida = "data/output/aplicacoes/kdtree_aplicacao_busca_espacial.dat";
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
    logPrint("  APLICAÇÃO: KD-Tree (Busca de K-Vizinhos Próximos)\n");
    logPrint("=================================================\n\n");


    auto inicio = chrono::high_resolution_clock::now();

    vector<vector<double>> vizinhos = kdtree.buscarKVizinhosProximos(pontoAlvo, k);

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::microseconds>(fim - inicio).count();


    logPrint("-> Ponto de Consulta: (");
    for (size_t i = 0; i < pontoAlvo.size(); i++) {
        logPrint(to_string(pontoAlvo[i]) + (i + 1 < pontoAlvo.size() ? ", " : ""));
    }
    logPrint(")\n");
    logPrint("-> K (quantidade de vizinhos solicitados): " + to_string(k) + "\n\n");

    logPrint("-> Vizinhos Mais Próximos Encontrados (" + to_string(vizinhos.size()) + "):\n");
    for (size_t i = 0; i < vizinhos.size(); i++) {
        logPrint("   " + to_string(i + 1) + ". Coordenadas: (");
        for (size_t j = 0; j < vizinhos[i].size(); j++) {
            logPrint(to_string(vizinhos[i][j]) + (j + 1 < vizinhos[i].size() ? ", " : ""));
        }
        logPrint(")\n");
    }

    logPrint("\n-> Métricas de Desempenho:\n");
    logPrint("   - Tempo de Execução: " + to_string(duracao) + " us (" + to_string(static_cast<double>(duracao) / 1000.0) + " ms)\n");
    logPrint("=================================================\n\n");

    if (arqSaida.is_open()) {
        arqSaida.close();
        cout << "-> Saída da aplicação salva em '" << caminhoSaida << "' com sucesso.\n";
    }
}

int main() {
    int dim = 2;
    KdTree kdtree(dim);

    string caminhoDataset = "data/datasets/kd_aleatorio.dat";
    ifstream arquivo(caminhoDataset);

    if (arquivo.is_open()) {
    cout << "Carregando dataset de '" << caminhoDataset << "'...\n";
    string linha;
    int cont = 0;

    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        for (char& c : linha) {
            if (c == ',') c = ' ';
        }

        stringstream ss(linha);
        double x, y;

        if (ss >> x >> y) {
            kdtree.inserirElemento({x, y});
            cont++;
        }
    }
        arquivo.close();
        cout << "Total de " << cont << " pontos inseridos na KD-Tree.\n\n";
    } else {
        cout << "Dataset de arquivo não encontrado. Carregando conjunto de teste manual...\n";
        vector<vector<double>> pontosTeste = {
            {2.0, 3.0}, {5.0, 4.0}, {9.0, 6.0}, {4.0, 7.0}, {8.0, 1.0}, {7.0, 2.0}
        };
        for (const auto& p : pontosTeste) {
            kdtree.inserirElemento(p);
        }
        cout << "Conjunto manual inserido na KD-Tree.\n\n";
    }

    vector<double> pontoAlvo = {9.0, 2.0};
    int k = 3;

    testarKdTreeAplicacao(kdtree, pontoAlvo, k);

    return 0;
}