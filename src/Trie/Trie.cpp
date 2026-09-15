#include "Trie.hpp"

using namespace std;

Trie::Trie(): idNoAtual(0), num_comparacoes(0), consumo_memoria(0), tempo_execucao(0), ultima_operacao_sucesso(false), raiz(nullptr){}

Trie::~Trie(){
    destruirTrie(this->raiz);
    raiz = nullptr;
}

Trie::No* Trie::criarNo(){
    this->idNoAtual++;
    No *no = new No(this->idNoAtual);
    this->consumo_memoria += sizeof(No);
    return no;
}

void Trie::transformarMinusculo(std::string& palavra){
    for(char &letra: palavra){
        letra = tolower(letra);
    }
}

void Trie::substituirCaracteresEspeciais(std::string& palavra){
    vector<string> caracteresComAcento = {
        "ç", "Ç", 
        "á", "à", "ã", "â", "Á", "À", "Ã", "Â",
        "é", "ê", "É", "Ê", 
        "í", "Í", 
        "ó", "õ", "ô", "Ó", "Õ", "Ô", 
        "ú", "Ú", "ü",
        "R$"
    };

    vector<string> caracteresSubstituir = {
        "c", "c", 
        "a", "a", "a", "a", "a", "a", "a", "a",
        "e", "e", "e", "e", 
        "i", "i", 
        "o", "o", "o", "o", "o", "o", 
        "u", "u", "u",
        ""
    };

    for(size_t i=0;i<caracteresComAcento.size();i++){
        size_t posicao = palavra.find(caracteresComAcento[i]);

        while (posicao != std::string::npos){
            palavra.replace(posicao, caracteresComAcento[i].length(), caracteresSubstituir[i]);
            posicao = palavra.find(caracteresComAcento[i], posicao + caracteresSubstituir[i].length());
        }
    }
}

std::string Trie::normalizarPalavra(const std::string& palavra){
    string resultado = palavra;
    substituirCaracteresEspeciais(resultado);
    transformarMinusculo(resultado);

    return resultado;
}

Trie::No* Trie::inserirRecursivamente(No *no, const std::string& palavra, size_t caractere){
    if(no == nullptr){
        no = criarNo();
    }

    this->num_comparacoes++;
    if(palavra.length() == caractere){
        this->num_comparacoes++;
        if(no->fim){
            this->ultima_operacao_sucesso = false;
        } else {
            this->ultima_operacao_sucesso = true;
            no->fim = true;
        }
        
        return no;
    }

    int indiceLetra = palavra[caractere] - 'a';
    this->num_comparacoes++;
    if(indiceLetra < 0  || indiceLetra >= ALFABETO){
        this->ultima_operacao_sucesso = false;
        return no;
    }

    no->filhos[indiceLetra] = inserirRecursivamente(no->filhos[indiceLetra], palavra, caractere + 1);
    
    return no;
}

Trie::No* Trie::deletarRecursivamente(No *no, const std::string& palavra, size_t caractere){
    if(no == nullptr){
        this->ultima_operacao_sucesso = false;
        return no;
    }

    this->num_comparacoes++;
    if(palavra.length() == caractere){
        this->num_comparacoes++;
        if(!no->fim){
            this->ultima_operacao_sucesso = false;
            return no;
        } else {
            no->fim = false;
            this->ultima_operacao_sucesso = true;
            
            bool temFilho = false;
            for(int i=0;i<ALFABETO;i++){
                this->num_comparacoes++;
                if(no->filhos[i] != nullptr){
                    temFilho = true;
                    break;
                }
            }

            this->num_comparacoes++;
            if(temFilho){
                return no;
            } else {
                this->consumo_memoria -= sizeof(No);
                delete no;
                return nullptr;
            }
        }
    }

    int indiceLetra = palavra[caractere] - 'a';
    this->num_comparacoes++;
    if(indiceLetra < 0 || indiceLetra >= ALFABETO){
        this->ultima_operacao_sucesso = false;
        return no;
    }

    no->filhos[indiceLetra] = deletarRecursivamente(no->filhos[indiceLetra], palavra, caractere + 1);

    bool temFilho = false;
    for(int i = 0; i < ALFABETO; i++){
        this->num_comparacoes++;
        if (no->filhos[i] != nullptr) {
            temFilho = true;
            break;
        }
    }

    this->num_comparacoes++;
    if(!no->fim && !temFilho){
        this->consumo_memoria -= sizeof(No);
        delete no;
        return nullptr;
    }

    return no;
}

