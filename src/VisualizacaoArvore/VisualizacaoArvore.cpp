#include "VisualizacaoArvore.hpp"

VisualizacaoArvore::VisualizacaoArvore(const std::string& titulo) 
    : contadorPassos(0), tituloGeral(titulo) {
    bufferDot << "digraph RastreioArvore {\n";
    bufferDot << "  rankdir=TB;\n";
    bufferDot << "    graph [ordering=\"out\"];\n";
    bufferDot << "  label=\"" << tituloGeral << "\";\n";
    bufferDot << "  labelloc=\"t\";\n";
    bufferDot << "  fontsize=16;\n";
    bufferDot << "  node [shape=circle, fontname=\"Helvetica\", fontsize=10];\n\n";
}

int VisualizacaoArvore::calcularFB(AVL::No* no) {
    if (no == nullptr) return 0;
    
    int altEsq = (no->esquerda != nullptr) ? no->esquerda->h : 0;
    int altDir = (no->direita != nullptr) ? no->direita->h : 0;
    
    return altEsq - altDir;
}

// --- AVL ---
void VisualizacaoArvore::exportarSubgrafoAVL(AVL::No* no, std::stringstream& ss, int noDestaque, const std::string& corDestaque) {
    if (no == nullptr) return;
    int fb = calcularFB(no);
    std::string estilo = "";
    if (no->dado == noDestaque) {
        estilo = ", style=filled, fillcolor=\"" + corDestaque + "\", penwidth=2.0";
    }
    ss << "    \"" << no->dado << "_p" << contadorPassos << "\" [label=\"" << no->dado << "\\nFB:" << fb << "\"" << estilo << "];\n";

    if (no->esquerda != nullptr || no->direita != nullptr) {
        if (no->esquerda != nullptr) {
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"" << no->esquerda->dado << "_p" << contadorPassos << "\";\n";
            exportarSubgrafoAVL(no->esquerda, ss, noDestaque, corDestaque);
        } else {
            ss << "    \"null_l_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"null_l_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
        }
        if (no->direita != nullptr) {
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"" << no->direita->dado << "_p" << contadorPassos << "\";\n";
            exportarSubgrafoAVL(no->direita, ss, noDestaque, corDestaque);
        } else {
            ss << "    \"null_r_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"null_r_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
        }
    }
}

void VisualizacaoArvore::registrarPassoAVL(AVL::No* raiz, const std::string& descricao, int noDestaque, const std::string& corDestaque) {
    contadorPassos++;
    bufferDot << "  subgraph cluster_passo_" << contadorPassos << " {\n";
    bufferDot << "    label=\"Passo " << contadorPassos << ": " << descricao << "\";\n";
    bufferDot << "    color=\"#cbd5e1\"; style=\"rounded\";\n";
    if (raiz != nullptr) exportarSubgrafoAVL(raiz, bufferDot, noDestaque, corDestaque);
    else bufferDot << "    empty_p" << contadorPassos << " [label=\"Árvore Vazia\", shape=plaintext];\n";
    bufferDot << "  }\n\n";
}

// --- BST ---
void VisualizacaoArvore::exportarSubgrafoBST(BST::No* no, std::stringstream& ss, int noDestaque, const std::string& corDestaque) {
    if (no == nullptr) return;
    std::string estilo = "";
    if (no->dado == noDestaque) {
        estilo = ", style=filled, fillcolor=\"" + corDestaque + "\", penwidth=2.0";
    }
    ss << "    \"" << no->dado << "_p" << contadorPassos << "\" [label=\"" << no->dado << "\"" << estilo << "];\n";

    if (no->esquerda != nullptr || no->direita != nullptr) {
        if (no->esquerda != nullptr) {
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"" << no->esquerda->dado << "_p" << contadorPassos << "\";\n";
            exportarSubgrafoBST(no->esquerda, ss, noDestaque, corDestaque);
        } else {
            ss << "    \"null_l_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"null_l_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
        }
        if (no->direita != nullptr) {
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"" << no->direita->dado << "_p" << contadorPassos << "\";\n";
            exportarSubgrafoBST(no->direita, ss, noDestaque, corDestaque);
        } else {
            ss << "    \"null_r_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"null_r_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
        }
    }
}

