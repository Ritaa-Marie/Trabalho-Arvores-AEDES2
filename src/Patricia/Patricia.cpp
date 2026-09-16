#include "Patricia.hpp"

using namespace std;

Patricia::Patricia(): idNoAtual(0), num_comparacoes(0), consumo_memoria(0), tempo_execucao(0), ultima_operacao_sucesso(false), raiz(nullptr){}

Patricia::~Patricia(){
    destruirPatricia(this->raiz);
    raiz = nullptr;
}

Patricia::No* Patricia::criarNo(){
    this->idNoAtual++;
    No *novo = new No(this->idNoAtual);
    return novo;
}

Patricia::No* Patricia::inserirRecursivamente(No *no, const std::string& palavra, size_t caractere){
    if(no == nullptr){
        No *novo = criarNo();
        novo->prefixo = palavra.substr(caractere);
        novo->fim = true;
        this->consumo_memoria += sizeof(No) + novo->prefixo.capacity();
        this->ultima_operacao_sucesso = true;
        return novo;
    }

    size_t tamPalavra = palavra.length();
    size_t tamPrefixo = no->prefixo.length();
    size_t posicaoDiverge = tamPrefixo;

    for(size_t i=0;i<tamPrefixo;i++){
        this->num_comparacoes++;
        if((caractere + i) >= tamPalavra || palavra[caractere + i] != no->prefixo[i]){
            posicaoDiverge = i;
            break;
        }
    }

    this->num_comparacoes++;
    if(posicaoDiverge < tamPrefixo){
        this->num_comparacoes++;
        if (caractere + posicaoDiverge < tamPalavra) {
            unsigned char charDivergente = palavra[caractere + posicaoDiverge];
            this->num_comparacoes++;
            if (charDivergente < 'a' || charDivergente >= ('a' + ALFABETO)) {
                this->ultima_operacao_sucesso = false;
                return no; 
            }
        }

        No *novoPai = split(no, posicaoDiverge);
        this->num_comparacoes++;
        if(caractere + posicaoDiverge == tamPalavra){
            novoPai->fim = true;
            this->ultima_operacao_sucesso = true;
        } else {
            No *novoFilho = criarNo();
            novoFilho->prefixo = palavra.substr(caractere + posicaoDiverge);
            novoFilho->fim = true;
            this->consumo_memoria += sizeof(No) + no->prefixo.capacity();
            size_t indiceLetra = novoFilho->prefixo[0] - 'a';
            this->num_comparacoes++;
            if (indiceLetra < ALFABETO) {
                novoPai->filhos[indiceLetra] = novoFilho;
                this->ultima_operacao_sucesso = true;
            } else {
                this->ultima_operacao_sucesso = false;
                return no;
            }
        }
        return novoPai;
    } 
    
    size_t proximoCaractere = caractere + tamPrefixo;
    this->num_comparacoes++;
    if(proximoCaractere == tamPalavra){
        this->num_comparacoes++;
        if (!no->fim) {
            no->fim = true;
            this->ultima_operacao_sucesso = true; 
        } else {
            this->ultima_operacao_sucesso = false;
        }
        return no;
    } 
    
    size_t indiceLetra = palavra[proximoCaractere] - 'a';
    this->num_comparacoes++;
    if (indiceLetra < ALFABETO){
        no->filhos[indiceLetra] = inserirRecursivamente(no->filhos[indiceLetra], palavra, proximoCaractere);
    } else {
        this->ultima_operacao_sucesso = false;
        return no;
    }
    
    return no;
}

Patricia::No* Patricia::deletarRecursivamente(No *no, const std::string& palavra,size_t caractere){
    if(no == nullptr){
        return no;
    }

    size_t tamPrefixo = no->prefixo.length();
    size_t tamPalavra = palavra.length();

    bool prefixoBateu = true;
    for(size_t i=0;i<tamPrefixo;i++){
        this->num_comparacoes++;
        if((caractere + i) >= tamPalavra || palavra[caractere + i] != no->prefixo[i]){
            this->ultima_operacao_sucesso = false;
            prefixoBateu = false;
            return no;
        }
    }

    size_t proximaLetra = caractere + tamPrefixo;
    
    this->num_comparacoes++;
    if(prefixoBateu && tamPalavra == proximaLetra){
        this->num_comparacoes++;
        if(no->fim == true){
            no->fim = false;
            this->ultima_operacao_sucesso = true;
        } else {
            this->ultima_operacao_sucesso = false;
            return no;
        }
    } else {
        int posicaoFilho = palavra[proximaLetra] - 'a';
        if (posicaoFilho >= 0 && posicaoFilho < ALFABETO) {
                no->filhos[posicaoFilho] = deletarRecursivamente(no->filhos[posicaoFilho], palavra, proximaLetra);
            }
    }

    int numFilhos = 0;
    //No *filhoUnico = nullptr;
    for(int i=0;i<ALFABETO;i++){
        this->num_comparacoes++;
        if(no->filhos[i] == nullptr){
            numFilhos += 0;
        } else {
            numFilhos += 1;
            //filhoUnico = no->filhos[i];
        }
    }

    this->num_comparacoes++;
    if(numFilhos == 0 && no->fim == false){
        this->num_comparacoes++;
        if(no == raiz){
            raiz = nullptr;
        }
        this->consumo_memoria -= sizeof(No) + no->prefixo.capacity();
        delete no;
        return nullptr;
    } else if(numFilhos == 1 && no->fim == false){
        return merge(no);
    }

    return no;
}