Trie::No* Trie::buscarRecursivamente(No *no, const std::string& palavra, size_t caractere){
    if(no == nullptr){
        return nullptr;
    }

    this->num_comparacoes++;
    if(caractere == palavra.length()){
        this->num_comparacoes++;
        if(no->fim == true){
            return no;
        }
        return nullptr;
    }

    int indiceLetra = palavra[caractere] - 'a';
    this->num_comparacoes++;
    if(indiceLetra < 0 || indiceLetra >= ALFABETO){
        return nullptr;
    }

    return buscarRecursivamente(no->filhos[indiceLetra], palavra, caractere + 1);
}

void Trie::destruirTrie(No *no){
    if(no == nullptr){
        return;
    }

    for(int i=0;i<ALFABETO;i++){
        if(no->filhos[i] != nullptr){
            destruirTrie(no->filhos[i]);
            no->filhos[i] = nullptr;
        }
    }

    delete no;
}

void Trie::exibirInOrdemRecursivamente(No *no, std::string palavraFormada){
    if(no == nullptr){
        return;
    }

    if(no->fim){
        cout << palavraFormada <<endl;
    }

    for(int i=0;i<ALFABETO;i++){
        if(no->filhos[i] != nullptr){
            palavraFormada += ('a' + i);
            exibirInOrdemRecursivamente(no->filhos[i], palavraFormada);
            palavraFormada.pop_back();
        }
    }
}

int Trie::calcularAlturaRecursivamente(No *no){
    if(no == nullptr){
        return -1;
    }

    int alturaMaxima = -1;

    for(int i=0;i<ALFABETO;i++){
        if(no->filhos[i] != nullptr){
            int alturaFilho = calcularAlturaRecursivamente(no->filhos[i]);

            if(alturaFilho > alturaMaxima){
                alturaMaxima = alturaFilho;
            }
        }
    }

    return (1 + alturaMaxima);
}

void Trie::gerarDOTRecursivo(No *no, std::ofstream& arquivo) {
    if (no == nullptr) {
        return;
    }

    arquivo << "    \"" << no << "\" [label=\"";
    if (no == raiz) {
        arquivo << "raiz";
    }
    arquivo << "\", shape=" << (no->fim ? "doublecircle" : "circle");
    if (no->fim) {
        arquivo << ", style=filled, fillcolor=lightblue";
    }
    arquivo << "];\n";

    for (int i = 0; i < ALFABETO; i++) {
        if (no->filhos[i] != nullptr) {
            char letra = 'a' + i;

            arquivo << "    \"" << no << "\" -> \"" << no->filhos[i] 
                << "\" [label=\"" << letra << "\"];\n";

            gerarDOTRecursivo(no->filhos[i], arquivo);
        }
    }
}

long long Trie::getNumComparacoes(){
    return this->num_comparacoes;
}

long long Trie::getConsumoMemoria(){
    return this->consumo_memoria;
}

long long Trie::getTempoExecucao(){
    return this->tempo_execucao;
}

bool Trie::getUltimaOperacaoSucesso(){
    return this->ultima_operacao_sucesso;
}

void Trie::setNumComparacoes(long long num){
    if(num >= 0){
        this->num_comparacoes = num;
    }
}

void Trie::setConsumoMemoria(long long num){
    this->consumo_memoria = num;
}

void Trie::setTempoExecucao(long long num){
    this->tempo_execucao = num;
}

void Trie::setUltimaOperacaoSucesso(bool status){
    this->ultima_operacao_sucesso = status;
}

