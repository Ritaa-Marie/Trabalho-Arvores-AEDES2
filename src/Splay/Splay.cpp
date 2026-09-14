#include "Splay.hpp"

using namespace std;

Splay::Splay(): num_comparacoes(0), consumo_memoria(0), tempo_execucao(0), num_rotacoes(0), ultima_operacao_sucesso(false), raiz(nullptr){}

Splay::~Splay(){
    destruirSplay(this->raiz);
}

Splay::No* Splay::inserirRecursivamente(No *no, int elemento, No* &noInserido){
    if(no == nullptr){
        No *novo = new No(elemento);
        this->ultima_operacao_sucesso = true;
        noInserido = novo;
        return novo;
    }

    if(elemento < no->dado){
        No *noEsquerda = inserirRecursivamente(no->esquerda, elemento, noInserido);
        no->esquerda = noEsquerda;
        noEsquerda->pai = no;
    } else if(elemento > no->dado){
        No *noDireita = inserirRecursivamente(no->direita, elemento, noInserido);
        no->direita = noDireita;
        noDireita->pai = no;
    } else {
        this->ultima_operacao_sucesso = false;
        noInserido = no;
    }

    return no;
}

Splay::No* Splay::deletar(No *no, int elemento){
    No *alvo = buscarRecursivamente(no, elemento);
    if(alvo == nullptr){
        this->ultima_operacao_sucesso = false;
        return this->raiz;
    }

    splay(alvo);

    if(this->raiz == nullptr || this->raiz->dado != elemento){
        this->ultima_operacao_sucesso = false;
        return this->raiz;
    } 

    No *noDeletar = this->raiz;
    if(this->raiz->direita == nullptr){
        this->raiz = this->raiz->esquerda;
        if(raiz != nullptr){
            this->raiz->pai = nullptr;
        }
    } else if(this->raiz->esquerda == nullptr){
        this->raiz = this->raiz->direita;
        if(raiz != nullptr){
            this->raiz->pai = nullptr;
        }
    } else {
        No *subArvoreDireita = this->raiz->direita;
        No *subArvoreEsquerda = this->raiz->esquerda;
        subArvoreDireita->pai = nullptr;
        subArvoreEsquerda->pai = nullptr;
         
        this->raiz = subArvoreEsquerda;
        No *predecessor = buscarPredecessor(subArvoreEsquerda);

        if(predecessor != nullptr){
            splay(predecessor);
            this->raiz->direita = subArvoreDireita;
            subArvoreDireita->pai = this->raiz;
        } else {
            this->raiz = subArvoreDireita;
        }
    }

    delete noDeletar;
    this->ultima_operacao_sucesso = true;
    return this->raiz;
}


Splay::No* Splay::buscarRecursivamente(No *no, int elemento){
    if(no == nullptr){
        return nullptr;
    }

    this->num_comparacoes++;
    if(no->dado > elemento){
        if(no->esquerda == nullptr){
            return no;
        }
        return buscarRecursivamente(no->esquerda, elemento);
    } else if(no->dado < elemento){
        this->num_comparacoes++;
        if(no->direita == nullptr){
            return no;
        }
        return buscarRecursivamente(no->direita, elemento);
    }

    return no;
}

void Splay::destruirSplay(No *no){
    if(no == nullptr){
        return;
    }

    destruirSplay(no->esquerda);
    destruirSplay(no->direita);
    delete no;
}

void Splay::exibirInOrdemRecursivamente(No *no){
    if(no != nullptr){
        exibirInOrdemRecursivamente(no->esquerda);
        cout << no->dado << endl;
        exibirInOrdemRecursivamente(no->direita);
    }
}

// rotação simples para a direita
void Splay::zig(No *no){
    if(no == nullptr || no->pai == nullptr){
        return;
    }

    No *pai = no->pai;
    No *avo = pai->pai;
    No *netoDireita = no->direita;

    no->direita = pai;
    pai->esquerda = netoDireita;

    if(avo != nullptr){
        no->pai = avo;
        if(avo->esquerda == pai){
            avo->esquerda = no;
        } else {
            avo->direita = no;
        }
    } else {
        this->raiz = no;
        no->pai = nullptr;
    }

    pai->pai = no;

    if(netoDireita != nullptr){
        netoDireita->pai = pai;
    }
    
    this->num_rotacoes++;
}

// rotação simples para a esquerda
void Splay::zag(No *no){
    if(no == nullptr || no->pai == nullptr){
        return;
    }

    No *pai = no->pai;
    No *avo = pai->pai;
    No *netoEsquerda = no->esquerda;

    no->esquerda = pai;
    pai->direita = netoEsquerda;

    if(netoEsquerda != nullptr){
        netoEsquerda->pai = pai;
    }

    pai->pai = no;

    if(avo != nullptr){
        no->pai = avo;
        if(avo->esquerda == pai){
            avo->esquerda = no;
        } else {
            avo->direita = no;
        }
    } else {
        this->raiz = no;
        no->pai = nullptr;
    }

    this->num_rotacoes++;
}