Patricia::No* Patricia::buscarRecursivamente(No *no, const std::string& palavra, size_t caractere){
    if(no == nullptr){
        return nullptr;
    }

    size_t tamPrefixo = no->prefixo.length();
    size_t tamPalavra = palavra.length();

    bool prefixoBateu = true;
    for(size_t i=0;i<tamPrefixo;i++){
        this->num_comparacoes++;
        if((caractere + i) >= tamPalavra || palavra[caractere + i] != no->prefixo[i]){
            prefixoBateu = false;
            return nullptr;
        }
    }

    this->num_comparacoes++;
    if(prefixoBateu && palavra.length() == caractere + tamPrefixo){
        this->num_comparacoes++;
        if(no->fim == true){
            return no;
        } else {
            return nullptr;
        }
    }

    size_t proximoCaractere = caractere + tamPrefixo;
    this->num_comparacoes++;
    if(proximoCaractere < tamPalavra){
        int indiceFilho = palavra[proximoCaractere] - 'a';
        this->num_comparacoes++;
        if (indiceFilho >= 0 && indiceFilho < ALFABETO) {
            return buscarRecursivamente(no->filhos[indiceFilho], palavra, proximoCaractere);
        }
    }

    return nullptr;
}

void Patricia::destruirPatricia(No *no){
    if(no == nullptr){
        return;
    }

    for(int i=0;i<ALFABETO;i++){
        if(no->filhos[i] != nullptr){
            destruirPatricia(no->filhos[i]);
        }
    }

    delete no;
}

void Patricia::exibirInOrdemRecursivamente(No *no, std::string palavraFormada){
    if(no == nullptr){
        return;
    }

    palavraFormada += no->prefixo;
    if(no->fim){
        cout << palavraFormada << endl;
    } 

    for(int i=0;i<ALFABETO;i++){
        if(no->filhos[i] != nullptr){
            exibirInOrdemRecursivamente(no->filhos[i], palavraFormada);
        }
    }
}

