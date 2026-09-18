import os
import re
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# ==============================================================================
# CONFIGURAÇÕES GERAIS E DIRETÓRIOS
# ==============================================================================
PASTA_SAIDA = os.path.join("data", "output", "img", "graficos")
os.makedirs(PASTA_SAIDA, exist_ok=True)

TODAS_ESTRUTURAS = ["AVL", "BST", "Splay", "Treap", "Trie", "Patricia", "KDTree"]

CORES = {
    'AVL':      '#2563eb',  # Azul
    'BST':      '#dc2626',  # Vermelho
    'Treap':    '#16a34a',  # Verde
    'Splay':    '#ea580c',  # Laranja
    'Trie':     '#06b6d4',  # Ciano
    'Patricia': '#ec4899',  # Rosa/Roxo
    'KDTree':   '#8b5cf6'   # Roxo
}

ESTILOS_COMPLETO = {
    'BST':   {'color': CORES['BST'],   'marker': 'o', 'linestyle': '-', 'label': r'BST — Pior Caso $O(N^2)$'},
    'AVL':   {'color': CORES['AVL'],   'marker': 's', 'linestyle': '-', 'label': r'Árvore AVL'},
    'Treap': {'color': CORES['Treap'], 'marker': '^', 'linestyle': '-', 'label': r'Árvore Treap'},
    'Splay': {'color': CORES['Splay'], 'marker': 'D', 'linestyle': '-', 'label': r'Árvore Splay'}
}

COMPLEXIDADE_TEORICA = {
    "AVL":      lambda n: np.log2(n),
    "BST":      lambda n: np.log2(n),
    "Splay":    lambda n: np.log2(n),
    "Treap":    lambda n: np.log2(n),
    "Trie":     lambda n: np.ones_like(n),
    "Patricia": lambda n: np.ones_like(n),
    "KDTree":   lambda n: np.log2(n),
}

# ==============================================================================
# FUNÇÃO DE CARREGAMENTO E NORMALIZAÇÃO
# ==============================================================================
def carregar_dados(caminho_csv="data/output/resultado_dados_comparativos.csv"):
    if not os.path.exists(caminho_csv):
        caminho_csv = os.path.join("data", "output", "resultados.csv")
        
    df = pd.read_csv(caminho_csv)
    df.columns = df.columns.str.strip().str.lower()
    return df

# ==============================================================================
# 1. BLOCO: MÉTRICAS GERAIS (Geral Comparações, Rotações, Memória, Altura)
# ==============================================================================

def plot_metrica_geral(df, coluna, nome_arquivo, titulo, ylabel,
                       subtitulo="Análise comparativa de métricas estruturais",
                       estruturas=TODAS_ESTRUTURAS, escala_log=True):
    
    fig, ax = plt.subplots(figsize=(9, 6), dpi=300)
    fig.patch.set_facecolor('#f8fafc')
    ax.set_facecolor('#f8fafc')
    
    subset = df[(df["operacao"].str.lower() == "insercao") & 
                (df["dataset"].str.lower() == "aleatorio")]

    for estrutura in estruturas:
        dados_e = subset[subset["estrutura"] == estrutura]
        if dados_e.empty:
            continue
        
        dados_agrupados = dados_e.groupby("tamanhon", as_index=False)[coluna].mean()
        dados_agrupados = dados_agrupados.sort_values("tamanhon")
        
        if escala_log:
            dados_agrupados = dados_agrupados[dados_agrupados[coluna] > 0]
            
        if dados_agrupados.empty:
            continue

        cor = CORES.get(estrutura, "#333333")
        
        ax.plot(dados_agrupados["tamanhon"], dados_agrupados[coluna],
                marker="o", markersize=6, linestyle="-", linewidth=2.0, 
                label=estrutura, color=cor)

    ax.set_xscale("log")
    if escala_log:
        ax.set_yscale("log")

    ax.grid(True, which="major", color="#cbd5e1", linestyle="--", linewidth=0.7, alpha=0.7)
    ax.grid(True, which="minor", color="#e2e8f0", linestyle=":", linewidth=0.5, alpha=0.5)
    
    for spine in ax.spines.values():
        spine.set_color("#94a3b8")
        spine.set_linewidth(0.8)

    ax.set_xlabel("Tamanho da Entrada N (número de chaves)", fontsize=10, fontweight="bold", color="#1e293b", labelpad=10)
    ax.set_ylabel(f"{ylabel} {'(escala log₁₀)' if escala_log else ''}", fontsize=10, fontweight="bold", color="#1e293b", labelpad=10)
    
    plt.suptitle(titulo, fontsize=13, fontweight="bold", color="#0f172a", x=0.125, ha="left", y=0.97)
    ax.set_title(subtitulo, fontsize=9.5, color="#64748b", loc="left", pad=12)

    leg = ax.legend(frameon=True, facecolor="white", edgecolor="#cbd5e1", fontsize=9.5, loc="upper left")
    for text in leg.get_texts():
        text.set_color("#1e293b")

    fig.tight_layout(rect=[0, 0, 1, 0.95])
    
    os.makedirs(PASTA_SAIDA, exist_ok=True)
    caminho = os.path.join(PASTA_SAIDA, nome_arquivo)
    fig.savefig(caminho, dpi=300, bbox_inches='tight', facecolor=fig.get_facecolor())
    plt.close(fig)
    print(f"[+] Gráfico com linhas retas gerado: {nome_arquivo}")

