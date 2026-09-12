#ifndef TREAP_HPP
#define TREAP_HPP
#include <string>
#include <fstream>
#include <random>
#include <cstdint>
#include <algorithm>
#include <iostream>

class Treap{
    private:
        struct No{
            int dado;
            No *esquerda;
            No *direita;
            uint64_t prioridade;

            No(int dado, uint64_t prioridade): dado(dado), esquerda(nullptr), direita(nullptr), prioridade(prioridade){}
        };

        long long num_comparacoes;
        long long consumo_memoria;
        long long tempo_execucao;
        long long num_rotacoes;
        bool ultima_operacao_sucesso;
        No *raiz;
        std::mt19937_64 geradorBinario;
        std::uniform_int_distribution<uint64_t> distribuicaoInteiro;

        uint64_t gerarPrioridade();
        No* inserirRecursivamente(No *no, int elemento);
        No* deletarRecursivamente(No *no, int elemento);
        No* buscarRecursivamente(No *no, int elemento);
        void destruirTreap(No *no);
        void exibirInOrdemRecursivamente(No *no);
        No* rotacaoDireita(No *no);
        No* rotacaoEsquerda(No *no);
        void gerarDOTRecursivo(No *no, std::ofstream& arquivo);
        int calcularAlturaRecursivamente(No *no);

    public:
        Treap();
        ~Treap();

        Treap(const Treap&) = delete;
        Treap& operator=(const Treap&) = delete;

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

        void inserirElemento(int elemento);
        bool buscarElemento(int elemento);
        void deletarElemento(int elemento);
        void exibirTreapInOrdem();
        void gerarDOT(const std::string& caminho);
        int alturaTreap();
};


#endif