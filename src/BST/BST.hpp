#ifndef BST_hpp
#define BST_hpp
#include <string>
#include <fstream>

class BST {
    private: 
        struct No{
            int dado;
            No *esquerda;
            No *direita;

            //construtor do nó
            No(int dado): dado(dado), esquerda(nullptr), direita(nullptr) {}
        };

    private:
        long long num_comparacoes;
        long long consumo_memoria;
        long long tempo_execucao;
        bool ultima_operacao_sucesso;
        No *raiz;

        No* inserirRecursivamente(No *no, int elemento);
        No* buscarSucessor(No *no);
        No* deletarRecursivamente(No *no, int elemento);
        No* buscarRecursivamente(No *no, int elemento);
        void exibirInOrdemRecursivamente(No *no);
        void destruirBST(No *no);
        int calcularAlturaRecursivo(No *no);
        void gerarDOTRecursivo(No *no, std::ofstream& arquivo);

    public:
        BST();
        ~BST();

        BST(const BST&) = delete;
        BST& operator=(const BST&) = delete;

        long long getNumComparacoes();
        long long getConsumoMemoria();
        long long getTempoExecucao();
        bool getUltimaOperacaoSucesso();
        void setNumComparacoes(long long num);
        void setConsumoMemoria(long long num);
        void setTempoExecucao(long long num);
        void setUltimaOperacaoSucesso(bool status);

        void inserirElemento(int elemento);
        void deletarElemento(int elemento);
        bool buscarElemento(int elemento);
        void exibirBSTInOrdem();
        void gerarDOT(const std::string& caminho);
        int calcularAlturaBST();
};

#endif