int Patricia::calcularAlturaRecursivamente(No *no){
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

void Patricia::gerarDOTRecursivo(No *no, std::ofstream& arquivo) {
    if (no == nullptr) {
        return;
    }

    arquivo << "    \"" << no->idNo << "\" [label=\"";
    if (no == raiz && no->prefixo.empty()) {
        arquivo << "raiz";
    } else {
        arquivo << no->prefixo;
    }
    arquivo << "\", shape=" << (no->fim ? "doublecircle" : "circle");
    if (no->fim) {
        arquivo << ", style=filled, fillcolor=lightblue";
    }
    arquivo << "];\n";

    for (int i = 0; i < ALFABETO; i++) {
        if (no->filhos[i] != nullptr) {
            arquivo << "    \"" << no->idNo << "\" -> \"" << no->filhos[i]->idNo 
                    << "\" [label=\"" << no->filhos[i]->prefixo << "\"];\n";

            gerarDOTRecursivo(no->filhos[i], arquivo);
        }
    }
}

Patricia::No* Patricia::split(No *no, size_t posicaoDiverge){
    No *novoPai = criarNo();

    novoPai->prefixo = no->prefixo.substr(0, posicaoDiverge);
    this->consumo_memoria += sizeof(No) + novoPai->prefixo.capacity();
    no->prefixo = no->prefixo.substr(posicaoDiverge);

    size_t indiceLetra = no->prefixo[0] - 'a';
    novoPai->filhos[indiceLetra] = no;

    return novoPai;
}

Patricia::No* Patricia::merge(No *no){
    if(no == nullptr || no->fim || this->raiz == no){
        return no;
    }

    int numFilhos = 0;
    No *filhoUnico = nullptr;
    for(int i=0;i<ALFABETO;i++){
        if(no->filhos[i] == nullptr){
            numFilhos += 0;
        } else {
            numFilhos += 1;
            filhoUnico = no->filhos[i];
        }
    }

    if(numFilhos != 1){
        return no;
    }

    no->prefixo += filhoUnico->prefixo;
    no->fim = filhoUnico->fim;

    for (int i = 0; i < ALFABETO; i++) {
        no->filhos[i] = filhoUnico->filhos[i];
    }

    this->consumo_memoria -= sizeof(No) + filhoUnico->prefixo.capacity();
    delete filhoUnico;
    return no;
}

void Patricia::transformarMinusculo(std::string& palavra){
    for(char &letra: palavra){
        letra = tolower(letra);
    }
}

void Patricia::substituirCaracteresEspeciais(std::string& palavra){
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

std::string Patricia::normalizarPalavra(const std::string& palavra){
    string resultado = palavra;
    substituirCaracteresEspeciais(resultado);
    transformarMinusculo(resultado);

    return resultado;
}

long long Patricia::getNumComparacoes(){
    return this->num_comparacoes;
}

long long Patricia::getConsumoMemoria(){
    return this->consumo_memoria;
}

long long Patricia::getTempoExecucao(){
    return this->tempo_execucao;
}

bool Patricia::getUltimaOperacaoSucesso(){
    return this->ultima_operacao_sucesso;
}

void Patricia::setNumComparacoes(long long num){
    if(num >= 0){
    this->num_comparacoes = num;
    }
}

void Patricia::setConsumoMemoria(long long num){
    this->consumo_memoria = num;
}

void Patricia::setTempoExecucao(long long num){
    this->tempo_execucao = num;
}

void Patricia::setUltimaOperacaoSucesso(bool status){
    this->ultima_operacao_sucesso = status;
}

void Patricia::inserirElemento(const std::string& palavra){
    std::string palavraTratada = normalizarPalavra(palavra);
    
    if (palavraTratada.empty()) {
        this->ultima_operacao_sucesso = false;
        cout << "Esse elemento está inválido para inserção na árvore Patricia" << endl;
        return;
    }

    this->ultima_operacao_sucesso = false;
    raiz = inserirRecursivamente(raiz, palavraTratada, 0);
    
    /*if(this->ultima_operacao_sucesso){
        cout << "Elemento inserido na árvore Patricia com sucesso" << endl;
    } else {
        cout << "Esse elemento já existe na árvore Patricia" << endl;
    }*/
}

bool Patricia::buscarElemento(const std::string& palavra){
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

void Patricia::deletarElemento(const std::string& palavra){
    string palavraTratada = normalizarPalavra(palavra);

    if (palavraTratada.empty()) {
        cout << "Esse elemento está inválido para remoção na árvore Patricia" << endl;
        return;
    }

    this->ultima_operacao_sucesso = false;
    raiz = deletarRecursivamente(raiz, palavraTratada, 0);
    /*if(ultima_operacao_sucesso){
        cout << "Elemento deletado da Patricia com sucesso" <<endl;
    } else {
        cout << "Erro ao deletar elemento da Patricia" <<endl;
    }*/
}

void Patricia::exibirPatriciaInOrdem(){
    if(this->raiz == nullptr){
        cout << "A árvore Patricia Tree está vazia" << endl; 
        return;
    }

    string palavraFormada = "";
    exibirInOrdemRecursivamente(this->raiz, palavraFormada);
}

int Patricia::alturaPatricia(){
    return calcularAlturaRecursivamente(this->raiz);
}

void Patricia::gerarDOT(const std::string& caminho){
    std::ofstream arquivo(caminho);

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para exportar o DOT da Patricia Trie" << std::endl;
        return;
    }

    arquivo << "digraph PatriciaTrie {\n";
    arquivo << "    node [fontname=\"Arial\"];\n";
    arquivo << "    edge [fontname=\"Arial\"];\n";

    if (raiz == nullptr) {
        arquivo << "    // Arvore Patricia vazia\n";
    } else {
        gerarDOTRecursivo(raiz, arquivo);
    }

    arquivo << "}\n";
    arquivo.close();
}

// Função para aplicação
std::string Patricia::buscarPrefixoMaisLongo(const std::string& ipDestino) {
    std::string ipTratado = normalizarPalavra(ipDestino);
    
    if (ipTratado.empty() || this->raiz == nullptr) {
        return "Nenhum prefixo correspondente";
    }

    std::string melhorResultado = "";
    
    No* noBusca = buscarRecursivamente(this->raiz, ipTratado, 0);

    if (noBusca != nullptr) {
        return ipDestino; 
    }

    std::string prefixoAcumulado = "";
    for (char c : ipTratado) {
        prefixoAcumulado += c;
        if (this->buscarElemento(prefixoAcumulado)) {
            melhorResultado = prefixoAcumulado;
        }
    }

    if (melhorResultado.empty()) {
        return "(Default Route)";
    }

    return melhorResultado;
}