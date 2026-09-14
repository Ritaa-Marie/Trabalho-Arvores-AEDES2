#ifndef KDTREE_HPP
#define KDTREE_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <cmath>
#include <limits>

class KdTree{
    private: 
        struct Ponto{
            std::vector<double> coordenadas;
        };

        struct No{
            Ponto ponto;
            int eixo;
            No *esquerda;
            No *direita;
            
            No(int eixo, Ponto ponto): ponto(ponto), eixo(eixo), esquerda(nullptr), direita(nullptr){}
        };

        long long num_comparacoes;
        long long consumo_memoria;
        long long tempo_execucao;
        bool ultima_operacao_sucesso;
        No *raiz;
        int k; //dimensões

        No* inserirRecursivamente(No* no, const std::vector<double>& coord, int profundidade);
        No* deletarRecursivamente(No* no, const std::vector<double>& coord, int profundidade);
        No* buscarRecursivamente(No* no, const std::vector<double>& coord, int profundidade);
        void buscarVizinhoMaisProximoRecursivo(No* no, const std::vector<double>& alvo, int profundidade, No*& melhorNo, double& melhorDistancia);
        void destruirKdTree(No *no);
        void exibirInOrdemRecursivamente(No *no);
        int calcularAlturaRecursivamente(No *no);
        void gerarDOTRecursivo(No *no, std::ofstream& arquivo);
        No* criarNo(const std::vector<double>& coordenadas, int profundidade);
        int definirEixo(int profundidade);
        bool pontoDentroDoDelimitador(const std::vector<double>& min, const std::vector<double>& max, const std::vector<double>& ponto);
        void buscarPontosNoDelimitadorRecursivo(No* no, const std::vector<double>& min, const std::vector<double>& max, int profundidade, std::vector<std::vector<double>>& resultados);
        double calcularDistancia(const std::vector<double>& ponto1, const std::vector<double>& ponto2);
        bool pontosIguais(const std::vector<double>& ponto1, const std::vector<double>& ponto2);
        No* buscarMinimo(No* no, int dimensaoAlvo, int profundidadeAtual);

    public:
        KdTree(int k = 2);
        ~KdTree();

        KdTree(const KdTree&) = delete;
        KdTree& operator=(const KdTree&) = delete;

        long long getConsumoMemoria();
        long long getNumComparacoes();
        long long getTempoExecucao();
        bool getUltimaOperacaoSucesso();
        void setConsumoMemoria(long long num);
        void setNumComparacoes(long long num);
        void setTempoExecucao(long long num);
        void setUltimaOperacaoSucesso(bool status);

        void inserirElemento(const std::vector<double>& ponto);
        bool buscarElemento(const std::vector<double>& ponto);
        std::vector<double> buscarElementoMaisProximo(const std::vector<double>& alvo);
        void deletarElemento(const std::vector<double>& ponto);
        void exibirKdTreeInOrdem();
        int alturaKdTree();
        void buscarPontosNoDelimitador(const std::vector<double>& min, const std::vector<double>& max);
        void gerarDOT(const std::string& caminho);
};

#endif