void Trie::inserirElemento(const std::string& palavra){
    std::string palavraTratada = normalizarPalavra(palavra);
    
    if (palavraTratada.empty()) {
        this->ultima_operacao_sucesso = false;
        cout << "Esse elemento está inválido para inserção na árvore Trie" << endl;
        return;
    }

    this->ultima_operacao_sucesso = false;
    raiz = inserirRecursivamente(raiz, palavraTratada, 0);
    
    /*if(this->ultima_operacao_sucesso){
        cout << "Elemento inserido na árvore Trie com sucesso" << endl;
    } else {
        cout << "Esse elemento já existe na árvore Trie" << endl;
    }*/
}

bool Trie::buscarElemento(const std::string& palavra){
    string palavraTratada = normalizarPalavra(palavra);

    if (palavraTratada.empty()) {
        return false;
    }

    No *aux = buscarRecursivamente(raiz, palavraTratada, 0);
    if(aux == nullptr){
        return false;
    } else {
        return true;
    }
}

void Trie::deletarElemento(const std::string& palavra){
    string palavraTratada = normalizarPalavra(palavra);

    if (palavraTratada.empty()) {
        cout << "Esse elemento está inválido para remoção na árvore Trie" << endl;
        return;
    }

    this->ultima_operacao_sucesso = false;
    raiz = deletarRecursivamente(raiz, palavraTratada, 0);
    /*if(ultima_operacao_sucesso){
        cout << "Elemento deletado da Trie com sucesso" <<endl;
    } else {
        cout << "Erro ao deletar elemento da Trie" <<endl;
    }*/
}

void Trie::exibirTrieInOrdem(){
    if(this->raiz == nullptr){
        cout << "A árvore Trie está vazia" << endl; 
        return;
    }

    string palavraFormada = "";
    exibirInOrdemRecursivamente(this->raiz, palavraFormada);
}

int Trie::alturaTrie(){
    return calcularAlturaRecursivamente(this->raiz);
}

void Trie::gerarDOT(const std::string& caminho) {
    std::ofstream arquivo(caminho);

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para exportar o DOT da Trie" << std::endl;
        return;
    }

    arquivo << "digraph Trie {\n";
    arquivo << "    node [fontname=\"Arial\"];\n";
    arquivo << "    edge [fontname=\"Arial\"];\n";

    if (raiz == nullptr) {
        arquivo << "    // Árvore Trie vazia\n";
    } else {
        gerarDOTRecursivo(raiz, arquivo);
    }

    arquivo << "}\n";
    arquivo.close();
}

// Funções para aplicação
Trie::No* Trie::buscarNoDoPrefixo(No* no, const std::string& prefixo, size_t caractere) {
    if (no == nullptr) {
        return nullptr;
    }

    if (caractere == prefixo.length()) {
        return no;
    }

    int indiceLetra = prefixo[caractere] - 'a';
    if (indiceLetra < 0 || indiceLetra >= ALFABETO) {
        return nullptr;
    }

    return buscarNoDoPrefixo(no->filhos[indiceLetra], prefixo, caractere + 1);
}

void Trie::coletarPalavrasComPrefixo(No* no, std::string& palavraAtual, std::vector<std::string>& resultados) {
    if (no == nullptr) {
        return;
    }

    if (no->fim) {
        resultados.push_back(palavraAtual);
    }

    for (int i = 0; i < ALFABETO; i++) {
        if (no->filhos[i] != nullptr) {
            palavraAtual.push_back('a' + i);
            coletarPalavrasComPrefixo(no->filhos[i], palavraAtual, resultados);
            palavraAtual.pop_back();
        }
    }
}

std::vector<std::string> Trie::autocompletar(const std::string& prefixo) {
    std::vector<std::string> sugestoes;
    std::string prefixoTratado = normalizarPalavra(prefixo);

    if (prefixoTratado.empty() || this->raiz == nullptr) {
        return sugestoes;
    }

    No* noPrefixo = buscarNoDoPrefixo(this->raiz, prefixoTratado, 0);

    if (noPrefixo != nullptr) {
        std::string palavraAtual = prefixoTratado;
        coletarPalavrasComPrefixo(noPrefixo, palavraAtual, sugestoes);
    }

    return sugestoes;
}