import os
import random
import math
import string

TOTAL_ELEMENTOS = 100_000
PASTA_SAIDA = "data/datasets"

os.makedirs(PASTA_SAIDA, exist_ok=True)

print(f"Gerando datasets em '{PASTA_SAIDA}' com {TOTAL_ELEMENTOS:,} elementos cada...\n")


# arquivos para KD (4D)
print("arquivos 4D para k-d Tree...")

# KD - Aleatório
with open(os.path.join(PASTA_SAIDA, "kd_aleatorio.dat"), "w") as f:
    for _ in range(TOTAL_ELEMENTOS):
        x = round(random.uniform(-1000.0, 1000.0), 4)
        y = round(random.uniform(-1000.0, 1000.0), 4)
        z = round(random.uniform(-1000.0, 1000.0), 4)
        w = round(random.uniform(-1000.0, 1000.0), 4)
        f.write(f"{x},{y},{z},{w}\n")

# KD - Diagonal
with open(os.path.join(PASTA_SAIDA, "kd_diagonal.dat"), "w") as f:
    for i in range(TOTAL_ELEMENTOS):
        val = round(i * 0.01 + random.uniform(-0.001, 0.001), 4)
        f.write(f"{val},{val},{val},{val}\n")

# KD - Clusterizado
num_clusters = 10
centroides = [[random.uniform(-500, 500) for _ in range(4)] for _ in range(num_clusters)]

with open(os.path.join(PASTA_SAIDA, "kd_clusterizado.dat"), "w") as f:
    for _ in range(TOTAL_ELEMENTOS):
        c = random.choice(centroides)
        x = round(c[0] + random.gauss(0, 15.0), 4)
        y = round(c[1] + random.gauss(0, 15.0), 4)
        z = round(c[2] + random.gauss(0, 15.0), 4)
        w = round(c[3] + random.gauss(0, 15.0), 4)
        f.write(f"{x},{y},{z},{w}\n")



# arquivos para inteiros
print("arquivos de inteiros...")

# Ordenado (Sem duplicatas)
with open(os.path.join(PASTA_SAIDA, "int_ordenado.dat"), "w") as f:
    for i in range(1, TOTAL_ELEMENTOS + 1):
        f.write(f"{i}\n")

# Decrescente (Sem duplicatas)
with open(os.path.join(PASTA_SAIDA, "int_decrescente.dat"), "w") as f:
    for i in range(TOTAL_ELEMENTOS, 0, -1):
        f.write(f"{i}\n")

# Aleatório (Com 10% de elementos duplicados, mantendo 100.000 linhas)
qtd_unicos_int = int(TOTAL_ELEMENTOS * 0.9)  
qtd_duplicados_int = TOTAL_ELEMENTOS - qtd_unicos_int  

conjunto_ints = set()
limite_maximo = int(TOTAL_ELEMENTOS * 1.5)

while len(conjunto_ints) < qtd_unicos_int:
    conjunto_ints.add(random.randint(1, limite_maximo))

lista_ints_unicos = list(conjunto_ints)
duplicatas_int = random.choices(lista_ints_unicos, k=qtd_duplicados_int)

lista_ints_final = lista_ints_unicos + duplicatas_int
random.shuffle(lista_ints_final)

with open(os.path.join(PASTA_SAIDA, "int_aleatorio.dat"), "w") as f:
    for num in lista_ints_final:
        f.write(f"{num}\n")



# arquivos de string (Apenas 'a' ate 'z')
print("arquivos de strings (apenas letras a-z)...")

PREFIXOS = [
    "computacaocomputador", "algoritmobusca", "sistemadistribuido",
    "engenhariasoftware", "estruturadados", "programacaocpp",
    "bancodedadosrelacional", "inteligenciaartificial", "redesdecomputadores",
    "arquiteturadeprocessadores"
]

SUFIXOS_BASE = ["base", "core", "test", "node", "tree", "item", "data", "val", "elem", "key"]

conjunto_strings = set()
letras_az = string.ascii_lowercase  # 'a' ate 'z'

contador = 0
while len(conjunto_strings) < TOTAL_ELEMENTOS:
    prefixo = PREFIXOS[contador % len(PREFIXOS)]
    sufixo_base = SUFIXOS_BASE[(contador // len(PREFIXOS)) % len(SUFIXOS_BASE)]
    
    idx = contador // (len(PREFIXOS) * len(SUFIXOS_BASE))
    sufixo_letras = ""
    temp = idx
    while True:
        sufixo_letras = letras_az[temp % 26] + sufixo_letras
        temp = temp // 26
        if temp == 0:
            break
            
    palavra = f"{prefixo}{sufixo_base}{sufixo_letras}"
    conjunto_strings.add(palavra)
    contador += 1

lista_strings = list(conjunto_strings)

# string - ordenada 
lista_strings.sort()
with open(os.path.join(PASTA_SAIDA, "string_ordenado.dat"), "w") as f:
    for st in lista_strings:
        f.write(f"{st}\n")

# string - decrescente 
lista_strings.sort(reverse=True)
with open(os.path.join(PASTA_SAIDA, "string_decrescente.dat"), "w") as f:
    for st in lista_strings:
        f.write(f"{st}\n")

# string - aleatória (Com 10% de elementos duplicados, mantendo 100.000 linhas)
lista_strings_embaralhada = lista_strings.copy()
random.shuffle(lista_strings_embaralhada)

qtd_unicos_str = int(TOTAL_ELEMENTOS * 0.9)  
qtd_duplicados_str = TOTAL_ELEMENTOS - qtd_unicos_str  

lista_strings_unicas = lista_strings_embaralhada[:qtd_unicos_str]
duplicatas_str = random.choices(lista_strings_unicas, k=qtd_duplicados_str)

lista_strings_final = lista_strings_unicas + duplicatas_str
random.shuffle(lista_strings_final)

with open(os.path.join(PASTA_SAIDA, "string_aleatorio.dat"), "w") as f:
    for st in lista_strings_final:
        f.write(f"{st}\n")

print("\nTodos os 9 arquivos foram gerados com sucesso no diretório 'data/datasets/'!")