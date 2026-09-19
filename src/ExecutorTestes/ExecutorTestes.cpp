#include "ExecutorTestes.hpp"
#include "GerenciadorArquivos/GerenciadorArquivos.hpp"
#include "AVL/AVL.hpp"
#include "BST/BST.hpp"
#include "Splay/Splay.hpp"
#include "Treap/Treap.hpp"
#include "Trie/Trie.hpp"
#include "Patricia/Patricia.hpp"
#include "KdTree/KdTree.hpp"
#include <chrono>

const std::string CAMINHO_CSV = "data/output/resultado_dados_comparativos.csv";
const int NUM_RODADAS = 10;

// ============================================================================
// 1. FUNÇÕES DE TESTE - ÁRVORES NUMÉRICAS (AVL, BST, Splay, Treap)
// ============================================================================

void ExecutorTestes::testarAVL(const std::vector<int>& dados, const std::string& dataset, size_t n) {
    if (dados.empty()) return;

    double tempoTotalInsert = 0.0, tempoTotalSearch = 0.0, tempoTotalDelete = 0.0;
    long long rotacoesInsert = 0, comparacoesInsert = 0, alturaInsert = 0;
    long long rotacoesSearch = 0, comparacoesSearch = 0;
    long long rotacoesDelete = 0, comparacoesDelete = 0, alturaDelete = 0;
    size_t memoria = 0, memoriaDelete = 0;
    size_t limiteRemocao = n / 2;

    for (int r = 0; r < NUM_RODADAS; ++r) {
        AVL arvore;

        // INSERÇÃO
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int v : dados) arvore.inserirElemento(v);
        auto fim = std::chrono::high_resolution_clock::now();
        tempoTotalInsert += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            rotacoesInsert = arvore.getNumRotacoes();
            comparacoesInsert = arvore.getNumComparacoes();
            memoria = arvore.getConsumoMemoria();
            alturaInsert = arvore.calcularAlturaAVL();
        }

        // BUSCA
        inicio = std::chrono::high_resolution_clock::now();
        for (int v : dados) arvore.buscarElemento(v);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalSearch += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            rotacoesSearch = arvore.getNumRotacoes();
            comparacoesSearch = arvore.getNumComparacoes();
        }

        // REMOÇÃO
        inicio = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < limiteRemocao; ++i) arvore.deletarElemento(dados[i]);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalDelete += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            rotacoesDelete = arvore.getNumRotacoes();
            comparacoesDelete = arvore.getNumComparacoes();
            alturaDelete = arvore.calcularAlturaAVL();
            memoriaDelete = arvore.getConsumoMemoria(); 
        }
    }

    double mediaInsert = tempoTotalInsert / NUM_RODADAS;
    double mediaSearch = tempoTotalSearch / NUM_RODADAS;
    double mediaDelete = tempoTotalDelete / NUM_RODADAS;

    Resultado resInsert;
    resInsert.dataset = dataset;
    resInsert.estrutura = "AVL";
    resInsert.operacao = "Insercao";
    resInsert.tamanhoN = n;
    resInsert.tempoTotalSegundos = mediaInsert;
    resInsert.tempoMedioNanossegundos = (mediaInsert * 1e9) / n;
    resInsert.rotacoes = rotacoesInsert;
    resInsert.comparacoes = comparacoesInsert;
    resInsert.memoria = memoria;
    resInsert.altura = alturaInsert;
    resInsert.observacao = "Sucesso";
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resInsert);

    Resultado resSearch = resInsert;
    resSearch.operacao = "Busca";
    resSearch.tempoTotalSegundos = mediaSearch;
    resSearch.tempoMedioNanossegundos = (mediaSearch * 1e9) / n;
    resSearch.rotacoes = rotacoesSearch;
    resSearch.comparacoes = comparacoesSearch;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resSearch);

    Resultado resDelete = resInsert;
    resDelete.operacao = "Remocao";
    resDelete.tempoTotalSegundos = mediaDelete;
    resDelete.tempoMedioNanossegundos = (limiteRemocao > 0) ? (mediaDelete * 1e9) / limiteRemocao : 0;
    resDelete.rotacoes = rotacoesDelete;
    resDelete.comparacoes = comparacoesDelete;
    resDelete.altura = alturaDelete;
    resDelete.memoria = memoriaDelete; 
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resDelete);
}

