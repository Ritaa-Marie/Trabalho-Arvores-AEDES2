#include "KdTree.hpp"

using namespace std;

KdTree::KdTree(int k){
    if(k < 1){
        this->k = 2;
    } else {
        this->k = k;
    }

    this->num_comparacoes = 0;
    this->consumo_memoria = 0;
    this->tempo_execucao = 0;
    this->ultima_operacao_sucesso = false;
    this->raiz = nullptr;
}

KdTree::~KdTree(){
    destruirKdTree(this->raiz);
    this->raiz = nullptr;
}

KdTree::No* KdTree::inserirRecursivamente(No* no, const std::vector<double>& coord, int profundidade){
    if(no == nullptr){
        No *novo = criarNo(coord, profundidade);
        this->ultima_operacao_sucesso = true;
        return novo;
    }

    bool saoIguais = pontosIguais(coord, no->ponto.coordenadas);
    this->num_comparacoes++;
    if(saoIguais){
        this->ultima_operacao_sucesso = false;
        return no;
    }

    int eixoAtual = definirEixo(profundidade);

    this->num_comparacoes++;
    if(coord[eixoAtual] < no->ponto.coordenadas[eixoAtual]){
        no->esquerda = inserirRecursivamente(no->esquerda, coord, profundidade + 1);
    } else {
        no->direita = inserirRecursivamente(no->direita, coord, profundidade + 1);
    }

    return no;
}

KdTree::No* KdTree::deletarRecursivamente(No* no, const std::vector<double>& coord, int profundidade){
    if (no == nullptr) {
        this->ultima_operacao_sucesso = false;
        return nullptr;
    }

    int eixoAtual = definirEixo(profundidade);
    bool saoIguais = pontosIguais(coord, no->ponto.coordenadas);

    this->num_comparacoes++;
    if(saoIguais) {
        this->ultima_operacao_sucesso = true;
        if(no->esquerda == nullptr && no->direita == nullptr){
            this->num_comparacoes++;
            this->consumo_memoria -= sizeof(No) + no->ponto.coordenadas.capacity() * sizeof(double);
            delete no;
            return nullptr;
        }

        this->num_comparacoes++;
        if(no->direita != nullptr){
            No* minimo = buscarMinimo(no->direita, eixoAtual, profundidade + 1);
            no->ponto = minimo->ponto;
            no->direita = deletarRecursivamente(no->direita, minimo->ponto.coordenadas, profundidade + 1);
        } else if (no->esquerda != nullptr) {
            this->num_comparacoes++;
            No* minimo = buscarMinimo(no->esquerda, eixoAtual, profundidade + 1);
            no->ponto = minimo->ponto;
            no->direita = no->esquerda;
            no->esquerda = nullptr;
            no->direita = deletarRecursivamente(no->direita, minimo->ponto.coordenadas, profundidade + 1);
        }

        return no;
    }

    this->num_comparacoes++;
    if(coord[eixoAtual] < no->ponto.coordenadas[eixoAtual]) {
        no->esquerda = deletarRecursivamente(no->esquerda, coord, profundidade + 1);
    } else {
        no->direita = deletarRecursivamente(no->direita, coord, profundidade + 1);
    }

    return no;
}

KdTree::No* KdTree::buscarRecursivamente(No* no, const std::vector<double>& coord, int profundidade){
    if(no == nullptr){
        return nullptr;
    }

    bool saoIguais = pontosIguais(coord, no->ponto.coordenadas);
    this->num_comparacoes++;
    if(saoIguais){
        return no;
    }

    int eixoAtual = definirEixo(profundidade);

    this->num_comparacoes++;
    if(coord[eixoAtual] < no->ponto.coordenadas[eixoAtual]){
        return buscarRecursivamente(no->esquerda, coord, profundidade + 1);
    } else {
        return buscarRecursivamente(no->direita, coord, profundidade + 1);
    }

    return no;
}

void KdTree::buscarVizinhoMaisProximoRecursivo(No* no, const std::vector<double>& alvo, int profundidade, No*& melhorNo, double& melhorDistancia){
    if (no == nullptr) {
        return;
    }

    double distanciaAtual = calcularDistancia(no->ponto.coordenadas, alvo);

    if (distanciaAtual < melhorDistancia) {
        melhorDistancia = distanciaAtual;
        melhorNo = no;
    }

    int eixoAtual = definirEixo(profundidade);

    No* proximaSubarvore = nullptr;
    No* outraSubarvore = nullptr;

    if(alvo[eixoAtual] < no->ponto.coordenadas[eixoAtual]){
        proximaSubarvore = no->esquerda;
        outraSubarvore = no->direita;
    } else {
        proximaSubarvore = no->direita;
        outraSubarvore = no->esquerda;
    }

    buscarVizinhoMaisProximoRecursivo(proximaSubarvore, alvo, profundidade + 1, melhorNo, melhorDistancia);

    double distanciaPlano = std::fabs(alvo[eixoAtual] - no->ponto.coordenadas[eixoAtual]);

    if (distanciaPlano < melhorDistancia) {
        buscarVizinhoMaisProximoRecursivo(outraSubarvore, alvo, profundidade + 1, melhorNo, melhorDistancia);
    }
}