# ==============================================================================
# 2. BLOCO: TEORIA X PRÁTICA (Curva Assintótica Sobreposta)
# ==============================================================================
def plot_teoria_vs_pratica_grid(df: pd.DataFrame, operacao: str = "Busca", 
                                dataset: str = "Aleatorio", 
                                nome_arquivo: str = "teoria_vs_pratica_grid.png"):
    estruturas = ["AVL", "BST", "Splay", "Treap"]
    
    fig, axes = plt.subplots(2, 2, figsize=(11, 9.5), dpi=200)
    axes = axes.flatten()

    sub_df = df[(df["operacao"].str.lower() == operacao.lower()) & 
                (df["dataset"].str.lower() == dataset.lower())]

    if sub_df.empty:
        print(f"[AVISO] Sem dados para Teoria x Prática Grid ({operacao}/{dataset}).")
        return

    for idx, estr in enumerate(estruturas):
        ax = axes[idx]
        
        dados_e = sub_df[sub_df["estrutura"].str.upper() == estr.upper()]
        if dados_e.empty and estr.upper() == 'SPLAT':
            dados_e = sub_df[sub_df["estrutura"].str.upper() == 'SPLAY']

        if dados_e.empty:
            ax.set_title(estr, fontweight="bold")
            continue

        dados = dados_e.groupby("tamanhon", as_index=False)["tempomedio_ns"].mean().sort_values("tamanhon")
        
        n = dados["tamanhon"].values
        tempo_real = dados["tempomedio_ns"].values

        func_teorica = COMPLEXIDADE_TEORICA.get(estr, lambda x: np.log2(x))
        curva_teorica = func_teorica(n)
        fator = tempo_real[0] / curva_teorica[0] if curva_teorica[0] != 0 else 1
        curva_teorica_escalada = curva_teorica * fator

        cor = CORES.get(estr, "#2563eb")

        ax.plot(n, tempo_real, marker="o", markersize=4, linewidth=1.8,
                linestyle="-", color=cor, label="Observado (experimental)")
        ax.plot(n, curva_teorica_escalada, linestyle="--", linewidth=1.5,
                color="black", label="Curva teórica esperada (escalada)")

        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.set_xlabel("Tamanho da entrada (n) — escala log", fontsize=8.5)
        ax.set_ylabel("Tempo médio por operação (ns) — escala log", fontsize=8.5)
        
        nome_titulo = "Splay" if estr.upper() in ["SPLAT", "SPLAY"] else estr
        ax.set_title(f"{nome_titulo}", fontsize=11, fontweight="bold", pad=10)
        
        ax.grid(True, which="both", linestyle=":", alpha=0.4)
        ax.tick_params(labelsize=8)

        ax.legend(loc="upper left", fontsize=7.5, frameon=True, 
                  facecolor="white", edgecolor="#d1d5db")

    fig.suptitle(f"Teoria x Prática: tempo de {operacao} — Dataset {dataset}", 
                 fontsize=13, fontweight="bold", y=0.98)

    fig.tight_layout(rect=[0, 0, 1, 0.95])
    
    caminho_saida = os.path.join(PASTA_SAIDA, nome_arquivo)
    fig.savefig(caminho_saida, bbox_inches="tight", dpi=200)
    plt.close(fig)
    print(f"[+] Grid 2x2 com margem ajustada gerado em: {caminho_saida}")

