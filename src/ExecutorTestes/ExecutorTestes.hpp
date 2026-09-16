#ifndef EXECUTOR_TESTES_HPP
#define EXECUTOR_TESTES_HPP
#include <vector>
#include <string>
#include <cstddef>
#include "GerenciadorArquivos/GerenciadorArquivos.hpp"

class ExecutorTestes{
public:
    static void rodarBateriaCompleta();

private:
    static void testarAVL(const std::vector<int>& dados, const std::string& dataset, size_t n);
    static void testarBST(const std::vector<int>& dados, const std::string& dataset, size_t n);
    static void testarSplay(const std::vector<int>& dados, const std::string& dataset, size_t n);
    static void testarTreap(const std::vector<int>& dados, const std::string& dataset, size_t n);
    static void testarTrie(const std::vector<std::string>& dados, const std::string& dataset, size_t n);
    static void testarPatricia(const std::vector<std::string>& dados, const std::string& dataset, size_t n);
    static void testarKDTree(const std::vector<std::vector<double>>& pontos, const std::string& dataset, size_t n, int dimensao);
};

#endif 