void KdTree::destruirKdTree(No *no){
    if(no == nullptr){
        return;
    }

    destruirKdTree(no->esquerda);
    destruirKdTree(no->direita);
    delete no;
}

void KdTree::exibirInOrdemRecursivamente(No *no){
    if(no != nullptr){
        exibirInOrdemRecursivamente(no->esquerda);

        cout << "(";
        for (size_t i = 0; i < no->ponto.coordenadas.size(); i++) {
            cout << no->ponto.coordenadas[i];
            if (i + 1 < no->ponto.coordenadas.size()) {
                cout << ", ";
            }
        }
        cout << ") [Eixo: " << no->eixo << "]\n";

        exibirInOrdemRecursivamente(no->direita);
    }
}

int KdTree::calcularAlturaRecursivamente(No *no){
    if(no == nullptr){
        return -1;
    }

    int tamDireita = calcularAlturaRecursivamente(no->direita);
    int tamEsquerda = calcularAlturaRecursivamente(no->esquerda);

    return (1 +  max(tamDireita, tamEsquerda));
}


void KdTree::gerarDOTRecursivo(No* no, std::ofstream& arquivo) {
    if (no == nullptr) {
        return;
    }

    std::string rotuloPonto = "(";
    for (size_t i = 0; i < no->ponto.coordenadas.size(); i++) {
        rotuloPonto += std::to_string(no->ponto.coordenadas[i]);
        if (i + 1 < no->ponto.coordenadas.size()) {
            rotuloPonto += ", ";
        }
    }
    rotuloPonto += ")";

    arquivo << "    \"" << no << "\" [label=\"" << rotuloPonto 
            << "\\nEixo: " << no->eixo << "\"];\n";

    if (no->esquerda != nullptr) {
        arquivo << "    \"" << no << "\" -> \"" << no->esquerda << "\" [label=\"L\"];\n";
        gerarDOTRecursivo(no->esquerda, arquivo);
    } else {
        arquivo << "    nullL" << no << " [shape=point];\n";
        arquivo << "    \"" << no << "\" -> nullL" << no << ";\n";
    }

    if (no->direita != nullptr) {
        arquivo << "    \"" << no << "\" -> \"" << no->direita << "\" [label=\"R\"];\n";
        gerarDOTRecursivo(no->direita, arquivo);
    } else {
        arquivo << "    nullR" << no << " [shape=point];\n";
        arquivo << "    \"" << no << "\" -> nullR" << no << ";\n";
    }
}

KdTree::No* KdTree::criarNo(const std::vector<double>& coordenadas, int profundidade){
    Ponto ponto;
    ponto.coordenadas = coordenadas;

    int eixo = definirEixo(profundidade);
    No *no = new No(eixo, ponto);
    this->consumo_memoria += sizeof(No) + no->ponto.coordenadas.capacity() * sizeof(double);
    return no;
}

int KdTree::definirEixo(int profundidade){
    return (profundidade % this->k);
}

bool KdTree::pontoDentroDoDelimitador(const std::vector<double>& min, const std::vector<double>& max, const std::vector<double>& ponto){
    if(min.size() != max.size() || max.size() != ponto.size()){
        return false;
    }

    for(size_t i=0;i<min.size();i++){
        if(min[i] > ponto[i] || max[i] < ponto[i]){
            return false;
        }
    }

    return true;
}

void KdTree::buscarPontosNoDelimitadorRecursivo(No* no, const std::vector<double>& min, const std::vector<double>& max, int profundidade, std::vector<std::vector<double>>& resultados){
    if(no == nullptr){
        return;
    }

    bool pertenceConjunto = pontoDentroDoDelimitador(min, max, no->ponto.coordenadas);
    if(pertenceConjunto){
        resultados.push_back(no->ponto.coordenadas);
    }

    int eixoAtual = definirEixo(profundidade);

    if(min[eixoAtual] <= no->ponto.coordenadas[eixoAtual]){
        buscarPontosNoDelimitadorRecursivo(no->esquerda, min, max, profundidade + 1, resultados);
    } 

    if(max[eixoAtual] >= no->ponto.coordenadas[eixoAtual]){
        buscarPontosNoDelimitadorRecursivo(no->direita, min, max, profundidade + 1, resultados);
    }

}

