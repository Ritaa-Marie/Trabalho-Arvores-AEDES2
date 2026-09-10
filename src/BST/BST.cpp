#include "BST.hpp"
#include <iostream>
#include <fstream>

using namespace std;

BST::BST(): num_comparacoes(0), consumo_memoria(0), tempo_execucao(0), ultima_operacao_sucesso(false), raiz(nullptr){}

BST::~BST(){
    destruirBST(this->raiz);
}

long long BST::getNumComparacoes(){
    return this->num_comparacoes;
}

long long BST::getConsumoMemoria(){
    return this->consumo_memoria;
}

long long BST::getTempoExecucao(){
    return this->tempo_execucao;
}

void BST::setNumComparacoes(long long num){
    if(num >= 0){
        this->num_comparacoes = num;
    }
}

void BST::setConsumoMemoria(long long num){
    this->consumo_memoria = num;
}

void BST::setTempoExecucao(long long num){
    this->tempo_execucao = num;
}

BST::No* BST::inserirRecursivamente(No *no, int elemento){
    if(no == nullptr){
        No *noNovo = new No(elemento);
        this->ultima_operacao_sucesso = true;
        return noNovo;
    }
 
    this->num_comparacoes++;
    if(elemento < no->dado){
        no->esquerda = inserirRecursivamente(no->esquerda, elemento);
        return no;
    } 

    this->num_comparacoes++;
    if(elemento > no->dado) {
        no->direita = inserirRecursivamente(no->direita, elemento);
        return no;
    }

    this->ultima_operacao_sucesso = false;
    return no;
}

BST::No* BST::buscarSucessor(No *no){
    if(no == nullptr){
        return nullptr;
    }

    if(no->esquerda == nullptr){
        return no;
    } else {
        return buscarSucessor(no->esquerda);
    }
}

BST::No* BST::deletarRecursivamente(No *no, int elemento){
    if(no == nullptr){
        this->ultima_operacao_sucesso = false;
        return nullptr;
    }

    this->num_comparacoes++;
    if(elemento < no->dado){
        no->esquerda = deletarRecursivamente(no->esquerda, elemento);
    } else {
        this->num_comparacoes++;
        if(elemento > no->dado){
            no->direita = deletarRecursivamente(no->direita, elemento);
        } else {
            if(no->esquerda == nullptr && no->direita == nullptr){
                delete no;
                this->ultima_operacao_sucesso = true;
                no = nullptr;
                return nullptr;
            } else if(no->esquerda != nullptr && no->direita == nullptr){
                No *novo = no->esquerda;
                delete no;
                this->ultima_operacao_sucesso = true;
                no = novo;
                return novo;
            } else if(no->direita != nullptr && no->esquerda == nullptr){
                No *novo = no->direita;
                delete no;
                this->ultima_operacao_sucesso = true;
                no = novo;
                return novo;
            } else {
                No *sucessor = buscarSucessor(no->direita);
                no->dado = sucessor->dado;
                no->direita = deletarRecursivamente(no->direita, sucessor->dado);
                this->ultima_operacao_sucesso = true;
                return no;
            }
        }
    }
    return no;
}

BST::No* BST::buscarRecursivamente(No *no, int elemento){
    if(no == nullptr){
        return nullptr;
    }

    this->num_comparacoes++;
    if(elemento < no->dado){
        return buscarRecursivamente(no->esquerda, elemento);
    } 

    this->num_comparacoes++;
    if(elemento > no->dado){
        return buscarRecursivamente(no->direita, elemento);
    } else {
        return no;
    }
}

void BST::destruirBST(No *no){
    if(no == nullptr){
        return;
    }
    
    destruirBST(no->esquerda);
    destruirBST(no->direita);

    delete no;
}

void BST::exibirInOrdemRecursivamente(No *no){
    if(no != nullptr){
        exibirInOrdemRecursivamente(no->esquerda);
        cout <<  no->dado  << endl;
        exibirInOrdemRecursivamente(no->direita);
    }

}

void BST::gerarDOTRecursivo(No *no, std::ofstream& arquivo){
    if(no == nullptr){
        return;
    }

    if(no->esquerda != nullptr || no->direita != nullptr){
        if(no->esquerda != nullptr){
            arquivo << "  " << no->dado << " -> " << no->esquerda->dado << ";\n";
            gerarDOTRecursivo(no->esquerda, arquivo);
        } else {
            arquivo << "  null_l_" << no->dado << "[style=invis];\n";
            arquivo << "   " << no->dado << " ->  null_l_" << no->dado << "[style=invis];\n";
        }

        if(no->direita != nullptr){
            arquivo << "  " << no->dado << " -> " << no->direita->dado << ";\n";
            gerarDOTRecursivo(no->direita, arquivo);
        } else {
            arquivo << "  null_r_" << no->dado << "[style=invis];\n";
            arquivo << "   " << no->dado << " ->  null_r_" << no->dado << "[style=invis];\n";
        }
    }
}

void BST::inserirElemento(int elemento){
    this->raiz = inserirRecursivamente(this->raiz, elemento);
    if(this->ultima_operacao_sucesso){
        cout << "Elemento inserido com sucesso" <<endl;
    } else {
        cout << "Esse elemento já existe na árvore" <<endl;
    }
}

void BST::deletarElemento(int elemento){
    this->raiz = deletarRecursivamente(this->raiz, elemento);
    if(this->ultima_operacao_sucesso){
        cout << "Elemento deletado com sucesso" <<endl;
    } else {
        cout << "Erro ao deletar elemento" <<endl;
    }
}

bool BST::buscarElemento(int elemento){
    No *no = buscarRecursivamente(this->raiz, elemento);
    if(no == nullptr){
        return false;
    } else {
        return true;
    }
}

void BST::exibirBSTInOrdem(){
    if(this->raiz == nullptr){
        cout<< "A árvore está vazia" << endl;
        return;
    }

    exibirInOrdemRecursivamente(this->raiz);
}

void BST::gerarDOT(const std::string& caminho){
    ofstream arquivo(caminho);

    if(!arquivo.is_open()){
        cerr << "Erro ao abrir arquivo da exportação do DOT.\n";
        return;
    }

    arquivo << "digraph BST {\n";
    arquivo <<  "   node [shape=circle];\n";

    if(this->raiz != nullptr){
        if(this->raiz->esquerda == nullptr && this->raiz->direita == nullptr){
            arquivo << "  " << this->raiz->dado << ";\n";
        } else {
            gerarDOTRecursivo(this->raiz, arquivo);
        }
    }

    arquivo << "}\n";
    arquivo.close();

}