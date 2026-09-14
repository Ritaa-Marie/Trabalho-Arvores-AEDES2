#include <chrono>
#include <filesystem>
#include "Patricia.hpp"

using namespace std;

void testarPatriciaLPM(Patricia& patricia, const vector<string>& ipsParaTestar) {
    filesystem::create_directories("data/output/aplicacoes");
    
    string caminhoSaida = "data/output/aplicacoes/patricia_aplicacao_roteamento.dat";
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
    logPrint("  APLICAÇÃO: Patricia Tree (Roteamento IP)\n");
    logPrint("=================================================\n\n");

    patricia.setNumComparacoes(0);

    auto inicio = chrono::high_resolution_clock::now();

    for (const string& ip : ipsParaTestar) {
        string rotaEncontrada = patricia.buscarPrefixoMaisLongo(ip);
        logPrint("-> IP/Prefixo Destino: " + ip + " => Rota: " + rotaEncontrada + "\n");
    }

    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::microseconds>(fim - inicio).count();

    long long totalComparacoes = patricia.getNumComparacoes();

    logPrint("\n-> Tempo total para " + to_string(ipsParaTestar.size()) + " consultas: " + to_string(duracao) + " us (" + to_string(static_cast<double>(duracao) / 1000.0) + " ms)\n");
    logPrint("-> Total de Comparações: " + to_string(totalComparacoes) + "\n");
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
        return 1;
    }

    cout << "Lendo dataset: " << caminhoDataset << "...\n";
    vector<string> rotas;
    string rota;

    while (arquivo >> rota) {
        rotas.push_back(rota);
    }
    arquivo.close();

    cout << "Total de " << rotas.size() << " palavras/rotas carregadas.\n";

    Patricia patricia;
    for (const string& r : rotas) {
        patricia.inserirElemento(r);
    }

    cout << "Árvore Patricia populada com sucesso!\n\n";

    vector<string> ipsTeste = {
        "algoritmobuscabase",
        "algoritmobuscabaseba",
        "algoritmoinvalidoxyz"
    };

    testarPatriciaLPM(patricia, ipsTeste);

    return 0;
}