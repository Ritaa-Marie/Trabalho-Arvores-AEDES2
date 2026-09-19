#ifndef TRIE_HPP
#define TRIE_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>

class Trie{
    private: 
        static constexpr int ALFABETO = 26;
        int idNoAtual;

        struct No{
            bool fim;
            int idNo;
            No* filhos[ALFABETO];
            
            void inicializarNo(int id){
                fim = false;
                idNo = id;

                for(int i=0;i<ALFABETO;i++){
                    filhos[i] = nullptr;
                }
            }

            No(int id){
                inicializarNo(id);
            } 
        };

        long long num_comparacoes;
        long long consumo_memoria;
        long long tempo_execucao;
        bool ultima_operacao_sucesso;
        No *raiz;

        No* inserirRecursivamente(No *no, const std::string& palavra, size_t caractere);
        No* deletarRecursivamente(No *no, const std::string& palavra,size_t caractere);
        No* buscarRecursivamente(No *no, const std::string& palavra, size_t caractere);
        void destruirTrie(No *no);
        void exibirInOrdemRecursivamente(No *no, std::string palavraFormada);
        int calcularAlturaRecursivamente(No *no);
        void gerarDOTRecursivo(No *no, std::ofstream& arquivo);
        No* criarNo();
        void transformarMinusculo(std::string& palavra);
        void substituirCaracteresEspeciais(std::string& palavra);
        std::string normalizarPalavra(const std::string& palavra);
        Trie::No* buscarNoDoPrefixo(No* no, const std::string& prefixo, size_t caractere);
        void coletarPalavrasComPrefixo(No* no, std::string& palavraAtual, std::vector<std::string>& resultados);

        friend class VisualizacaoArvore;
    public:
        No* getRaiz() const;
        Trie();
        ~Trie();

        Trie(const Trie&) = delete;
        Trie& operator=(const Trie&) = delete;

        long long getConsumoMemoria();
        long long getNumComparacoes();
        long long getTempoExecucao();
        bool getUltimaOperacaoSucesso();
        void setConsumoMemoria(long long num);
        void setNumComparacoes(long long num);
        void setTempoExecucao(long long num);
        void setUltimaOperacaoSucesso(bool status);

        void inserirElemento(const std::string& palavra);
        bool buscarElemento(const std::string& palavra);
        void deletarElemento(const std::string& palavra);
        void exibirTrieInOrdem();
        int alturaTrie();
        void gerarDOT(const std::string& caminho);
        std::vector<std::string> autocompletar(const std::string& prefixo);
};

#endif