void ExecutorTestes::testarBST(const std::vector<int>& dados, const std::string& dataset, size_t n) {
    if (dados.empty()) return;

    if ((dataset == "Ordenado" || dataset == "Decrescente") && n > 8000) {
        std::cout << "[IGNORADO] BST omitida para N=" << n << " (" << dataset << ") para evitar estouro de pilha.\n";
        return;
    }

    double tempoTotalInsert = 0.0, tempoTotalSearch = 0.0, tempoTotalDelete = 0.0;
    long long comparacoesInsert = 0, comparacoesSearch = 0, comparacoesDelete = 0;
    long long alturaInsert = 0, alturaDelete = 0;
    size_t memoria = 0, memoriaDelete = 0;
    size_t limiteRemocao = n / 2;

    for (int r = 0; r < NUM_RODADAS; ++r) {
        BST arvore;

        // INSERÇÃO
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int v : dados) arvore.inserirElemento(v);
        auto fim = std::chrono::high_resolution_clock::now();
        tempoTotalInsert += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            comparacoesInsert = arvore.getNumComparacoes();
            memoria = arvore.getConsumoMemoria();
            alturaInsert = arvore.calcularAlturaBST();
        }

        // BUSCA
        inicio = std::chrono::high_resolution_clock::now();
        for (int v : dados) arvore.buscarElemento(v);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalSearch += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) comparacoesSearch = arvore.getNumComparacoes();

        // REMOÇÃO
        inicio = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < limiteRemocao; ++i) arvore.deletarElemento(dados[i]);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalDelete += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            comparacoesDelete = arvore.getNumComparacoes();
            alturaDelete = arvore.calcularAlturaBST();
            memoriaDelete = arvore.getConsumoMemoria();
        }
    }

    double mediaInsert = tempoTotalInsert / NUM_RODADAS;
    double mediaSearch = tempoTotalSearch / NUM_RODADAS;
    double mediaDelete = tempoTotalDelete / NUM_RODADAS;

    Resultado resInsert;
    resInsert.dataset = dataset;
    resInsert.estrutura = "BST";
    resInsert.operacao = "Insercao";
    resInsert.tamanhoN = n;
    resInsert.tempoTotalSegundos = mediaInsert;
    resInsert.tempoMedioNanossegundos = (mediaInsert * 1e9) / n;
    resInsert.rotacoes = 0;
    resInsert.comparacoes = comparacoesInsert;
    resInsert.memoria = memoria;
    resInsert.altura = alturaInsert;
    resInsert.observacao = "Sucesso";
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resInsert);

    Resultado resSearch = resInsert;
    resSearch.operacao = "Busca";
    resSearch.tempoTotalSegundos = mediaSearch;
    resSearch.tempoMedioNanossegundos = (mediaSearch * 1e9) / n;
    resSearch.comparacoes = comparacoesSearch;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resSearch);

    Resultado resDelete = resInsert;
    resDelete.operacao = "Remocao";
    resDelete.tempoTotalSegundos = mediaDelete;
    resDelete.tempoMedioNanossegundos = (limiteRemocao > 0) ? (mediaDelete * 1e9) / limiteRemocao : 0;
    resDelete.comparacoes = comparacoesDelete;
    resDelete.altura = alturaDelete;
    resDelete.memoria = memoriaDelete;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resDelete);
}