# ==============================================================================
# 3. BLOCO: DASHBOARD ESTILIZADO (Pior Caso)
# ==============================================================================
ESTILOS_CUSTOM = {
    'BST':   {'color': '#dc2626', 'marker': 'o', 'linestyle': '-', 'label': r'BST — Pior Caso $O(N^2)$'},
    'AVL':   {'color': '#2563eb', 'marker': 's', 'linestyle': '-', 'label': r'Árvore AVL'},
    'Treap': {'color': '#16a34a', 'marker': '^', 'linestyle': '-', 'label': r'Árvore Treap'},
    'Splat': {'color': '#ea580c', 'marker': 'D', 'linestyle': '-', 'label': r'Árvore Splat'},
    'Splay': {'color': '#c154c1', 'marker': 'D', 'linestyle': '-', 'label': r'Árvore Splay'}
}

def plotar_grafico_estilizado(df, dataset='Ordenado', operacao='Insercao', 
                              metrica='tempomedio_ns', nome_saida='pior_caso_ordenado.png',
                              subtitulo=None, label_y_custom=None):
    
    sub_df = df[(df['dataset'].str.lower() == dataset.lower()) & 
                (df['operacao'].str.lower() == operacao.lower())].copy()
    if sub_df.empty:
        print(f"[-] Sem dados para Dataset={dataset} e Operacao={operacao}")
        return

    if label_y_custom:
        label_y = label_y_custom
    elif metrica == 'tempomedio_ns':
        label_y = f'Tempo de {operacao.capitalize()} (s) — escala $\log_{{10}}$'
    else:
        label_y = f'{metrica} — escala $\log_{{10}}$'

    if 'tempomedio_ns' in sub_df.columns:
        sub_df['ValorExibicao'] = sub_df['tempomedio_ns'] / 1e9

    fig, ax = plt.subplots(figsize=(11, 6.5), dpi=300)
    fig.patch.set_facecolor('#f8fafc')
    ax.set_facecolor('#f8fafc')

    ordem_estruturas = ['BST', 'AVL', 'TREAP', 'SPLAT', 'SPLAY']

    for estr in ordem_estruturas:
        dados_estr = sub_df[sub_df['estrutura'].str.upper() == estr].copy()
        
        dados_estr = dados_estr[dados_estr['ValorExibicao'] > 0]

        if not dados_estr.empty:
            nome_chave = estr.capitalize() if estr not in ['BST', 'AVL'] else estr
            dados_agrupados = dados_estr.groupby('tamanhon', as_index=False)['ValorExibicao'].mean().sort_values('tamanhon')
            
            estilo = ESTILOS_CUSTOM.get(nome_chave, {'color': '#333333', 'marker': 'o', 'linestyle': '-', 'label': nome_chave})
            ax.plot(
                dados_agrupados['tamanhon'], 
                dados_agrupados['ValorExibicao'], 
                color=estilo['color'],
                marker=estilo['marker'],
                linestyle='-',
                linewidth=2.2,
                markersize=6,
                label=estilo['label']
            )

    ax.set_xscale('log')
    ax.set_yscale('log')

    ax.set_xlabel('Tamanho da Entrada N (chaves em ordem crescente)', fontsize=11, fontweight='bold', color='#334155', labelpad=10)
    ax.set_ylabel(label_y, fontsize=11, fontweight='bold', color='#334155', labelpad=10)

    ax.grid(True, which="both", linestyle=":", color="#e2e8f0", linewidth=1.0)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_color('#cbd5e1')
    ax.spines['bottom'].set_color('#cbd5e1')
    ax.tick_params(colors='#64748b', labelsize=9)

    legend = ax.legend(
        loc='upper right', 
        frameon=True, 
        facecolor='white', 
        edgecolor='#e2e8f0',
        fontsize=10,
        ncol=2,
        borderpad=1,
        handletextpad=0.8
    )
    legend.get_frame().set_boxstyle('Round,pad=0.5,rounding_size=0.4')

    tipo_analise = "Análise de Caso Médio" if dataset.lower() == 'aleatorio' else "Análise de Pior Caso"
    
    plt.text(0.0, 1.12, f"Árvores de Busca Binária — {tipo_analise}", transform=ax.transAxes,
             fontsize=9, fontweight='bold', color='#2563eb',
             bbox=dict(boxstyle='round,pad=0.4', facecolor='#dbeafe', edgecolor='none'))

    plt.text(0.0, 1.05, f"Dataset {dataset} ({operacao.capitalize()}): BST vs. AVL, Treap e Splay", transform=ax.transAxes,
             fontsize=14, fontweight='bold', color='#0f172a')

    if subtitulo:
        texto_sub = subtitulo
    elif dataset.lower() == 'aleatorio':
        texto_sub = r"Comportamento de caso médio com tempo $O(\log N)$ para estruturas auto-balanceáveis e Splay"
    else:
        texto_sub = r"BST degenera em lista encadeada $O(N^2)$; auto-balanceáveis mantêm $O(N \log N)$"

    plt.text(0.0, 1.01, texto_sub, transform=ax.transAxes, fontsize=9.5, color='#64748b')

    caminho_saida = os.path.join(PASTA_SAIDA, nome_saida)
    plt.savefig(caminho_saida, bbox_inches='tight', facecolor=fig.get_facecolor())
    plt.close()
    print(f"[+] Gráfico estilizado gerado com sucesso em: {caminho_saida}")

