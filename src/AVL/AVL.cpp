#include "AVL.hpp"

using namespace std;

AVL::AVL(): num_comparacoes(0), consumo_memoria(0), tempo_execucao(0), num_rotacoes(0), ultima_operacao_sucesso(false), raiz(nullptr) {}

AVL::~AVL(){
    destruirAVL(this->raiz);
}

AVL::No* AVL::getRaiz() const { 
        return raiz; 
}

long long AVL::getConsumoMemoria(){
    return this->consumo_memoria;
}

long long AVL::getNumComparacoes(){
    return this->num_comparacoes;
}

long long AVL::getTempoExecucao(){
    return this->tempo_execucao;
}

long long AVL::getNumRotacoes(){
    return this->num_rotacoes;
}

bool AVL::getUltimaOperacaoSucesso(){
    return this->ultima_operacao_sucesso;
}

void AVL::setConsumoMemoria(long long num){
    this->consumo_memoria = num;
}

void AVL::setNumComparacoes(long long num){
    if(num >= 0){
        this->num_comparacoes = num;
    }
}

void AVL::setTempoExecucao(long long num){
    this->tempo_execucao = num;
}

void AVL::setNumRotacoes(long long num){
    if(num >=0){
        this->num_rotacoes = num;
    }
}

void AVL::setUltimaOperacaoSucesso(bool status){
    this->ultima_operacao_sucesso = status;
}

AVL::No* AVL::inserirRecursivamente(No *no, int elemento){
    if(no == nullptr){
        No *novo = new No(elemento);
        this->ultima_operacao_sucesso = true;
        this->consumo_memoria += sizeof(No);
        return novo;
    }

    this->num_comparacoes++;
    if(no->dado > elemento){
        no->esquerda = inserirRecursivamente(no->esquerda, elemento);
    } else if(no->dado < elemento){
        this->num_comparacoes++;
        no->direita = inserirRecursivamente(no->direita, elemento);
    } else {
        this->ultima_operacao_sucesso = false;
        return no;
    }

    return balanceamentoAVL(no);
}

AVL::No* AVL::deletarRecursivamente(No *no, int elemento){
    if(no == nullptr){
        this->ultima_operacao_sucesso = false;
        return no;
    }

    this->num_comparacoes++;
    if(no->dado > elemento){
        no->esquerda = deletarRecursivamente(no->esquerda, elemento);
    } else if(no->dado < elemento){
        this->num_comparacoes++;
        no->direita = deletarRecursivamente(no->direita, elemento);
    } else {
        this->ultima_operacao_sucesso = true;
        this->num_comparacoes++;
        if(no->direita == nullptr && no->esquerda == nullptr){
            this->consumo_memoria -= sizeof(No);
            delete no;
            return nullptr;
        } else if(no->direita == nullptr){
            this->num_comparacoes++;
            No *novo = no->esquerda;
            this->consumo_memoria -= sizeof(No);
            delete no;
            return balanceamentoAVL(novo);
        } else if(no->esquerda == nullptr){
            this->num_comparacoes++;
            No *novo = no->direita;
            this->consumo_memoria -= sizeof(No);
            delete no;
            return balanceamentoAVL(novo);
        } else {
            No *sucessor = buscarSucessor(no->direita);
            no->dado = sucessor->dado;
            no->direita = deletarRecursivamente(no->direita, sucessor->dado);
        }
    }

    return balanceamentoAVL(no);
}

AVL::No* AVL::buscarRecursivamente(No *no, int elemento){
    if(no == nullptr){
        return nullptr;
    }

    this->num_comparacoes++;
    if(elemento > no->dado){
        return buscarRecursivamente(no->direita, elemento);
    } 

    this->num_comparacoes++;
    if(elemento < no->dado){
        return buscarRecursivamente(no->esquerda, elemento);
    }

    return no;
}

void AVL::destruirAVL(No *no){
    if(no == nullptr){
        return;
    }

    destruirAVL(no->esquerda);
    destruirAVL(no->direita);
    delete no;
}

void AVL::exibirInOrdemRecursivamente(No *no){
    if(no != nullptr){
        exibirInOrdemRecursivamente(no->esquerda);
        cout << no->dado << " " <<endl;
        exibirInOrdemRecursivamente(no->direita);
    }
}

AVL::No* AVL::buscarSucessor(No *no){
    if(no == nullptr){
        return nullptr;
    }

    this->num_comparacoes++;
    if(no->esquerda == nullptr){
        return no;
    }

    return buscarSucessor(no->esquerda);
}

void AVL::calcularAlturaNo(No *no){
    if(no == nullptr){
        return;
    }

    if(no->esquerda == nullptr && no->direita == nullptr){
        no->h = 0;
        return;
    }

    if(no->esquerda == nullptr){
        no->h = 1 + max((no->direita->h), -1);
        return;
    } else if (no->direita == nullptr){
        no->h = 1 + max((no->esquerda->h), -1);
        return;
    }

    no->h = 1 + max((no->esquerda->h), (no->direita->h));
}

