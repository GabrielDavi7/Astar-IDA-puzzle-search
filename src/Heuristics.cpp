#include "Heuristics.hpp"
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>

int manhattan_distance(const std::vector<int>& tabuleiro) { 
    int distancia = 0;
    int lado = (tabuleiro.size() == 9) ? 3 : 4; 

    for (size_t i = 0; i < tabuleiro.size(); ++i) {
        int valor = tabuleiro[i];
        
        if (valor != 0) { 
            int posXAtual = i % lado;
            int posYAtual = i / lado;

            // O objetivo do valor é a sua própria posição (alvo 0 1 2 ... 15)
            int posXObjetivo = valor % lado;
            int posYObjetivo = valor / lado;

            distancia += std::abs(posXAtual - posXObjetivo) + std::abs(posYAtual - posYObjetivo);
        }
    }
    return distancia;
}

int conflict_linear(const std::vector<int>& tabuleiro) {
    int pecas_removidas = 0;
    int lado = (tabuleiro.size() == 9) ? 3 : 4; 

    // 1. Analisando Conflitos nas LINHAS
    for (int linha = 0; linha < lado; ++linha) {
        std::vector<int> linha_atual;
        for (int col = 0; col < lado; ++col) {
            int val = tabuleiro[linha * lado + col];
            // Verifica se a peça pertence a esta linha e não é o zero
            if (val != 0 && (val / lado) == linha) {
                linha_atual.push_back(val);
            }
        }

        bool resolvido = false;
        while (!resolvido) {
            int max_conflitos = 0;
            int idx_pior_peca = -1;
            std::vector<int> conflitos_por_peca(linha_atual.size(), 0);

            for (size_t i = 0; i < linha_atual.size(); ++i) {
                if (linha_atual[i] == -1) continue; // -1 significa que a peça já foi "removida"

                for (size_t j = i + 1; j < linha_atual.size(); ++j) {
                    if (linha_atual[j] == -1) continue;

                    // Se estão na ordem errada para o objetivo (objetivo é val1 < val2)
                    if (linha_atual[i] > linha_atual[j]) {
                        conflitos_por_peca[i]++;
                        conflitos_por_peca[j]++;
                    }
                }
                if (conflitos_por_peca[i] > max_conflitos) {
                    max_conflitos = conflitos_por_peca[i];
                    idx_pior_peca = i;
                }
            }

            if (max_conflitos > 0) {
                linha_atual[idx_pior_peca] = -1; // Remove a peça que causa mais conflitos
                pecas_removidas++;
            } else {
                resolvido = true; // Linha livre de conflitos
            }
        }
    }

    // 2. Analisando Conflitos nas COLUNAS
    for (int col = 0; col < lado; ++col) {
        std::vector<int> coluna_atual;
        for (int linha = 0; linha < lado; ++linha) {
            int val = tabuleiro[linha * lado + col];
            // Verifica se a peça pertence a esta coluna e não é o zero
            if (val != 0 && (val % lado) == col) {
                coluna_atual.push_back(val);
            }
        }

        bool resolvido = false;
        while (!resolvido) {
            int max_conflitos = 0;
            int idx_pior_peca = -1;
            std::vector<int> conflitos_por_peca(coluna_atual.size(), 0);

            for (size_t i = 0; i < coluna_atual.size(); ++i) {
                if (coluna_atual[i] == -1) continue; 

                for (size_t j = i + 1; j < coluna_atual.size(); ++j) {
                    if (coluna_atual[j] == -1) continue;

                    if (coluna_atual[i] > coluna_atual[j]) {
                        conflitos_por_peca[i]++;
                        conflitos_por_peca[j]++;
                    }
                }
                if (conflitos_por_peca[i] > max_conflitos) {
                    max_conflitos = conflitos_por_peca[i];
                    idx_pior_peca = i;
                }
            }

            if (max_conflitos > 0) {
                coluna_atual[idx_pior_peca] = -1; 
                pecas_removidas++;
            } else {
                resolvido = true; 
            }
        }
    }

    // Retorna o Manhattan base somado à penalidade dos engarrafamentos (2 passos por peça removida)
    return manhattan_distance(tabuleiro) + (2 * pecas_removidas);
}