Splay::No* Splay::buscarPredecessor(No *no){
    if(no == nullptr){
        return nullptr;
    }

    if(no->direita == nullptr){
        return no;
    }

    return buscarPredecessor(no->direita);
}

void Splay::splay(No *no){
    if(no == nullptr){
        return;
    }

    while(no->pai != nullptr){
        No *pai = no->pai;
        No *avo = pai->pai;

        if(avo == nullptr){
            if(pai->esquerda == no){
                zig(no);
            } else {
                zag(no);
            }
        } else if(avo->esquerda == pai && pai->esquerda == no){
            zig(pai);
            zig(no);
        } else if(avo->direita == pai && pai->direita == no){
            zag(pai);
            zag(no);
        } else if(avo->esquerda == pai && pai->direita == no){
            zag(no);
            zig(no);
        } else if(avo->direita == pai && pai->esquerda == no){
            zig(no);
            zag(no);
        }
    }

    if(no->pai == nullptr){
        this->raiz = no;
    }
}

void Splay::gerarDOTRecursivo(No *no, std::ofstream& arquivo){
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

int Splay::calcularAlturaRecursivamente(No *no){
    if(no == nullptr){
        return -1;
    }

    int alturaDir = calcularAlturaRecursivamente(no->direita);
    int alturaEsq = calcularAlturaRecursivamente(no->esquerda);

    return (1 + max(alturaDir, alturaEsq));
}

long long Splay::getNumComparacoes(){
    return this->num_comparacoes;
}

long long Splay::getConsumoMemoria(){
    return this->consumo_memoria;
}

long long Splay::getTempoExecucao(){
    return this->tempo_execucao;
}

long long Splay::getNumRotacoes(){
    return this->num_rotacoes;
}

bool Splay::getUltimaOperacaoSucesso(){
    return this->ultima_operacao_sucesso;
}

int Splay::getRaizValor(){
    if (this->raiz == nullptr) {
        throw runtime_error("Erro: Tentativa de acessar a raiz de uma árvore vazia!");
    }

    return this->raiz->dado;
}

void Splay::setNumComparacoes(long long num){
    if(num >= 0){
        this->num_comparacoes = num;
    }
}

void Splay::setConsumoMemoria(long long num){
    this->consumo_memoria = num;
}

void Splay::setTempoExecucao(long long num){
    this->tempo_execucao = num;
}

void Splay::setNumRotacoes(long long num){
    this->num_rotacoes = num;
}

void Splay::setUltimaOperacaoSucesso(bool status){
    this->ultima_operacao_sucesso = status;
}

void Splay::inserirElemento(int elemento){
    No *noInserido = nullptr;
    this->ultima_operacao_sucesso = false;

    this->raiz = inserirRecursivamente(this->raiz, elemento, noInserido);

    if(noInserido != nullptr){
        splay(noInserido);
    }

    /*if(this->ultima_operacao_sucesso){
        cout << "Elemento inserido na árvore Splay com sucesso" << endl;
    } else {
        cout << "Esse elemento já existe na árvore Splay" << endl;
    }*/
}

bool Splay::buscarElemento(int elemento){
    if(this->raiz == nullptr){
        return false;
    }

    No *alvo = buscarRecursivamente(this->raiz, elemento);

    if(alvo == nullptr){
        return false;
    } else {
        splay(alvo);
    }

    if(this->raiz != nullptr && elemento == this->raiz->dado){
        return true;
    } else {
        return false;
    }
}

void Splay::deletarElemento(int elemento){
    this->ultima_operacao_sucesso = false;
    
    this->raiz = deletar(this->raiz, elemento);
    
    if(ultima_operacao_sucesso){
        cout << "Elemento deletado da Splay com sucesso" <<endl;
    } else {
        cout << "Erro ao deletar elemento da Splay" <<endl;
    }
}

void Splay::exibirSplayInOrdem(){
    if(this->raiz == nullptr){
        cout << "A árvore Splay está vazia" << endl; 
        return;
    }

    exibirInOrdemRecursivamente(this->raiz);
}

int Splay::alturaSplay(){
    return calcularAlturaRecursivamente(this->raiz);
}

void Splay::gerarDOT(const std::string& caminho){
    ofstream arquivo(caminho);

    if(!arquivo.is_open()){
        cerr << "Erro ao abrir arquivo da exportação do DOT da Splay.\n";
        return;
    }

    arquivo << "digraph Splay {\n";
    arquivo <<  "   node [shape=circle];\n";

    if(this->raiz != nullptr){
        if(this->raiz->esquerda == nullptr && this->raiz->direita == nullptr){
            arquivo << "  " << this->raiz->dado << " [label=\"" << this->raiz->dado << "\"];\n";
        } else {
            gerarDOTRecursivo(this->raiz, arquivo);
        }
    }

    arquivo << "}\n";
    arquivo.close();
}