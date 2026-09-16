#include "GerenciadorArquivos.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

std::vector<std::string> GerenciadorArquivos::lerLinhas(const std::string& caminho, size_t quantidade) {
    std::vector<std::string> dados;
    std::ifstream arq(caminho);

    if (!arq.is_open()) {
        std::cerr << "[ERRO] Nao foi possivel abrir o arquivo: " << caminho << std::endl;
        return dados;
    }

    dados.reserve(quantidade);
    std::string linha;
    while (dados.size() < quantidade && std::getline(arq, linha)) {
        if (!linha.empty()) {
            dados.push_back(linha);
        }
    }

    return dados;
}

std::vector<int> GerenciadorArquivos::lerInteiros(const std::string& caminho, size_t quantidade) {
    std::vector<int> dados;
    std::ifstream arq(caminho);

    if (!arq.is_open()) {
        std::cerr << "[ERRO] Nao foi possivel abrir o arquivo: " << caminho << std::endl;
        return dados;
    }

    dados.reserve(quantidade);
    int valor;
    while (dados.size() < quantidade && arq >> valor) {
        dados.push_back(valor);
    }

    return dados;
}

std::vector<std::vector<double>> GerenciadorArquivos::lerPontos(const std::string& caminho, size_t quantidade, int dimensao) {
    std::vector<std::vector<double>> pontos;
    std::ifstream arq(caminho);
    if (!arq.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << caminho << std::endl;
        return pontos;
    }

    std::string linha;
    while (pontos.size() < quantidade && std::getline(arq, linha)) {
        if (linha.empty()) continue;

        std::stringstream ss(linha);
        std::string valorStr;
        std::vector<double> ponto;

        while (ponto.size() < static_cast<size_t>(dimensao) && std::getline(ss, valorStr, ',')) {
            if (!valorStr.empty()) {
                ponto.push_back(std::stod(valorStr));
            }
        }

        if (ponto.size() == static_cast<size_t>(dimensao)) {
            pontos.push_back(ponto);
        }
    }
    return pontos;
}

void GerenciadorArquivos::salvarResultadoCSV(const std::string& caminho, const Resultado& r) {
    std::ifstream checaExiste(caminho);
    bool existe = checaExiste.good();
    checaExiste.close();

    std::ofstream arq(caminho, std::ios::app);
    if (!arq.is_open()) {
        std::cerr << "[ERRO] Nao foi possivel abrir o CSV para escrita: " << caminho << std::endl;
        return;
    }

    if (!existe) {
        arq << "Dataset,Estrutura,Operacao,TamanhoN,TempoTotal_s,TempoMedio_ns,Rotacoes,Comparacoes,Memoria,Altura,Dimensao,Observacao\n";
    }

    arq << r.dataset << ","
        << r.estrutura << ","
        << r.operacao << ","
        << r.tamanhoN << ","
        << std::fixed << std::setprecision(6) << r.tempoTotalSegundos << ","
        << std::fixed << std::setprecision(2) << r.tempoMedioNanossegundos << ","
        << r.rotacoes << ","
        << r.comparacoes << ","
        << r.memoria << ","
        << r.altura << ","
        << "\"" << r.observacao << "\"\n";
}