void ExecutorTestes::testarSplay(const std::vector<int>& dados, const std::string& dataset, size_t n) {
    if (dados.empty()) return;

    double tempoTotalInsert = 0.0, tempoTotalSearch = 0.0, tempoTotalDelete = 0.0;
    long long rotacoesInsert = 0, comparacoesInsert = 0, alturaInsert = 0;
    long long rotacoesSearch = 0, comparacoesSearch = 0;
    long long rotacoesDelete = 0, comparacoesDelete = 0, alturaDelete = 0;
    size_t memoria = 0, memoriaDelete = 0;
    size_t limiteRemocao = n / 2;

    for (int r = 0; r < NUM_RODADAS; ++r) {
        Splay arvore;

        // INSERÇÃO
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int v : dados) arvore.inserirElemento(v);
        auto fim = std::chrono::high_resolution_clock::now();
        tempoTotalInsert += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            rotacoesInsert = arvore.getNumRotacoes();
            comparacoesInsert = arvore.getNumComparacoes();
            memoria = arvore.getConsumoMemoria();
            alturaInsert = arvore.alturaSplay();
        }

        // BUSCA
        inicio = std::chrono::high_resolution_clock::now();
        for (int v : dados) arvore.buscarElemento(v);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalSearch += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            rotacoesSearch = arvore.getNumRotacoes();
            comparacoesSearch = arvore.getNumComparacoes();
        }

        // REMOÇÃO
        inicio = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < limiteRemocao; ++i) arvore.deletarElemento(dados[i]);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalDelete += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            rotacoesDelete = arvore.getNumRotacoes();
            comparacoesDelete = arvore.getNumComparacoes();
            alturaDelete = arvore.alturaSplay();
            memoriaDelete = arvore.getConsumoMemoria();
        }
    }

    double mediaInsert = tempoTotalInsert / NUM_RODADAS;
    double mediaSearch = tempoTotalSearch / NUM_RODADAS;
    double mediaDelete = tempoTotalDelete / NUM_RODADAS;

    Resultado resInsert;
    resInsert.dataset = dataset;
    resInsert.estrutura = "Splay";
    resInsert.operacao = "Insercao";
    resInsert.tamanhoN = n;
    resInsert.tempoTotalSegundos = mediaInsert;
    resInsert.tempoMedioNanossegundos = (mediaInsert * 1e9) / n;
    resInsert.rotacoes = rotacoesInsert;
    resInsert.comparacoes = comparacoesInsert;
    resInsert.memoria = memoria;
    resInsert.altura = alturaInsert;
    resInsert.observacao = "Sucesso";
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resInsert);

    Resultado resSearch = resInsert;
    resSearch.operacao = "Busca";
    resSearch.tempoTotalSegundos = mediaSearch;
    resSearch.tempoMedioNanossegundos = (mediaSearch * 1e9) / n;
    resSearch.rotacoes = rotacoesSearch;
    resSearch.comparacoes = comparacoesSearch;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resSearch);

    Resultado resDelete = resInsert;
    resDelete.operacao = "Remocao";
    resDelete.tempoTotalSegundos = mediaDelete;
    resDelete.tempoMedioNanossegundos = (limiteRemocao > 0) ? (mediaDelete * 1e9) / limiteRemocao : 0;
    resDelete.rotacoes = rotacoesDelete;
    resDelete.comparacoes = comparacoesDelete;
    resDelete.altura = alturaDelete;
    resDelete.memoria = memoriaDelete;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resDelete);
}

