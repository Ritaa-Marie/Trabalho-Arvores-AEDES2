#ifndef PATRICIA_HPP
#define PATRICIA_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>

class Patricia{
    private: 
        static constexpr int ALFABETO = 26;
        int idNoAtual;

        struct No{
            bool fim;
            int idNo;
            std::string prefixo;
            No* filhos[ALFABETO];
            
            void inicializarNo(int id){
                fim = false;
                idNo = id;
                prefixo = "";

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
        void destruirPatricia(No *no);
        void exibirInOrdemRecursivamente(No *no, std::string palavraFormada);
        int calcularAlturaRecursivamente(No *no);
        void gerarDOTRecursivo(No *no, std::ofstream& arquivo);
        No* criarNo();
        No* split(No *no, size_t posicaoDiverge);
        No* merge(No *no);
        void transformarMinusculo(std::string& palavra);
        void substituirCaracteresEspeciais(std::string& palavra);
        std::string normalizarPalavra(const std::string& palavra);

    public:
        Patricia();
        ~Patricia();

        Patricia(const Patricia&) = delete;
        Patricia& operator=(const Patricia&) = delete;

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
        void exibirPatriciaInOrdem();
        int alturaPatricia();
        void gerarDOT(const std::string& caminho);
};

#endif