double KdTree::calcularDistancia(const std::vector<double>& ponto1, const std::vector<double>& ponto2){
    if(ponto1.size() != ponto2.size()){
        return -1.0;
    }

    double soma = 0.0;
    for(size_t i=0;i<ponto1.size();i++){
        double diferenca = ponto1[i] - ponto2[i];
        soma += diferenca * diferenca;
    }
    
    return sqrt(soma);
}

bool KdTree::pontosIguais(const std::vector<double>& ponto1, const std::vector<double>& ponto2){
    if(ponto1.size() != ponto2.size()){
        return false;
    }

    const double eps = 1e-6;
    for(size_t i=0;i<ponto1.size();i++){
        this->num_comparacoes++;
        if(std::fabs(ponto1[i] - ponto2[i]) >= eps){
            return false;
        }
    }

    return true;
}

KdTree::No* KdTree::buscarMinimo(No* no, int dimensaoAlvo, int profundidadeAtual){
    if(no == nullptr){
        return nullptr;
    }

    int eixoAtual = definirEixo(profundidadeAtual);
    this->num_comparacoes++;
    if(eixoAtual == dimensaoAlvo){
        if(no->esquerda == nullptr){
            return no;
        }
        return buscarMinimo(no->esquerda, dimensaoAlvo, profundidadeAtual + 1);
    } 

    No *menorSubEsq = buscarMinimo(no->esquerda, dimensaoAlvo, profundidadeAtual + 1);
    No *menorSubDir = buscarMinimo(no->direita, dimensaoAlvo, profundidadeAtual + 1);
    No* minNo = no;

    this->num_comparacoes++;
    if (menorSubEsq != nullptr && menorSubEsq->ponto.coordenadas[dimensaoAlvo] < minNo->ponto.coordenadas[dimensaoAlvo]) {
        minNo = menorSubEsq;
    }

    this->num_comparacoes++;
    if (menorSubDir != nullptr && menorSubDir->ponto.coordenadas[dimensaoAlvo] < minNo->ponto.coordenadas[dimensaoAlvo]) {
        minNo = menorSubDir;
    }

    return minNo;
}

long long KdTree::getNumComparacoes(){
    return this->num_comparacoes;
}

long long KdTree::getConsumoMemoria(){
    return this->consumo_memoria;
}

long long KdTree::getTempoExecucao(){
    return this->tempo_execucao;
}

bool KdTree::getUltimaOperacaoSucesso(){
    return this->ultima_operacao_sucesso;
}

void KdTree::setNumComparacoes(long long num){
    if(num >= 0){
        this->num_comparacoes = num;
    }
}

void KdTree::setConsumoMemoria(long long num){
    this->consumo_memoria = num;
}

void KdTree::setTempoExecucao(long long num){
    this->tempo_execucao = num;
}

void KdTree::setUltimaOperacaoSucesso(bool status){
    this->ultima_operacao_sucesso = status;
}


void KdTree::inserirElemento(const std::vector<double>& ponto){
    if(ponto.size() != static_cast<size_t>(this->k)){
        this->ultima_operacao_sucesso = false;
        cout << "Dimensão do ponto incompatível com a árvore" << endl;
        return;
    }

    this->ultima_operacao_sucesso = false;
    this->raiz = inserirRecursivamente(this->raiz, ponto, 0);

    /*if(this->ultima_operacao_sucesso){
        cout << "Elemento inserido na árvore Kd Tree com sucesso" << endl;
    } else {
        cout << "Esse elemento já existe na árvore Kd Tree" << endl;
    }*/
}

bool KdTree::buscarElemento(const std::vector<double>& ponto){
    if(ponto.size() != static_cast<size_t>(this->k)){
        this->ultima_operacao_sucesso = false;
        cout << "Dimensão do ponto incompatível com a árvore" << endl;
        return false;
    } 

    No *aux = buscarRecursivamente(this->raiz, ponto, 0);
    if(aux == nullptr){
        return false;
    } else {
        return true;
    }
}

std::vector<double> KdTree::buscarElementoMaisProximo(const std::vector<double>& alvo){
    if (this->raiz == nullptr || alvo.size() != static_cast<size_t>(this->k)) {
        this->ultima_operacao_sucesso = false;
        cout << "Árvore vazia ou dimensão incompatível\n";
        return {};
    }

    No* melhorNo = nullptr;
    double melhorDistancia = numeric_limits<double>::max();

    buscarVizinhoMaisProximoRecursivo(this->raiz, alvo, 0, melhorNo, melhorDistancia);

    if (melhorNo != nullptr) {
        this->ultima_operacao_sucesso = true;

        cout << "Vizinho mais próximo encontrado: (";
        for (size_t i = 0; i < melhorNo->ponto.coordenadas.size(); i++) {
            cout << melhorNo->ponto.coordenadas[i];
            if (i + 1 < melhorNo->ponto.coordenadas.size()) {
                cout << ", ";
            }
        }
        cout << ") [Eixo: " << melhorNo->eixo << "] | Distancia: " << melhorDistancia << "\n";
        return melhorNo->ponto.coordenadas;
    } else {
        this->ultima_operacao_sucesso = false;
        cout << "Nenhum vizinho encontrado.\n";
        return {};
    }
}