# ==============================================================================
# KD-TREE: ANÁLISE DE DIMENSÕES (2, 3 e 4)
# ==============================================================================
CORES_DIMENSOES = {
    2: '#2563eb',  # Azul
    3: '#16a34a',  # Verde
    4: '#ea580c'   # Laranja
}

def plotar_kdtree_dimensoes(df, operacao='Insercao', nome_saida='kdtree_desempenho_dimensoes.png'):
    sub_df = df.copy()

    col_dim = None
    for col in sub_df.columns:
        if sub_df[col].astype(str).str.contains("Dimensao=", case=False, na=False).any():
            col_dim = col
            break

    if col_dim is None:
        print("[-] Erro: Coluna com formato 'Dimensao=X' não foi encontrada no CSV.")
        return

    sub_df['dim_num'] = sub_df[col_dim].astype(str).str.extract(r'(\d+)').astype(float)

    sub_df = sub_df[(sub_df['estrutura'].str.upper() == 'KDTREE') & 
                    (sub_df['operacao'].str.lower() == operacao.lower())]

    sub_df = sub_df[sub_df['dim_num'].isin([2, 3, 4])]

    if sub_df.empty:
        print(f"[-] Nenhum dado encontrado para a operação '{operacao}' nas dimensões 2, 3 e 4.")
        return

    if 'tempomedio_ns' in sub_df.columns:
        sub_df['ValorExibicao'] = sub_df['tempomedio_ns'] / 1e9
        unidade_tempo = "s"
    elif 'tempomedio' in sub_df.columns:
        sub_df['ValorExibicao'] = sub_df['tempomedio']
        unidade_tempo = "s"
    else:
        col_tempo = sub_df.columns[4]
        sub_df['ValorExibicao'] = sub_df[col_tempo]
        unidade_tempo = "s"

    fig, ax = plt.subplots(figsize=(11, 6.5), dpi=300)
    fig.patch.set_facecolor('#f8fafc')
    ax.set_facecolor('#f8fafc')

    dimensoes_presentes = sorted(sub_df['dim_num'].unique())

    for dim in [2, 3, 4]:
        if dim not in dimensoes_presentes:
            continue
            
        dados_dim = sub_df[sub_df['dim_num'] == dim]
        dados_agrupados = dados_dim.groupby('tamanhon', as_index=False)['ValorExibicao'].mean().sort_values('tamanhon')
        
        cor = CORES_DIMENSOES.get(int(dim), '#333333')
        
        ax.plot(
            dados_agrupados['tamanhon'], 
            dados_agrupados['ValorExibicao'], 
            color=cor,
            marker='o',
            linestyle='-',
            linewidth=2.2,
            markersize=6,
            label=f'Dimensão K = {int(dim)}'
        )

    ax.set_xscale('log')
    ax.set_yscale('log')

    ax.set_xlabel('Tamanho da Entrada N (número de pontos)', fontsize=11, fontweight='bold', color='#334155', labelpad=10)
    ax.set_ylabel(f'Tempo Médio de {operacao.capitalize()} ({unidade_tempo}) — escala $\log_{{10}}$', 
                  fontsize=11, fontweight='bold', color='#334155', labelpad=10)

    ax.grid(True, which="both", linestyle=":", color="#e2e8f0", linewidth=1.0)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_color('#cbd5e1')
    ax.spines['bottom'].set_color('#cbd5e1')
    ax.tick_params(colors='#64748b', labelsize=9)

    legend = ax.legend(
        loc='upper left', 
        frameon=True, 
        facecolor='white', 
        edgecolor='#e2e8f0',
        fontsize=10,
        ncol=1,
        borderpad=1,
        handletextpad=0.8
    )
    legend.get_frame().set_boxstyle('Round,pad=0.5,rounding_size=0.4')

    plt.text(0.0, 1.12, "Estruturas Multidimensionais — Análise de Desempenho", transform=ax.transAxes,
             fontsize=9, fontweight='bold', color='#8b5cf6',
             bbox=dict(boxstyle='round,pad=0.4', facecolor='#f3e8ff', edgecolor='none'))

    plt.text(0.0, 1.05, f"KD-Tree: Tempo de {operacao.capitalize()} por Dimensão (K = 2, 3, 4)", transform=ax.transAxes,
             fontsize=14, fontweight='bold', color='#0f172a')

    plt.text(0.0, 1.01, r"Comparativo do tempo de execução variando a dimensão $K$ no espaço multidimensional", transform=ax.transAxes,
             fontsize=9.5, color='#64748b')

    caminho_saida = os.path.join(PASTA_SAIDA, nome_saida)
    plt.savefig(caminho_saida, bbox_inches='tight', facecolor=fig.get_facecolor())
    plt.close()
    print(f"[+] Gráfico KD-Tree (K=2,3,4) gerado com sucesso em: {caminho_saida}")