void VisualizacaoArvore::registrarPassoBST(BST::No* raiz, const std::string& descricao, int noDestaque, const std::string& corDestaque) {
    contadorPassos++;
    bufferDot << "  subgraph cluster_passo_" << contadorPassos << " {\n";
    bufferDot << "    label=\"Passo " << contadorPassos << ": " << descricao << "\";\n";
    bufferDot << "    color=\"#cbd5e1\"; style=\"rounded\";\n";
    if (raiz != nullptr) exportarSubgrafoBST(raiz, bufferDot, noDestaque, corDestaque);
    else bufferDot << "    empty_p" << contadorPassos << " [label=\"Árvore Vazia\", shape=plaintext];\n";
    bufferDot << "  }\n\n";
}

// --- SPLAY ---
void VisualizacaoArvore::exportarSubgrafoSplay(Splay::No* no, std::stringstream& ss, int noDestaque, const std::string& corDestaque) {
    if (no == nullptr) return;
    std::string estilo = "";
    if (no->dado == noDestaque) {
        estilo = ", style=filled, fillcolor=\"" + corDestaque + "\", penwidth=2.0";
    }
    ss << "    \"" << no->dado << "_p" << contadorPassos << "\" [label=\"" << no->dado << "\"" << estilo << "];\n";

    if (no->esquerda != nullptr || no->direita != nullptr) {
        if (no->esquerda != nullptr) {
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"" << no->esquerda->dado << "_p" << contadorPassos << "\";\n";
            exportarSubgrafoSplay(no->esquerda, ss, noDestaque, corDestaque);
        } else {
            ss << "    \"null_l_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"null_l_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
        }
        if (no->direita != nullptr) {
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"" << no->direita->dado << "_p" << contadorPassos << "\";\n";
            exportarSubgrafoSplay(no->direita, ss, noDestaque, corDestaque);
        } else {
            ss << "    \"null_r_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"null_r_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
        }
    }
}

void VisualizacaoArvore::registrarPassoSplay(Splay::No* raiz, const std::string& descricao, int noDestaque, const std::string& corDestaque) {
    contadorPassos++;
    bufferDot << "  subgraph cluster_passo_" << contadorPassos << " {\n";
    bufferDot << "    label=\"Passo " << contadorPassos << ": " << descricao << "\";\n";
    bufferDot << "    color=\"#cbd5e1\"; style=\"rounded\";\n";
    if (raiz != nullptr) exportarSubgrafoSplay(raiz, bufferDot, noDestaque, corDestaque);
    else bufferDot << "    empty_p" << contadorPassos << " [label=\"Árvore Vazia\", shape=plaintext];\n";
    bufferDot << "  }\n\n";
}

// --- TREAP ---
void VisualizacaoArvore::exportarSubgrafoTreap(Treap::No* no, std::stringstream& ss, int noDestaque, const std::string& corDestaque) {
    if (no == nullptr) return;
    std::string estilo = "";
    if (no->dado == noDestaque) {
        estilo = ", style=filled, fillcolor=\"" + corDestaque + "\", penwidth=2.0";
    }
    ss << "    \"" << no->dado << "_p" << contadorPassos << "\" [label=\"" << no->dado << "\\nP:" << no->prioridade << "\"" << estilo << "];\n";

    if (no->esquerda != nullptr || no->direita != nullptr) {
        if (no->esquerda != nullptr) {
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"" << no->esquerda->dado << "_p" << contadorPassos << "\";\n";
            exportarSubgrafoTreap(no->esquerda, ss, noDestaque, corDestaque);
        } else {
            ss << "    \"null_l_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"null_l_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
        }
        if (no->direita != nullptr) {
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"" << no->direita->dado << "_p" << contadorPassos << "\";\n";
            exportarSubgrafoTreap(no->direita, ss, noDestaque, corDestaque);
        } else {
            ss << "    \"null_r_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
            ss << "    \"" << no->dado << "_p" << contadorPassos << "\" -> \"null_r_" << no->dado << "_p" << contadorPassos << "\" [style=invis];\n";
        }
    }
}

