#ifndef VISUALIZACAO_ARVORE_HPP
#define VISUALIZACAO_ARVORE_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "AVL/AVL.hpp"
#include "BST/BST.hpp"
#include "Splay/Splay.hpp"
#include "Treap/Treap.hpp"
#include "Trie/Trie.hpp"
#include "Patricia/Patricia.hpp"
#include "KdTree/KdTree.hpp"

class VisualizacaoArvore {
private:
    std::stringstream bufferDot;
    int contadorPassos;
    std::string tituloGeral;

    void exportarSubgrafoAVL(AVL::No* no, std::stringstream& ss, int noDestaque, const std::string& corDestaque);
    void exportarSubgrafoBST(BST::No* no, std::stringstream& ss, int noDestaque, const std::string& corDestaque);
    void exportarSubgrafoSplay(Splay::No* no, std::stringstream& ss, int noDestaque, const std::string& corDestaque);
    void exportarSubgrafoTreap(Treap::No* no, std::stringstream& ss, int noDestaque, const std::string& corDestaque);
    void exportarSubgrafoTrie(Trie::No* no, Trie::No* raizRef, std::stringstream& ss, Trie::No* noDestaque, const std::string& corDestaque);
    void exportarSubgrafoPatricia(Patricia::No* no, Patricia::No* raizRef, std::stringstream& ss, Patricia::No* noDestaque, const std::string& corDestaque);
    void exportarSubgrafoKdTree(KdTree::No* no, std::stringstream& ss, KdTree::No* noDestaque, const std::string& corDestaque);

    int calcularFB(AVL::No* no);

public:
    VisualizacaoArvore(const std::string& titulo = "Rastreio de Execução da Árvore");

    void registrarPassoAVL(AVL::No* raiz, const std::string& descricao, int noDestaque = -1, const std::string& corDestaque = "#fde047");
    void registrarPassoBST(BST::No* raiz, const std::string& descricao, int noDestaque = -1, const std::string& corDestaque = "#fde047");
    void registrarPassoSplay(Splay::No* raiz, const std::string& descricao, int noDestaque = -1, const std::string& corDestaque = "#fde047");
    void registrarPassoTreap(Treap::No* raiz, const std::string& descricao, int noDestaque = -1, const std::string& corDestaque = "#fde047");
    void registrarPassoTrie(Trie::No* raiz, const std::string& descricao, Trie::No* noDestaque = nullptr, const std::string& corDestaque = "#fde047");
    void registrarPassoPatricia(Patricia::No* raiz, const std::string& descricao, Patricia::No* noDestaque = nullptr, const std::string& corDestaque = "#fde047");
    void registrarPassoKdTree(KdTree::No* raiz, const std::string& descricao, KdTree::No* noDestaque = nullptr, const std::string& corDestaque = "#fde047");

    bool exportarEGerarImagem(const std::string& caminhoDot, const std::string& caminhoImagem);
};

#endif