# ==============================================================================
# TRIE VS PATRICIA: COMPARATIVO DE DESEMPENHO
# ==============================================================================
ESTILOS_PREFIXO = {
    'Trie': {
        'color': '#ef4444',      # Vermelho
        'marker': 'o',
        'linestyle': '-',
        'label': 'Trie Tradicional'
    },
    'Patricia': {
        'color': '#06b6d4',      # Ciano / Azul Turquesa
        'marker': 's',
        'linestyle': '-',
        'label': 'Árvore Patricia (Compressed Trie)'
    }
}

def plotar_trie_vs_patricia(df, dataset='Aleatorio', operacao='Busca', 
                           nome_saida='trie_vs_patricia.png'):
    
    sub_df = df[(df.iloc[:, 0].str.lower() == dataset.lower()) & 
                (df.iloc[:, 2].str.lower() == operacao.lower())].copy()

    if sub_df.empty:
        print(f"[-] Sem dados para Dataset={dataset} e Operacao={operacao}")
        return

    col_tempo_medio_ns = sub_df.columns[5]
    sub_df['ValorExibicao'] = pd.to_numeric(sub_df[col_tempo_medio_ns], errors='coerce') / 1e9

    sub_df = sub_df[sub_df['ValorExibicao'] > 0]

    fig, ax = plt.subplots(figsize=(11, 6.5), dpi=300)
    fig.patch.set_facecolor('#f8fafc')
    ax.set_facecolor('#f8fafc')

    estruturas_alvo = ['TRIE', 'PATRICIA']
    
    for estr in estruturas_alvo:
        dados_estr = sub_df[sub_df.iloc[:, 1].str.upper() == estr]

        if not dados_estr.empty:
            col_n = dados_estr.columns[3]
            dados_agrupados = dados_estr.groupby(col_n, as_index=False)['ValorExibicao'].mean().sort_values(col_n)
            
            nome_chave = 'Patricia' if estr == 'PATRICIA' else 'Trie'
            estilo = ESTILOS_PREFIXO[nome_chave]
            
            ax.plot(
                dados_agrupados[col_n], 
                dados_agrupados['ValorExibicao'], 
                color=estilo['color'],
                marker=estilo['marker'],
                linestyle=estilo['linestyle'],
                linewidth=2.2,
                markersize=6,
                label=estilo['label']
            )

    ax.set_xscale('log')
    ax.set_yscale('log')

    ax.set_xlabel('Tamanho da Entrada N (número de chaves)', fontsize=11, fontweight='bold', color='#334155', labelpad=10)
    ax.set_ylabel(f'Tempo Médio por {operacao.capitalize()} (s) — escala $\log_{{10}}$', 
                  fontsize=11, fontweight='bold', color='#334155', labelpad=10)

    ax.grid(True, which="both", linestyle=":", color="#e2e8f0", linewidth=1.0)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['left'].set_color('#cbd5e1')
    ax.spines['bottom'].set_color('#cbd5e1')
    ax.tick_params(colors='#64748b', labelsize=9)

    legend = ax.legend(
        loc='upper left', 
        frameon=True, 
        facecolor='white', 
        edgecolor='#e2e8f0',
        fontsize=10,
        ncol=1,
        borderpad=1,
        handletextpad=0.8
    )
    legend.get_frame().set_boxstyle('Round,pad=0.5,rounding_size=0.4')

    plt.text(0.0, 1.12, "Estruturas Digitais de Prefixo — Análise Comparativa", transform=ax.transAxes,
             fontsize=9, fontweight='bold', color='#06b6d4',
             bbox=dict(boxstyle='round,pad=0.4', facecolor='#cff4fc', edgecolor='none'))

    plt.text(0.0, 1.05, f"Trie Simples vs. Árvore Patricia: Tempo de {operacao.capitalize()}", transform=ax.transAxes,
             fontsize=14, fontweight='bold', color='#0f172a')

    plt.text(0.0, 1.01, r"Impacto da compactação de caminhos unários no tempo médio por operação", transform=ax.transAxes,
             fontsize=9.5, color='#64748b')

    caminho_saida = os.path.join(PASTA_SAIDA, nome_saida)
    plt.savefig(caminho_saida, bbox_inches='tight', facecolor=fig.get_facecolor())
    plt.close()
    print(f"[+] Gráfico Trie vs. Patricia gerado com sucesso em: {caminho_saida}")