void VisualizacaoArvore::registrarPassoTreap(Treap::No* raiz, const std::string& descricao, int noDestaque, const std::string& corDestaque) {
    contadorPassos++;
    bufferDot << "  subgraph cluster_passo_" << contadorPassos << " {\n";
    bufferDot << "    label=\"Passo " << contadorPassos << ": " << descricao << "\";\n";
    bufferDot << "    color=\"#cbd5e1\"; style=\"rounded\";\n";
    if (raiz != nullptr) exportarSubgrafoTreap(raiz, bufferDot, noDestaque, corDestaque);
    else bufferDot << "    empty_p" << contadorPassos << " [label=\"Árvore Vazia\", shape=plaintext];\n";
    bufferDot << "  }\n\n";
}

// --- TRIE ---
void VisualizacaoArvore::exportarSubgrafoTrie(Trie::No* no, Trie::No* raizRef, std::stringstream& ss, Trie::No* noDestaque, const std::string& corDestaque) {
    if (no == nullptr) return;

    ss << "    \"" << no << "_p" << contadorPassos << "\" [label=\"";
    if (no == raizRef) ss << "raiz";
    ss << "\", shape=" << (no->fim ? "doublecircle" : "circle");

    if (no == noDestaque) {
        ss << ", style=filled, fillcolor=\"" << corDestaque << "\", penwidth=2.0";
    } else if (no->fim) {
        ss << ", style=filled, fillcolor=lightblue";
    }
    ss << "];\n";

    for (int i = 0; i < Trie::ALFABETO; i++) {
        if (no->filhos[i] != nullptr) {
            char letra = 'a' + i;
            ss << "    \"" << no << "_p" << contadorPassos << "\" -> \"" << no->filhos[i] << "_p" << contadorPassos << "\" [label=\"" << letra << "\"];\n";
            exportarSubgrafoTrie(no->filhos[i], raizRef, ss, noDestaque, corDestaque);
        }
    }
}

void VisualizacaoArvore::registrarPassoTrie(Trie::No* raiz, const std::string& descricao, Trie::No* noDestaque, const std::string& corDestaque) {
    contadorPassos++;
    bufferDot << "  subgraph cluster_passo_" << contadorPassos << " {\n";
    bufferDot << "    label=\"Passo " << contadorPassos << ": " << descricao << "\";\n";
    bufferDot << "    color=\"#cbd5e1\"; style=\"rounded\";\n";
    if (raiz != nullptr) exportarSubgrafoTrie(raiz, raiz, bufferDot, noDestaque, corDestaque);
    else bufferDot << "    empty_p" << contadorPassos << " [label=\"Árvore Vazia\", shape=plaintext];\n";
    bufferDot << "  }\n\n";
}

// --- PATRICIA ---
void VisualizacaoArvore::exportarSubgrafoPatricia(Patricia::No* no, Patricia::No* raizRef, std::stringstream& ss, Patricia::No* noDestaque, const std::string& corDestaque) {
    if (no == nullptr) return;

    ss << "    \"" << no->idNo << "_p" << contadorPassos << "\" [label=\"";
    if (no == raizRef && no->prefixo.empty()) ss << "raiz";
    else ss << no->prefixo;
    ss << "\", shape=" << (no->fim ? "doublecircle" : "circle");

    if (no == noDestaque) {
        ss << ", style=filled, fillcolor=\"" << corDestaque << "\", penwidth=2.0";
    } else if (no->fim) {
        ss << ", style=filled, fillcolor=lightblue";
    }
    ss << "];\n";

    for (int i = 0; i < Patricia::ALFABETO; i++) {
        if (no->filhos[i] != nullptr) {
            ss << "    \"" << no->idNo << "_p" << contadorPassos << "\" -> \"" << no->filhos[i]->idNo << "_p" << contadorPassos 
               << "\" [label=\"" << no->filhos[i]->prefixo << "\"];\n";
            exportarSubgrafoPatricia(no->filhos[i], raizRef, ss, noDestaque, corDestaque);
        }
    }
}