void ExecutorTestes::testarTreap(const std::vector<int>& dados, const std::string& dataset, size_t n) {
    if (dados.empty()) return;

    double tempoTotalInsert = 0.0, tempoTotalSearch = 0.0, tempoTotalDelete = 0.0;
    long long rotacoesInsert = 0, comparacoesInsert = 0, alturaInsert = 0;
    long long rotacoesSearch = 0, comparacoesSearch = 0;
    long long rotacoesDelete = 0, comparacoesDelete = 0, alturaDelete = 0;
    size_t memoria = 0, memoriaDelete = 0;
    size_t limiteRemocao = n / 2;

    for (int r = 0; r < NUM_RODADAS; ++r) {
        Treap arvore;

        // INSERÇÃO
        auto inicio = std::chrono::high_resolution_clock::now();
        for (int v : dados) arvore.inserirElemento(v);
        auto fim = std::chrono::high_resolution_clock::now();
        tempoTotalInsert += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            rotacoesInsert = arvore.getNumRotacoes();
            comparacoesInsert = arvore.getNumComparacoes();
            memoria = arvore.getConsumoMemoria();
            alturaInsert = arvore.alturaTreap();
        }

        // BUSCA
        inicio = std::chrono::high_resolution_clock::now();
        for (int v : dados) arvore.buscarElemento(v);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalSearch += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            rotacoesSearch = arvore.getNumRotacoes();
            comparacoesSearch = arvore.getNumComparacoes();
        }

        // REMOÇÃO
        inicio = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < limiteRemocao; ++i) arvore.deletarElemento(dados[i]);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalDelete += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            rotacoesDelete = arvore.getNumRotacoes();
            comparacoesDelete = arvore.getNumComparacoes();
            alturaDelete = arvore.alturaTreap();
            memoriaDelete = arvore.getConsumoMemoria();
        }
    }

    double mediaInsert = tempoTotalInsert / NUM_RODADAS;
    double mediaSearch = tempoTotalSearch / NUM_RODADAS;
    double mediaDelete = tempoTotalDelete / NUM_RODADAS;

    Resultado resInsert;
    resInsert.dataset = dataset;
    resInsert.estrutura = "Treap";
    resInsert.operacao = "Insercao";
    resInsert.tamanhoN = n;
    resInsert.tempoTotalSegundos = mediaInsert;
    resInsert.tempoMedioNanossegundos = (mediaInsert * 1e9) / n;
    resInsert.rotacoes = rotacoesInsert;
    resInsert.comparacoes = comparacoesInsert;
    resInsert.memoria = memoria;
    resInsert.altura = alturaInsert;
    resInsert.observacao = "Sucesso";
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resInsert);

    Resultado resSearch = resInsert;
    resSearch.operacao = "Busca";
    resSearch.tempoTotalSegundos = mediaSearch;
    resSearch.tempoMedioNanossegundos = (mediaSearch * 1e9) / n;
    resSearch.rotacoes = rotacoesSearch;
    resSearch.comparacoes = comparacoesSearch;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resSearch);

    Resultado resDelete = resInsert;
    resDelete.operacao = "Remocao";
    resDelete.tempoTotalSegundos = mediaDelete;
    resDelete.tempoMedioNanossegundos = (limiteRemocao > 0) ? (mediaDelete * 1e9) / limiteRemocao : 0;
    resDelete.rotacoes = rotacoesDelete;
    resDelete.comparacoes = comparacoesDelete;
    resDelete.altura = alturaDelete;
    resDelete.memoria = memoriaDelete;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resDelete);
}

// ============================================================================
// 2. FUNÇÕES DE TESTE - ÁRVORES DE STRINGS (Trie, Patricia)
// ============================================================================

