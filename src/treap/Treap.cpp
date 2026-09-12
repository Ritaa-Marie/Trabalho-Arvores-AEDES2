#include "Treap.hpp"

using namespace std;

Treap::Treap(): num_comparacoes(0), consumo_memoria(0), tempo_execucao(0), num_rotacoes(0), ultima_operacao_sucesso(false), 
                raiz(nullptr), geradorBinario(std::random_device{}()), distribuicaoInteiro(0, UINT64_MAX){}

Treap::~Treap(){
    destruirTreap(this->raiz);
}

uint64_t Treap::gerarPrioridade(){
    return this->distribuicaoInteiro(geradorBinario);
}

Treap::No* Treap::inserirRecursivamente(No *no, int elemento){
    if(no == nullptr){
        uint64_t prioridade = gerarPrioridade();
        No *novo = new No(elemento, prioridade);
        return novo;
    }

    if(elemento < no->dado){
        no->esquerda = inserirRecursivamente(no->esquerda, elemento);
        if(no->esquerda != nullptr && no->esquerda->prioridade > no->prioridade){
            return rotacaoDireita(no);
        }
    } else if(elemento > no->dado){
        no->direita = inserirRecursivamente(no->direita, elemento);
        if(no->direita != nullptr && no->direita->prioridade > no->prioridade){
            return rotacaoEsquerda(no);
        }
    }

    return no;
}

Treap::No* Treap::deletarRecursivamente(No *no, int elemento){
    if(no == nullptr){
        return no;
    }

    if(elemento < no->dado){
        no->esquerda = deletarRecursivamente(no->esquerda, elemento);
    } else if(elemento > no->dado){
        no->direita = deletarRecursivamente(no->direita, elemento);
    } else {
        if(no->direita == nullptr && no->esquerda == nullptr){
            delete no;
            return nullptr;
        } else if(no->esquerda == nullptr) {
            No *novaRaiz = rotacaoEsquerda(no);
            novaRaiz->esquerda = deletarRecursivamente(novaRaiz->esquerda, elemento);
            return novaRaiz;
        } else if(no->direita == nullptr){
            No *novaRaiz =  rotacaoDireita(no);
            novaRaiz->direita = deletarRecursivamente(novaRaiz->direita, elemento);
            return novaRaiz;
        } else {
            if(no->esquerda->prioridade > no->direita->prioridade){
                No *novaRaiz =  rotacaoDireita(no);
                novaRaiz->direita = deletarRecursivamente(novaRaiz->direita, elemento);
                return novaRaiz;
            } else {
                No *novaRaiz = rotacaoEsquerda(no);
                novaRaiz->esquerda = deletarRecursivamente(novaRaiz->esquerda, elemento);
                return novaRaiz;
            }
        }
    }

    return no;
}

Treap::No* Treap::buscarRecursivamente(No *no, int elemento){
    if(no == nullptr){
        return nullptr;
    }

    if(elemento < no->dado){
        return buscarRecursivamente(no->esquerda, elemento);
    } else if(elemento > no->dado){
        return buscarRecursivamente(no->direita, elemento);
    }

    return no;
}

void Treap::destruirTreap(No *no){
    if(no == nullptr){
        return;
    }

    destruirTreap(no->esquerda);
    destruirTreap(no->direita);
    delete no;
}

void Treap::exibirInOrdemRecursivamente(No *no){
    if(no != nullptr){
        exibirInOrdemRecursivamente(no->esquerda);
        cout << no->dado << endl;
        exibirInOrdemRecursivamente(no->direita);
    }
}

Treap::No* Treap::rotacaoDireita(No *no){
    if(no == nullptr || no->esquerda == nullptr){
        return no;
    }

    No *novaRaiz = no->esquerda;
    No *filhoDireitaOrfao = novaRaiz->direita;

    novaRaiz->direita = no;
    no->esquerda = filhoDireitaOrfao;

    this->num_rotacoes++;
    return novaRaiz;
}

Treap::No* Treap::rotacaoEsquerda(No *no){
    if(no == nullptr || no->direita == nullptr){
        return no;
    }

    No *novaRaiz = no->direita;
    No *filhoEsquerdoOrfao = novaRaiz->esquerda;

    novaRaiz->esquerda = no;
    no->direito = filhoEsquerdoOrfao;

    this->num_rotacoes++;
    return novaRaiz;
}

int Treap::calcularAlturaRecursivamente(No *no){
    if(no == nullptr){
        return 0;
    }

    int tamDireita = calcularAlturaRecursivamente(no->direita);
    int tamEsquerda = calcularAlturaRecursivamente(no->esquerda);

    return (1 +  max(tamDireita, tamEsquerda));
}

void Treap::gerarDOTRecursivo(No *no, std::ofstream& arquivo){
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

long long Treap::getNumComparacoes(){
    return this->num_comparacoes;
}

long long Treap::getConsumoMemoria(){
    return this->consumo_memoria;
}

long long Treap::getTempoExecucao(){
    return this->tempo_execucao;
}

long long Treap::getNumRotacoes(){
    return this->num_rotacoes;
}

bool Treap::getUltimaOperacaoSucesso(){
    return this->ultima_operacao_sucesso;
}

void Treap::setNumComparacoes(long long num){
    if(num >= 0){
        this->num_comparacoes = num;
    }
}

void Treap::setConsumoMemoria(long long num){
    this->consumo_memoria = num;
}

void Treap::setTempoExecucao(long long num){
    this->tempo_execucao = num;
}

void Treap::setNumRotacoes(long long num){
    this->num_rotacoes = num;
}

void Treap::setUltimaOperacaoSucesso(bool status){
    this->ultima_operacao_sucesso = status;
}

void Treap::inserirElemento(int elemento){
    this->raiz = inserirRecursivamente(this->raiz, elemento);

    if(this->ultima_operacao_sucesso){
        cout << "Elemento inserido na árvore Treap com sucesso" << endl;
    } else {
        cout << "Esse elemento já existe na árvore Treap" << endl;
    }
}

bool Treap::buscarElemento(int elemento){
    No *aux = buscarRecursivamente(this->raiz, elemento);
    if(aux == nullptr){
        return false;
    } else {
        return true;
    }
}
void Treap::deletarElemento(int elemento){
    this->raiz = deletarRecursivamente(this->raiz, elemento);
    if(ultima_operacao_sucesso){
        cout << "Elemento deletado da Treap com sucesso" <<endl;
    } else {
        cout << "Erro ao deletar elemento da Treap" <<endl;
    }
}

void Treap::exibirTreapInOrdem(){
    if(this->raiz == nullptr){
        cout << "A árvore Treap está vazia" << endl; 
        return;
    }

    exibirInOrdemRecursivamente(this->raiz);
}

int Treap::calcularAlturaTreap(){
    return calcularAlturaRecursivamente(this->raiz);
}

void Treap::gerarDOT(const std::string& caminho){
    ofstream arquivo(caminho);

    if(!arquivo.is_open()){
        cerr << "Erro ao abrir arquivo da exportação do DOT da Treap.\n";
        return;
    }

    arquivo << "digraph Treap {\n";
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
