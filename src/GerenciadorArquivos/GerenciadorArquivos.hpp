#ifndef GERENCIADOR_ARQUIVOS_HPP
#define GERENCIADOR_ARQUIVOS_HPP

#include <string>
#include <vector>
#include <cstddef>


struct Resultado{
    std::string dataset;
    std::string estrutura;
    std::string operacao;
    size_t tamanhoN;
    double tempoTotalSegundos;
    double tempoMedioNanossegundos;
    long long rotacoes;
    long long comparacoes;
    long long memoria;      
    int altura;
    int dimensao;
    std::string observacao;
};

class GerenciadorArquivos {
public:
    static std::vector<std::string> lerLinhas(const std::string& caminho, size_t quantidade);
    static std::vector<int> lerInteiros(const std::string& caminho, size_t quantidade);
    static std::vector<std::vector<double>> lerPontos(const std::string& caminho, size_t quantidade, int dimensao);

    static void salvarResultadoCSV(const std::string& caminho, const Resultado& resultado);
};

#endif 