# ==============================================================================
# TABELA COM OS DADOS GERAIS DAS ESTRUTURAS
# ==============================================================================
def gerar_tabela_todas_estruturas(caminho_csv='data/output/resultado_dados_comparativos.csv', caminho_saida='data/output/img/graficos/tabela_geral_todas_estruturas.png'):
    df = pd.read_csv(caminho_csv)

    complexidades = {
        'BST': 'O(h) / O(N)',
        'AVL': 'O(log N)',
        'Splay': 'O(log N) amortiz.',
        'Treap': 'O(log N) med.',
        'Trie': 'O(K)',
        'Patricia': 'O(K)',
        'KDTree': 'O(log N) / O(K)'
    }

    resumo = df.groupby(['Estrutura', 'Operacao'])['TempoMedio_ns'].mean().unstack()

    dados_tabela = [
        ['Estrutura', 'Complexidade', 'Inserção (ns)', 'Busca (ns)', 'Remoção (ns)']
    ]

    estruturas = ['BST', 'AVL', 'Splay', 'Treap', 'Trie', 'Patricia', 'KDTree']

    for est in estruturas:
        if est in resumo.index:
            ins = f"{resumo.loc[est, 'Insercao']:.2f}" if 'Insercao' in resumo.columns else '-'
            bus = f"{resumo.loc[est, 'Busca']:.2f}" if 'Busca' in resumo.columns else '-'
            rem = f"{resumo.loc[est, 'Remocao']:.2f}" if 'Remocao' in resumo.columns else '-'
            comp = complexidades.get(est, 'O(log N)')
            
            dados_tabela.append([est, comp, ins, bus, rem])

    fig, ax = plt.subplots(figsize=(11, 5.5), dpi=300)
    fig.patch.set_facecolor('#f8fafc')
    ax.set_facecolor('#f8fafc')
    ax.axis('off')

    tabela = ax.table(cellText=dados_tabela, loc='center', cellLoc='center')
    tabela.auto_set_font_size(False)
    tabela.set_fontsize(9.5)
    tabela.scale(1.2, 1.9)

    for (row, col), cell in tabela.get_celld().items():
        cell.set_edgecolor('#cbd5e1')
        cell.set_linewidth(1.0)
        
        if row == 0:
            cell.set_facecolor('#0f172a')
            cell.get_text().set_color('white')
            cell.get_text().set_weight('bold')
        elif col == 0:
            cell.set_facecolor('#f1f5f9')
            cell.get_text().set_color('#334155')
            cell.get_text().set_weight('bold')
        else:
            cell.set_facecolor('white')
            cell.get_text().set_color('#1e293b')

    plt.title('Consolidação Geral do Benchmark: Todas as Estruturas de Dados', 
              fontsize=13, fontweight='bold', color='#0f172a', pad=20)

    plt.savefig(caminho_saida, bbox_inches='tight', facecolor=fig.get_facecolor())
    plt.close()
    print(f"[+] Tabela gerada com sucesso em: {caminho_saida}")