int AVL::calcularFatorBalanceamento(No *no){
    if(no == nullptr){
        return 0;
    }

    if(no->esquerda == nullptr && no->direita == nullptr){
        return 0;
    }

    if(no->esquerda == nullptr){
        return ((no->direita->h) - (-1) );
    }

    if(no->direita == nullptr){
        return ((-1) - (no->esquerda->h));
    }

    return ((no->direita->h) - (no->esquerda->h));
}

AVL::No* AVL::rotacaoDireita(No *no){
    if(no == nullptr || no->esquerda == nullptr){
        return  no;
    }

    this->num_rotacoes++;
    No *novaRaiz = no->esquerda;
    No *filhoDireitaQueTroca = novaRaiz->direita;
    
    novaRaiz->direita = no;
    no->esquerda = filhoDireitaQueTroca;
    
    calcularAlturaNo(no);
    calcularAlturaNo(novaRaiz);

    return novaRaiz;
}

AVL::No* AVL::rotacaoEsquerda(No *no){
    if(no == nullptr || no->direita == nullptr){
        return  no;
    }

    this->num_rotacoes++;
    No *novaRaiz = no->direita;
    No * filhoEsquerdaQueTroca = novaRaiz->esquerda;
    novaRaiz->esquerda = no;
    no->direita = filhoEsquerdaQueTroca;

    calcularAlturaNo(no);
    calcularAlturaNo(novaRaiz);

    return novaRaiz;
}

AVL::No* AVL::rotacaoDuplaDireita(No *no){
    if(no == nullptr || no->esquerda == nullptr){
        return no;
    }

    No *novaRaiz = rotacaoEsquerda(no->esquerda);
    no->esquerda = novaRaiz;
    No *novaRaizTotal = rotacaoDireita(no);

    return novaRaizTotal;
}

AVL::No* AVL::rotacaoDuplaEsquerda(No *no){
    if(no == nullptr || no->direita == nullptr){
        return no;
    }

    No *novaRaiz = rotacaoDireita(no->direita);
    no->direita = novaRaiz;
    No *novaRaizTotal = rotacaoEsquerda(no);
    return novaRaizTotal;
}

AVL::No* AVL::balanceamentoAVL(No *no){
    if(no == nullptr){
        return no;
    }

    calcularAlturaNo(no);
    int fatorBalanceamento = calcularFatorBalanceamento(no);
    if(fatorBalanceamento >= 2){
        int fatorBalanceamentoDireita = calcularFatorBalanceamento(no->direita);
        if(fatorBalanceamentoDireita < 0){
            return rotacaoDuplaEsquerda(no);
        }

        return rotacaoEsquerda(no);
    }

    if(fatorBalanceamento <= -2){
        int fatorBalanceamentoEsquerda = calcularFatorBalanceamento(no->esquerda);
        if(fatorBalanceamentoEsquerda > 0){
            return rotacaoDuplaDireita(no);
        }

        return rotacaoDireita(no);
    }

    return no;
}

void AVL::gerarDOTRecursivo(No *no, std::ofstream& arquivo){
    if(no == nullptr){
        return;
    }

    int fatorBalanceamento = calcularFatorBalanceamento(no);
    arquivo << "  " << no->dado << " [label=\"" << no->dado << "\\nFB:" << fatorBalanceamento << "\"];\n";

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

void AVL::inserirElemento(int elemento){
    this->raiz = inserirRecursivamente(this->raiz, elemento);
    /*if(this->ultima_operacao_sucesso){
        cout << "Elemento inserido na AVL com sucesso" <<endl;
    } else {
        cout << "Esse elemento já existe na árvore AVL" <<endl;
    }*/
}

bool AVL::buscarElemento(int elemento){
    No *no = buscarRecursivamente(this->raiz, elemento);
    if(no == nullptr){
        return false;
    } else {
        return true;
    }
}

void AVL::deletarElemento(int elemento){
    this->raiz = deletarRecursivamente(this->raiz, elemento);
    /*if(this->ultima_operacao_sucesso){
        cout << "Elemento deletado da AVL com sucesso" <<endl;
    } else {
        cout << "Erro ao deletar elemento da AVL" <<endl;
    }*/
}

void AVL::exibirAVLInOrdem(){
    if(this->raiz == nullptr){
        cout<< "A árvore AVL está vazia" << endl;
        return;
    }

    exibirInOrdemRecursivamente(this->raiz);
}

int AVL::calcularAlturaAVL(){
    if(this->raiz == nullptr){
        return -1;
    }

    return this->raiz->h;
}

void AVL::gerarDOT(const std::string& caminho){
    ofstream arquivo(caminho);

    if(!arquivo.is_open()){
        cerr << "Erro ao abrir arquivo da exportação do DOT da AVL.\n";
        return;
    }

    arquivo << "digraph AVL {\n";
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

