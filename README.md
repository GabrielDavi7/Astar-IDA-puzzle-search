# 🧩 Busca Heurística: Solucionador Ótimo para 8-Puzzle e 15-Puzzle

Este repositório contém a implementação em **C++** de um solucionador ótimo para os clássicos problemas do 8-Puzzle e 15-Puzzle. O projeto utiliza algoritmos de busca informada em grafos combinados com heurísticas avançadas para lidar com o problema da explosão combinatória do espaço de estados (NP-Difícil).

Este projeto foi desenvolvido como parte da disciplina de Algoritmos para Grafos no Instituto Federal do Norte de Minas Gerais (IFNMG) - Campus Montes Claros.

---

## 🚀 Funcionalidades e Algoritmos

O motor de resolução foi projetado com foco em máxima eficiência de processamento e uso inteligente de memória (Space-Time Tradeoff).

- **Algoritmos de Busca:**
  - **A\* (A-Star):** Busca heurística com armazenamento completo da fronteira.
  - **IDA* (Iterative Deepening A*):** Busca em profundidade com aprofundamento iterativo, ideal para o 15-Puzzle por possuir complexidade espacial O(L).
- **Heurísticas Admissíveis:**
  - **Distância de Manhattan + Conflito Linear:** Cálculo em tempo real que penaliza peças fora do lugar e engarrafamentos na mesma linha/coluna.
  - **Disjoint Pattern Databases (PDBs):** Um oráculo de pré-computação que grava os caminhos ótimos exatos para subconjuntos de peças em memória primária, operando com custo de consulta O(1).
- **Modo Analítico e Visual:** * Resolução em lote de 100 instâncias para *benchmarking\*.
  - Reconstrução visual de matrizes passo a passo para auditoria de caminhos.

---

## 📊 Destaques de Desempenho

A implementação da heurística híbrida (usando `std::max` entre PDB e Conflito Linear) obteve resultados massivos na poda da árvore de busca:

| Métrica Média            | Manhattan + Conflito | PDB Híbrido | Redução (Poda) |
| :----------------------- | :------------------- | :---------- | :------------- |
| **8-Puzzle (Nós Exp.)**  | 1.123,74             | 22,16       | **-98,03%**    |
| **15-Puzzle (Nós Exp.)** | 22.216.800           | 12.826.900  | **-42,26%**    |

O uso do PDB demonstrou o poder do **Custo Amortizado**: ao sacrificar ~11 MB de RAM e tempo de I/O na primeira execução para gerar o Banco de Padrões, o sistema consolidou uma economia de tempo superior a **23%** na resolução bruta do 15-Puzzle.

---

## 🛠️ Estrutura do Projeto

    ├── include/           # Headers (.hpp) contendo as assinaturas de A*, IDA* e Heurísticas
    ├── src/               # Código-fonte (.cpp) principal do sistema
    ├── input/             # Ficheiros .txt contendo as instâncias de teste
    ├── Makefile           # Script de compilação automatizada
    └── README.md          # Documentação do projeto

---

## ⚙️ Como Compilar e Executar

**Pré-requisitos:**

- Compilador C++ (GCC/MinGW recomendado)
- `make` instalado

**Passos:**

1. Clone este repositório:

   ```bash
   git clone [https://github.com/GabrielDavi7/Astar-IDA-puzzle-search.git](https://github.com/GabrielDavi7/Astar-IDA-puzzle-search.git)
   ```

2. Navegue até o diretório e compile o projeto:

   ```bash
   cd Astar-IDA-puzzle-search
   make
   ```

3. Execute o solucionador:

   ```bash
   ./puzzle_solver
   ```

4. Siga as instruções do menu interativo no terminal para escolher o algoritmo, a heurística e as instâncias que deseja resolver.

---