# ==============================================================================
# EXECUÇÃO PRINCIPAL (MAIN)
# ==============================================================================
def main():
    df = carregar_dados("data/output/resultado_dados_comparativos.csv")

    plot_metrica_geral(df, "comparacoes", "geral_comparacoes.png",
                       "Número de comparações na inserção — dataset aleatório",
                       "Comparações", estruturas=TODAS_ESTRUTURAS, escala_log=True)

    plot_metrica_geral(df, "rotacoes", "geral_rotacoes.png",
                       "Número de rotações na inserção — dataset aleatório",
                       "Rotações", estruturas=["AVL", "Splay", "Treap"], escala_log=False)

    plot_metrica_geral(df, "memoria", "geral_memoria.png",
                       "Consumo de memória após inserção — dataset aleatório",
                       "Memória (bytes)", estruturas=TODAS_ESTRUTURAS, escala_log=True)

    plot_metrica_geral(df, "altura", "geral_altura.png",
                       "Altura da árvore após inserção — dataset aleatório",
                       "Altura", estruturas=TODAS_ESTRUTURAS, escala_log=True)

    plot_teoria_vs_pratica_grid(df, operacao="Busca", dataset="Aleatorio", 
                                nome_arquivo="teoria_vs_pratica_busca_aleatorio.png")
                                

    plotar_grafico_estilizado(df, dataset='Ordenado', operacao='Insercao', nome_saida='pior_caso_ordenado.png')
    plotar_grafico_estilizado(df, dataset='Aleatorio', operacao='Insercao', nome_saida='caso_medio_aleatorio_insercao.png')
    plotar_grafico_estilizado(df, dataset='Decrescente', operacao='Insercao', nome_saida='pior_caso_decrescente.png')

    plotar_grafico_estilizado(df, dataset='Aleatorio', operacao='Busca', nome_saida='caso_medio_aleatorio_busca.png')
    plotar_grafico_estilizado(df, dataset='Aleatorio', operacao='Remocao', nome_saida='caso_medio_aleatorio_remocao.png')

    plotar_kdtree_dimensoes(df, operacao='Insercao', nome_saida='kdtree_insercao_dimensoes.png')
    plotar_kdtree_dimensoes(df, operacao='Busca', nome_saida='kdtree_busca_dimensoes.png')

    plotar_trie_vs_patricia(df, dataset='Ordenado', operacao='Insercao', nome_saida='trie_vs_patricia_insercao.png')
    plotar_trie_vs_patricia(df, dataset='Ordenado', operacao='Busca', nome_saida='trie_vs_patricia_busca.png')
    plotar_trie_vs_patricia(df, dataset='Ordenado', operacao='Remocao', nome_saida='trie_vs_patricia_remocao.png')

    gerar_tabela_todas_estruturas()

if __name__ == "__main__":
    main()