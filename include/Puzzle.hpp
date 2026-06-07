#ifndef PUZZLE_HPP
#define PUZZLE_HPP

#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

// Declaramos a função aqui para podermos calcular a nova heurística 
// ao gerar os vizinhos sem causar dependência circular de includes.
int conflict_linear(const std::vector<int>& tabuleiro);

struct estado {
    std::vector<int> tabuleiro;
    int posVazio; 
    int custo; 
    int heuristica; 

    estado(const std::vector<int>& tabuleiro, int posVazio, int custoEstado, int custoHeuristica)
        : tabuleiro(tabuleiro), posVazio(posVazio), custo(custoEstado), heuristica(custoHeuristica) {}

    // Sobrecarga do operador < para garantir que a fila de prioridade do A* // funcione como um Min-Heap (tire sempre o menor custo F primeiro)
    bool operator<(const estado& outro) const {
        int f_atual = custo + heuristica;
        int f_Outro = outro.custo + outro.heuristica;

        if (f_atual == f_Outro) {
            return custo < outro.custo; 
        }

        return f_atual > f_Outro;
    }

    // Gera todos os movimentos possíveis a partir do estado atual
    std::vector<estado> gerarVizinhos() const {
        std::vector<estado> vizinhos;
        int lado = std::sqrt(tabuleiro.size());
        int linha = posVazio / lado;
        int coluna = posVazio % lado;

        const int dLinha[] = {-1, 1, 0, 0};
        const int dColuna[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; ++i) {
            int novaLinha = linha + dLinha[i];
            int novaColuna = coluna + dColuna[i];

            if (novaLinha >= 0 && novaLinha < lado && novaColuna >= 0 && novaColuna < lado) {
                int novaPosVazio = novaLinha * lado + novaColuna;
                std::vector<int> novoTabuleiro = tabuleiro;
                
                // Move a peça para o espaço vazio
                std::swap(novoTabuleiro[posVazio], novoTabuleiro[novaPosVazio]);
                
                // Calcula a heurística rigorosa (Manhattan + Conflitos Lineares)
                int novaHeuristica = conflict_linear(novoTabuleiro);
                
                vizinhos.emplace_back(novoTabuleiro, novaPosVazio, custo + 1, novaHeuristica); 
            }
        }
        return vizinhos;
    }

    // Verifica se o estado atual é a solução (0, 1, 2, 3...)
    bool is_objetivo() const {
        for (size_t i = 0; i < tabuleiro.size(); ++i) {
            if (tabuleiro[i] != static_cast<int>(i)) {
                return false;
            }
        }
        return true; 
    }
};

#endif