void ExecutorTestes::testarTrie(const std::vector<std::string>& dados, const std::string& dataset, size_t n) {
    if (dados.empty()) return;

    double tempoTotalInsert = 0.0, tempoTotalSearch = 0.0, tempoTotalDelete = 0.0;
    long long comparacoesInsert = 0, comparacoesSearch = 0, comparacoesDelete = 0;
    long long alturaInsert = 0, alturaDelete = 0;
    size_t memoria = 0, memoriaDelete = 0;
    size_t limiteRemocao = n / 2;

    for (int r = 0; r < NUM_RODADAS; ++r) {
        Trie arvore;

        // INSERÇÃO
        auto inicio = std::chrono::high_resolution_clock::now();
        for (const auto& str : dados) arvore.inserirElemento(str);
        auto fim = std::chrono::high_resolution_clock::now();
        tempoTotalInsert += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            comparacoesInsert = arvore.getNumComparacoes();
            memoria = arvore.getConsumoMemoria();
            alturaInsert = arvore.alturaTrie();
        }

        // BUSCA
        inicio = std::chrono::high_resolution_clock::now();
        for (const auto& str : dados) arvore.buscarElemento(str);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalSearch += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) comparacoesSearch = arvore.getNumComparacoes();

        // REMOÇÃO
        inicio = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < limiteRemocao; ++i) arvore.deletarElemento(dados[i]);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalDelete += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1){
            comparacoesDelete = arvore.getNumComparacoes();
            alturaDelete = arvore.alturaTrie();
            memoriaDelete = arvore.getConsumoMemoria();
        }
    }

    double mediaInsert = tempoTotalInsert / NUM_RODADAS;
    double mediaSearch = tempoTotalSearch / NUM_RODADAS;
    double mediaDelete = tempoTotalDelete / NUM_RODADAS;

    Resultado resInsert;
    resInsert.dataset = dataset;
    resInsert.estrutura = "Trie";
    resInsert.operacao = "Insercao";
    resInsert.tamanhoN = n;
    resInsert.tempoTotalSegundos = mediaInsert;
    resInsert.tempoMedioNanossegundos = (mediaInsert * 1e9) / n;
    resInsert.rotacoes = 0;
    resInsert.comparacoes = comparacoesInsert;
    resInsert.memoria = memoria;
    resInsert.altura = alturaInsert;
    resInsert.observacao = "Sucesso";
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resInsert);

    Resultado resSearch = resInsert;
    resSearch.operacao = "Busca";
    resSearch.tempoTotalSegundos = mediaSearch;
    resSearch.tempoMedioNanossegundos = (mediaSearch * 1e9) / n;
    resSearch.comparacoes = comparacoesSearch;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resSearch);

    Resultado resDelete = resInsert;
    resDelete.operacao = "Remocao";
    resDelete.tempoTotalSegundos = mediaDelete;
    resDelete.tempoMedioNanossegundos = (limiteRemocao > 0) ? (mediaDelete * 1e9) / limiteRemocao : 0;
    resDelete.comparacoes = comparacoesDelete;
    resDelete.memoria = memoriaDelete;
    resDelete.altura = alturaDelete;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resDelete);
}

void ExecutorTestes::testarPatricia(const std::vector<std::string>& dados, const std::string& dataset, size_t n) {
    if (dados.empty()) return;

    double tempoTotalInsert = 0.0, tempoTotalSearch = 0.0, tempoTotalDelete = 0.0;
    long long comparacoesInsert = 0, comparacoesSearch = 0, comparacoesDelete = 0;
    long long alturaInsert = 0, alturaDelete = 0;
    size_t memoria = 0, memoriaDelete = 0;
    size_t limiteRemocao = n / 2;

    for (int r = 0; r < NUM_RODADAS; ++r) {
        Patricia arvore;

        // INSERÇÃO
        auto inicio = std::chrono::high_resolution_clock::now();
        for (const auto& str : dados) arvore.inserirElemento(str);
        auto fim = std::chrono::high_resolution_clock::now();
        tempoTotalInsert += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) {
            comparacoesInsert = arvore.getNumComparacoes();
            memoria = arvore.getConsumoMemoria();
            alturaInsert = arvore.alturaPatricia();
        }

        // BUSCA
        inicio = std::chrono::high_resolution_clock::now();
        for (const auto& str : dados) arvore.buscarElemento(str);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalSearch += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1) comparacoesSearch = arvore.getNumComparacoes();

        // REMOÇÃO
        inicio = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < limiteRemocao; ++i) arvore.deletarElemento(dados[i]);
        fim = std::chrono::high_resolution_clock::now();
        tempoTotalDelete += std::chrono::duration<double>(fim - inicio).count();

        if (r == NUM_RODADAS - 1){
            comparacoesDelete = arvore.getNumComparacoes();
            memoriaDelete = arvore.getConsumoMemoria();
            alturaDelete = arvore.alturaPatricia();
        }
    }

    double mediaInsert = tempoTotalInsert / NUM_RODADAS;
    double mediaSearch = tempoTotalSearch / NUM_RODADAS;
    double mediaDelete = tempoTotalDelete / NUM_RODADAS;

    Resultado resInsert;
    resInsert.dataset = dataset;
    resInsert.estrutura = "Patricia";
    resInsert.operacao = "Insercao";
    resInsert.tamanhoN = n;
    resInsert.tempoTotalSegundos = mediaInsert;
    resInsert.tempoMedioNanossegundos = (mediaInsert * 1e9) / n;
    resInsert.rotacoes = 0;
    resInsert.comparacoes = comparacoesInsert;
    resInsert.memoria = memoria;
    resInsert.altura = alturaInsert;
    resInsert.observacao = "Sucesso";
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resInsert);

    Resultado resSearch = resInsert;
    resSearch.operacao = "Busca";
    resSearch.tempoTotalSegundos = mediaSearch;
    resSearch.tempoMedioNanossegundos = (mediaSearch * 1e9) / n;
    resSearch.comparacoes = comparacoesSearch;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resSearch);

    Resultado resDelete = resInsert;
    resDelete.operacao = "Remocao";
    resDelete.tempoTotalSegundos = mediaDelete;
    resDelete.tempoMedioNanossegundos = (limiteRemocao > 0) ? (mediaDelete * 1e9) / limiteRemocao : 0;
    resDelete.comparacoes = comparacoesDelete;
    resDelete.memoria = memoriaDelete;
    resDelete.altura = alturaDelete;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resDelete);
}

