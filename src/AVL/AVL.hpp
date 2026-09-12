#ifndef AVL_HPP
#define AVL_HPP
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

class AVL {
    private:
        struct No{
            int dado;
            No *esquerda;
            No *direita;
            int h; // altura, caso nullptr terá a altura de -1

            No(int dado): dado(dado), esquerda(nullptr), direita(nullptr), h(0){}
        };

        long long num_comparacoes;
        long long consumo_memoria;
        long long tempo_execucao;
        long long num_rotacoes;
        bool ultima_operacao_sucesso;
        No *raiz;

        No* inserirRecursivamente(No *no, int elemento);
        No* deletarRecursivamente(No *no, int elemento);
        No* buscarRecursivamente(No *no, int elemento);
        void destruirAVL(No *no);
        void exibirInOrdemRecursivamente(No *no);
        No* buscarSucessor(No *no);
        void calcularAlturaNo(No *no);
        int calcularFatorBalanceamento(No *no);
        No* rotacaoDireita(No *no);
        No* rotacaoEsquerda(No *no);
        No* rotacaoDuplaDireita(No *no);
        No* rotacaoDuplaEsquerda(No *no);
        No* balanceamentoAVL(No *no);
        void gerarDOTRecursivo(No *no, std::ofstream& arquivo);
        
    public:
        long long getConsumoMemoria();
        long long getNumComparacoes();
        long long getTempoExecucao();
        long long getNumRotacoes();
        bool getUltimaOperacaoSucesso();
        void setConsumoMemoria(long long num);
        void setNumComparacoes(long long num);
        void setTempoExecucao(long long num);
        void setNumRotacoes(long long num);
        void setUltimaOperacaoSucesso(bool status);

        AVL();
        ~AVL();

        AVL(const AVL&) = delete;
        AVL& operator=(const AVL&) = delete;

        void inserirElemento(int elemento);
        bool buscarElemento(int elemento);
        void deletarElemento(int elemento);
        void exibirAVLInOrdem();
        void gerarDOT(const std::string& caminho);
        int calcularAlturaAVL();
};


#endif