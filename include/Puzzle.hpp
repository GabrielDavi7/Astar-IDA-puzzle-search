#ifndef PUZZLE_HPP
#define PUZZLE_HPP

#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

struct estado {
    std::vector<int> tabuleiro;
    int posVazio; 
    int custo; 
    int heuristica; 

    estado(const std::vector<int>& tabuleiro, int posVazio, int custoEstado, int custoHeuristica)
        : tabuleiro(tabuleiro), posVazio(posVazio), custo(custoEstado), heuristica(custoHeuristica) {}

    bool operator<(const estado& outro) const {
        int f_atual = custo + heuristica;
        int f_Outro = outro.custo + outro.heuristica;

        if (f_atual == f_Outro) {
            return custo < outro.custo; 
        }

        return f_atual > f_Outro;
    }

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
                
                std::swap(novoTabuleiro[posVazio], novoTabuleiro[novaPosVazio]);
                
                vizinhos.emplace_back(novoTabuleiro, novaPosVazio, custo + 1, 0); 
            }
        }
        return vizinhos;
    }

    bool is_objetivo() const {
        for (size_t i = 0; i < tabuleiro.size() - 1; ++i) {
            if (tabuleiro[i] != static_cast<int>(i + 1)) {
                return false;
            }
        }
        return tabuleiro.back() == 0; 
    }
};

#endif