// ============================================================================
// 3. FUNÇÃO DE TESTE - ÁRVORE ESPACIAL (KD-Tree)
// ============================================================================

void ExecutorTestes::testarKDTree(const std::vector<std::vector<double>>& pontos, const std::string& dataset, size_t n, int dimensao) {
    if (pontos.empty()) return;

    double tempoTotalInsert = 0.0, tempoTotalSearch = 0.0, tempoTotalDelete = 0.0;
    long long comparacoesInsert = 0, comparacoesSearch = 0, comparacoesDelete = 0;
    long long alturaInsert = 0, alturaDelete = 0;
    size_t memoria = 0, memoriaDelete = 0;
    //size_t limiteRemocao = n / 2;
    size_t limiteRemocao = std::min(pontos.size() / 20, static_cast<size_t>(1000));

    for (int r = 0; r < NUM_RODADAS; ++r) {
        KdTree arvore(dimensao);

        // INSERÇÃO
        auto startInsert = std::chrono::high_resolution_clock::now();
        for (const auto& p : pontos) arvore.inserirElemento(p);
        auto endInsert = std::chrono::high_resolution_clock::now();
        tempoTotalInsert += std::chrono::duration<double>(endInsert - startInsert).count();

        if (r == NUM_RODADAS - 1) {
            comparacoesInsert = arvore.getNumComparacoes();
            memoria = arvore.getConsumoMemoria();
            alturaInsert = arvore.alturaKdTree();
        }

        // BUSCA
        auto startSearch = std::chrono::high_resolution_clock::now();
        for (const auto& p : pontos) arvore.buscarElemento(p);
        auto endSearch = std::chrono::high_resolution_clock::now();
        tempoTotalSearch += std::chrono::duration<double>(endSearch - startSearch).count();

        if (r == NUM_RODADAS - 1) comparacoesSearch = arvore.getNumComparacoes();

        // REMOÇÃO
        auto startDelete = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < limiteRemocao && i < pontos.size(); ++i) arvore.deletarElemento(pontos[i]);
        auto endDelete = std::chrono::high_resolution_clock::now();
        tempoTotalDelete += std::chrono::duration<double>(endDelete - startDelete).count();

        if (r == NUM_RODADAS - 1) {
            comparacoesDelete = arvore.getNumComparacoes();
            alturaDelete = arvore.alturaKdTree();
            memoriaDelete = arvore.getConsumoMemoria();
        }
    }

    double mediaInsert = tempoTotalInsert / NUM_RODADAS;
    double mediaSearch = tempoTotalSearch / NUM_RODADAS;
    double mediaDelete = tempoTotalDelete / NUM_RODADAS;

    Resultado resInsert;
    resInsert.dataset = dataset;
    resInsert.estrutura = "KDTree";
    resInsert.operacao = "Insercao";
    resInsert.tamanhoN = n;
    resInsert.tempoTotalSegundos = mediaInsert;
    resInsert.tempoMedioNanossegundos = (mediaInsert * 1e9) / n;
    resInsert.rotacoes = 0;
    resInsert.comparacoes = comparacoesInsert;
    resInsert.memoria = memoria;
    resInsert.altura = alturaInsert;
    resInsert.observacao = "Dimensao=" + std::to_string(dimensao);
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resInsert);

    Resultado resSearch = resInsert;
    resSearch.operacao = "Busca";
    resSearch.tempoTotalSegundos = mediaSearch;
    resSearch.tempoMedioNanossegundos = (mediaSearch * 1e9) / n;
    resSearch.comparacoes = comparacoesSearch;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resSearch);

    Resultado resDelete = resInsert;
    resDelete.operacao = "Remocao";
    resDelete.tamanhoN = limiteRemocao;
    resDelete.tempoTotalSegundos = mediaDelete;
    resDelete.tempoMedioNanossegundos = (limiteRemocao > 0) ? (mediaDelete * 1e9) / limiteRemocao : 0;
    resDelete.comparacoes = comparacoesDelete;
    resDelete.altura = alturaDelete;
    resDelete.memoria = memoriaDelete;
    GerenciadorArquivos::salvarResultadoCSV(CAMINHO_CSV, resDelete);
}