void VisualizacaoArvore::registrarPassoPatricia(Patricia::No* raiz, const std::string& descricao, Patricia::No* noDestaque, const std::string& corDestaque) {
    contadorPassos++;
    bufferDot << "  subgraph cluster_passo_" << contadorPassos << " {\n";
    bufferDot << "    label=\"Passo " << contadorPassos << ": " << descricao << "\";\n";
    bufferDot << "    color=\"#cbd5e1\"; style=\"rounded\";\n";
    if (raiz != nullptr) exportarSubgrafoPatricia(raiz, raiz, bufferDot, noDestaque, corDestaque);
    else bufferDot << "    empty_p" << contadorPassos << " [label=\"Árvore Vazia\", shape=plaintext];\n";
    bufferDot << "  }\n\n";
}

// --- KD-TREE ---
void VisualizacaoArvore::exportarSubgrafoKdTree(KdTree::No* no, std::stringstream& ss, KdTree::No* noDestaque, const std::string& corDestaque) {
    if (no == nullptr) return;

    std::string rotuloPonto = "(";
    for (size_t i = 0; i < no->ponto.coordenadas.size(); i++) {
        rotuloPonto += std::to_string(no->ponto.coordenadas[i]);
        if (i + 1 < no->ponto.coordenadas.size()) rotuloPonto += ", ";
    }
    rotuloPonto += ")";

    std::string estilo = "";
    if (no == noDestaque) {
        estilo = ", style=filled, fillcolor=\"" + corDestaque + "\", penwidth=2.0";
    } else {
        estilo = ", style=filled, fillcolor=lightgray";
    }

    ss << "    \"" << no << "_p" << contadorPassos << "\" [label=\"" << rotuloPonto << "\\nEixo: " << no->eixo << "\"" << estilo << "];\n";

    if (no->esquerda != nullptr) {
        ss << "    \"" << no << "_p" << contadorPassos << "\" -> \"" << no->esquerda << "_p" << contadorPassos << "\" [label=\"L\"];\n";
        exportarSubgrafoKdTree(no->esquerda, ss, noDestaque, corDestaque);
    } else {
        ss << "    nullL" << no << "_p" << contadorPassos << " [shape=point];\n";
        ss << "    \"" << no << "_p" << contadorPassos << "\" -> nullL" << no << "_p" << contadorPassos << ";\n";
    }

    if (no->direita != nullptr) {
        ss << "    \"" << no << "_p" << contadorPassos << "\" -> \"" << no->direita << "_p" << contadorPassos << "\" [label=\"R\"];\n";
        exportarSubgrafoKdTree(no->direita, ss, noDestaque, corDestaque);
    } else {
        ss << "    nullR" << no << "_p" << contadorPassos << " [shape=point];\n";
        ss << "    \"" << no << "_p" << contadorPassos << "\" -> nullR" << no << "_p" << contadorPassos << ";\n";
    }
}

void VisualizacaoArvore::registrarPassoKdTree(KdTree::No* raiz, const std::string& descricao, KdTree::No* noDestaque, const std::string& corDestaque) {
    contadorPassos++;
    bufferDot << "  subgraph cluster_passo_" << contadorPassos << " {\n";
    bufferDot << "    label=\"Passo " << contadorPassos << ": " << descricao << "\";\n";
    bufferDot << "    color=\"#cbd5e1\"; style=\"rounded\";\n";
    if (raiz != nullptr) exportarSubgrafoKdTree(raiz, bufferDot, noDestaque, corDestaque);
    else bufferDot << "    empty_p" << contadorPassos << " [label=\"Árvore Vazia\", shape=plaintext];\n";
    bufferDot << "  }\n\n";
}

// --- FINALIZAÇÃO ---
bool VisualizacaoArvore::exportarEGerarImagem(const std::string& caminhoDot, const std::string& caminhoImagem) {
    bufferDot << "}\n";
    std::ofstream arquivo(caminhoDot);
    if (!arquivo.is_open()) return false;
    arquivo << bufferDot.str();
    arquivo.close();

    std::string comando = "dot -Tpng " + caminhoDot + " -o " + caminhoImagem;
    return (std::system(comando.c_str()) == 0);
}