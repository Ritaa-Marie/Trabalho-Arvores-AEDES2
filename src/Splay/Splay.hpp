#ifndef SPLAY_HPP
#define SPLAY_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdexcept>

class Splay{
    // árvore em bottom-up, seguindo o artigo "Self-Adjusting Binary Search Trees"
    private:
        struct No{
            int dado;
            No *esquerda;
            No *direita;
            No *pai;

            No(int dado): dado(dado), esquerda(nullptr), direita(nullptr), pai(nullptr){}
        };

        long long num_comparacoes;
        long long consumo_memoria;
        long long tempo_execucao;
        long long num_rotacoes;
        bool ultima_operacao_sucesso;
        No *raiz;

        No* inserirRecursivamente(No *no, int elemento, No* &noInserido);
        No* deletar(No *no, int elemento);
        No* buscarRecursivamente(No *no, int elemento);
        void destruirSplay(No *no);
        void exibirInOrdemRecursivamente(No *no);
        void zig(No *no);
        void zag(No *no);
        No* buscarPredecessor(No *no);
        void splay(No *no);
        void gerarDOTRecursivo(No *no, std::ofstream& arquivo);
        int calcularAlturaRecursivamente(No *no);

        friend class VisualizacaoArvore;
    public: 
        No* getRaiz() const;
        Splay();
        ~Splay();

        Splay(const Splay&) = delete;
        Splay& operator=(const Splay&) = delete;

        long long getConsumoMemoria();
        long long getNumComparacoes();
        long long getTempoExecucao();
        long long getNumRotacoes();
        int getRaizValor();
        bool getUltimaOperacaoSucesso();
        void setConsumoMemoria(long long num);
        void setNumComparacoes(long long num);
        void setTempoExecucao(long long num);
        void setNumRotacoes(long long num);
        void setUltimaOperacaoSucesso(bool status);

        void inserirElemento(int elemento);
        bool buscarElemento(int elemento);
        void deletarElemento(int elemento);
        void exibirSplayInOrdem();
        int alturaSplay();
        void gerarDOT(const std::string& caminho);
};

#endif