// ============================================================================
// 4. BATERIA COMPLETA DE TESTES AUTOMÁTICOS
// ============================================================================

void ExecutorTestes::rodarBateriaCompleta() {
    std::cout << "\n=================================================================" << std::endl;
    std::cout << " INICIANDO BATERIA AUTOMATICA DE TESTES (N = 100 A 100.000) " << std::endl;
    std::cout << "=================================================================\n" << std::endl;

    std::vector<size_t> tamanhos = {100, 1000, 10000, 50000, 100000};
    
    for (size_t n : tamanhos) {
        std::cout << ">>> Executando testes para N = " << n << " ..." << std::endl;

        // --- 1. TESTES NUMÉRICOS (AVL, BST, Splay, Treap) ---
        std::vector<std::pair<std::string, std::string>> datasetsNumericos = {
            {"data/datasets/int_aleatorio.dat", "Aleatorio"},
            {"data/datasets/int_ordenado.dat", "Ordenado"},
            {"data/datasets/int_decrescente.dat", "Decrescente"}
        };

        for (const auto& ds : datasetsNumericos) {
            auto dadosNum = GerenciadorArquivos::lerInteiros(ds.first, n);
            if (!dadosNum.empty()) {
                testarAVL(dadosNum, ds.second, n);
                testarBST(dadosNum, ds.second, n);
                testarSplay(dadosNum, ds.second, n);
                testarTreap(dadosNum, ds.second, n);
            }
        }

        // --- 2. TESTES DE STRINGS (Trie, Patricia) ---
        std::vector<std::pair<std::string, std::string>> datasetsStrings = {
            {"data/datasets/string_decrescente.dat", "Decrescente"},
            {"data/datasets/string_ordenado.dat", "Ordenado"},
            {"data/datasets/string_aleatorio.dat", "Aleatorio"}
        };

        for (const auto& ds : datasetsStrings) {
            auto dadosStr = GerenciadorArquivos::lerLinhas(ds.first, n);
            if (!dadosStr.empty()) {
                testarTrie(dadosStr, ds.second, n);
                testarPatricia(dadosStr, ds.second, n);
            }
        }

        // --- 3. TESTES ESPACIAIS (KD-Tree) ---
        std::vector<std::pair<std::string, std::string>> datasetsEspaciais = {
            {"data/datasets/kd_aleatorio.dat", "Aleatorio"},
            {"data/datasets/kd_clusterizado.dat", "Clusterizado"},
            {"data/datasets/kd_diagonal.dat", "Diagonal"}
        };

        int dimensao = 2;
        for (const auto& ds : datasetsEspaciais){
            if (ds.second == "Diagonal" && n > 10000) {
                std::cout << "[IGNORADO] KDTree Diagonal omitida para N=" << n << " para evitar O(n^2).\n";
                dimensao++;
                continue;
            }
            auto pontosKD = GerenciadorArquivos::lerPontos(ds.first, n, dimensao);
            if (!pontosKD.empty()) {
                testarKDTree(pontosKD, ds.second, n, dimensao);
            }
            dimensao++;
        }
    }

    std::cout << "\n[SUCESSO] Bateria concluida! Resultados gravados em: " << CAMINHO_CSV << std::endl;
}