void KdTree::deletarElemento(const std::vector<double>& ponto){
    if(ponto.size() != static_cast<size_t>(this->k)){
        this->ultima_operacao_sucesso = false;
        cout << "Dimensão do ponto incompatível com a árvore" << endl;
        return;
    }

    this->ultima_operacao_sucesso = false;
    this->raiz = deletarRecursivamente(this->raiz, ponto, 0);
    /*if(ultima_operacao_sucesso){
        cout << "Elemento deletado da Kd Tree com sucesso" <<endl;
    } else {
        cout << "Erro ao deletar elemento da Kd Tree" <<endl;
    }*/
}

void KdTree::exibirKdTreeInOrdem(){
    if(this->raiz == nullptr){
        cout << "A árvore Kd Tree está vazia" << endl; 
        return;
    }

    exibirInOrdemRecursivamente(this->raiz);
}

int KdTree::alturaKdTree(){
    return calcularAlturaRecursivamente(this->raiz);
}

void KdTree::buscarPontosNoDelimitador(const std::vector<double>& min, const std::vector<double>& max){
    std::vector<std::vector<double>> resultados;
    buscarPontosNoDelimitadorRecursivo(this->raiz, min, max, 0, resultados);

    for (size_t i = 0; i < resultados.size(); i++) {
        cout << "(";
        for (size_t j = 0; j < resultados[i].size(); j++) {
            cout << resultados[i][j];
            if (j + 1 < resultados[i].size()) {
                cout << ", ";
            }
        }
        cout << ")\n";
    }
}

void KdTree::gerarDOT(const std::string& caminho){
    std::ofstream arquivo(caminho);

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo do DOT para Kd Tree" << endl;
        return;
    }

    arquivo << "digraph KdTree {\n";
    arquivo << "    node [fontname=\"Helvetica\", shape=ellipse, style=filled, fillcolor=lightgray];\n";
    arquivo << "    edge [fontname=\"Helvetica\"];\n\n";

    if (this->raiz == nullptr) {
        arquivo << "    empty [label=\"Árvore Vazia\", shape=plaintext];\n";
    } else {
        gerarDOTRecursivo(this->raiz, arquivo);
    }

    arquivo << "}\n";
    arquivo.close();
}

// Função para aplicação
void KdTree::buscarKVizinhosProximosRecursivo(No* no, const std::vector<double>& alvo, int k, int profundidade, std::priority_queue<std::pair<double, std::vector<double>>>& maxHeap){
    if (no == nullptr) {
        return;
    }

    double dist = calcularDistancia(no->ponto.coordenadas, alvo);

    if (static_cast<int>(maxHeap.size()) < k) {
        maxHeap.push({dist, no->ponto.coordenadas});
    } else if (dist < maxHeap.top().first) {
        maxHeap.pop();
        maxHeap.push({dist, no->ponto.coordenadas});
    }

    int eixoAtual = definirEixo(profundidade);
    double distanciaPlano = alvo[eixoAtual] - no->ponto.coordenadas[eixoAtual];

    No* proxima = nullptr;
    if(distanciaPlano < 0){
       proxima = no->esquerda;
    } else {
        proxima = no->direita;
    }

    No* outra = nullptr;
    if(distanciaPlano < 0){
       outra = no->direita;
    } else {
        outra = no->esquerda;
    }
    

    buscarKVizinhosProximosRecursivo(proxima, alvo, k, profundidade + 1, maxHeap);

    if (static_cast<int>(maxHeap.size()) < k || std::fabs(distanciaPlano) < maxHeap.top().first) {
        buscarKVizinhosProximosRecursivo(outra, alvo, k, profundidade + 1, maxHeap);
    }
}

std::vector<std::vector<double>> KdTree::buscarKVizinhosProximos(const std::vector<double>& alvo, int k) {
    if (this->raiz == nullptr || alvo.size() != static_cast<size_t>(this->k) || k < 1) {
        this->ultima_operacao_sucesso = false;
        return {};
    }

    // <distância, coordenadas>
    std::priority_queue<std::pair<double, std::vector<double>>> maxHeap;

    buscarKVizinhosProximosRecursivo(this->raiz, alvo, k, 0, maxHeap);

    this->ultima_operacao_sucesso = !maxHeap.empty();

    std::vector<std::vector<double>> vizinhos;
    while (!maxHeap.empty()) {
        vizinhos.push_back(maxHeap.top().second);
        maxHeap.pop();
    }

    std::reverse(vizinhos.begin(), vizinhos.end());
    return vizinhos;
}