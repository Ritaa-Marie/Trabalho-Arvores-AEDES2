CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2 -Isrc
PYTHON   := python3

# Diretórios
SRC_DIR    := src
BUILD_DIR  := build
BIN_DIR    := bin
SCRIPT_DIR := scripts

$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

ALL_SRCS   := $(shell find $(SRC_DIR) -name "*.cpp")
COMMON_SRCS := $(filter-out $(SRC_DIR)/main.cpp, $(ALL_SRCS))

COMMON_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(COMMON_SRCS))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ==============================================================================
# 1. BATERIA PRINCIPAL (src/main.cpp)
# ==============================================================================
all: $(BIN_DIR)/benchmark

$(BIN_DIR)/benchmark: $(COMMON_OBJS) $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

run-tests: $(BIN_DIR)/benchmark
	@./$(BIN_DIR)/benchmark

# ==============================================================================
# 2. SCRIPTS PYTHON (Geração de Dados e Gráficos)
# ==============================================================================
gerar-dados:
	$(PYTHON) $(SCRIPT_DIR)/gerar_dados.py

gerar-graficos:
	$(PYTHON) $(SCRIPT_DIR)/gerar_graficos.py

# ==============================================================================
# 3. TESTES ISOLADOS (Um para cada árvore)
# ==============================================================================
test-avl: $(BIN_DIR)/test_avl
	@./$(BIN_DIR)/test_avl

$(BIN_DIR)/test_avl: $(COMMON_OBJS) $(SCRIPT_DIR)/test_avl.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test-bst: $(BIN_DIR)/test_bst
	@./$(BIN_DIR)/test_bst

$(BIN_DIR)/test_bst: $(COMMON_OBJS) $(SCRIPT_DIR)/test_bst.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test-splay: $(BIN_DIR)/test_splay
	@./$(BIN_DIR)/test_splay

$(BIN_DIR)/test_splay: $(COMMON_OBJS) $(SCRIPT_DIR)/test_splay.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test-treap: $(BIN_DIR)/test_treap
	@./$(BIN_DIR)/test_treap

$(BIN_DIR)/test_treap: $(COMMON_OBJS) $(SCRIPT_DIR)/test_treap.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test-trie: $(BIN_DIR)/test_trie
	@./$(BIN_DIR)/test_trie

$(BIN_DIR)/test_trie: $(COMMON_OBJS) $(SCRIPT_DIR)/test_trie.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test-patricia: $(BIN_DIR)/test_patricia
	@./$(BIN_DIR)/test_patricia

$(BIN_DIR)/test_patricia: $(COMMON_OBJS) $(SCRIPT_DIR)/test_patricia.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test-kdtree: $(BIN_DIR)/test_kdtree
	@./$(BIN_DIR)/test_kdtree

$(BIN_DIR)/test_kdtree: $(COMMON_OBJS) $(SCRIPT_DIR)/test_kdtree.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# ==============================================================================
# 4. APLICAÇÕES PRÁTICAS (Uma para cada árvore)
# ==============================================================================
splay-aplicacao: $(BIN_DIR)/app_splay
	@./$(BIN_DIR)/app_splay

$(BIN_DIR)/app_splay: $(COMMON_OBJS) $(SCRIPT_DIR)/app_splay.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

treap-aplicacao: $(BIN_DIR)/app_treap
	@./$(BIN_DIR)/app_treap

$(BIN_DIR)/app_treap: $(COMMON_OBJS) $(SCRIPT_DIR)/app_treap.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

trie-aplicacao: $(BIN_DIR)/app_trie
	@./$(BIN_DIR)/app_trie

$(BIN_DIR)/app_trie: $(COMMON_OBJS) $(SCRIPT_DIR)/app_trie.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

patricia-aplicacao: $(BIN_DIR)/app_patricia
	@./$(BIN_DIR)/app_patricia

$(BIN_DIR)/app_patricia: $(COMMON_OBJS) $(SCRIPT_DIR)/app_patricia.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

kdtree-aplicacao: $(BIN_DIR)/app_kdtree
	@./$(BIN_DIR)/app_kdtree

$(BIN_DIR)/app_kdtree: $(COMMON_OBJS) $(SCRIPT_DIR)/app_kdtree.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

# ==============================================================================
# 5. COMANDOS GERAIS (Roda todos de uma vez)
# ==============================================================================
test-all: test-avl test-bst test-splay test-treap test-trie test-patricia test-kdtree
app-all: splay-aplicacao treap-aplicacao trie-aplicacao patricia-aplicacao kdtree-aplicacao

# ==============================================================================
# 6. LIMPEZA
# ==============================================================================
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all run-tests gerar-dados plotar-graficos clean test-all app-all\
        test-avl test-bst test-splay test-treap test-trie test-patricia test-kdtree \
        splay-aplicacao treap-aplicacao trie-aplicacao patricia-aplicacao kdtree-aplicacao
