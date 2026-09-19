#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include "Splay/Splay.hpp"
#include "VisualizacaoArvore/VisualizacaoArvore.hpp"

using namespace std;

int main(){

    Splay arvore;
    VisualizacaoArvore vis;

    auto inicio = std::chrono::high_resolution_clock::now();

    // estado 1: inserções 
    arvore.inserirElemento(10);
    arvore.inserirElemento(5);
    arvore.inserirElemento(8);
    arvore.inserirElemento(16);
    arvore.inserirElemento(-8);
    arvore.inserirElemento(56);
    arvore.inserirElemento(6);
    arvore.gerarDOT("data/output/dot/splay1.dot");
    vis.registrarPassoSplay(arvore.getRaiz(), "Inserção das chaves 10, 5,8, 16, -8, 56, 6 (último inserido vira raiz)", 6,"#fde047");

    // estado 2 : busca
    arvore.buscarElemento(-8);
    arvore.gerarDOT("data/output/dot/splay2.dot");
    vis.registrarPassoSplay(arvore.getRaiz(), "Busca pela chave -8 (Splay traz o -8 para a raiz)", -8,"#a3e269");

    // estado 3: remoção
    arvore.deletarElemento(16);
    arvore.gerarDOT("data/output/dot/splay3.dot");
    vis.registrarPassoSplay(arvore.getRaiz(), "Remoção do 16 (Splay do 16 para a raiz, divisão e fusão)", 10,"#fde047");
    
    auto fim = std::chrono::high_resolution_clock::now();
    double duracao_ms = std::chrono::duration<double, std::milli>(fim - inicio).count();

    long long numComp = arvore.getNumComparacoes();
    long long mem = arvore.getConsumoMemoria();
    int alt = arvore.alturaSplay();
    int rotacoes = arvore.getNumRotacoes(); 
    int n = 7;

    vis.exportarEGerarImagem("data/output/dot/rastreio_splay.dot", "data/output/img/rastreio/rastreio_splay.png");
    std::cout << "\nRastreio da Splay gerado em rastreio_splay.png com sucesso!\n" << std::endl;

    std::cout << "\n============================================================\n";
    std::cout << "        MÉTRICAS DO EXPERIMENTO INDIVIDUAL DA SPLAY        \n";
    std::cout << "==============================================================\n";
    std::cout << "  Tamanho da Entrada (N): " << std::setw(15) << n << "\n";
    std::cout << "  Comparações de Chaves : " << std::setw(15) << numComp << "\n";
    std::cout << "  Número de rotações    : " << std::setw(15) << rotacoes << "\n";
    std::cout << "  Consumo de Memória    : " << std::setw(15) << mem << " bytes\n";
    std::cout << "  Altura da Árvore      : " << std::setw(15) << alt << "\n";
    std::cout << "  Tempo de Execução     : " << std::setw(12) << std::fixed << std::setprecision(3) << duracao_ms << " ms\n";
    std::cout << "==================================================\n\n";

    return 0;
}


