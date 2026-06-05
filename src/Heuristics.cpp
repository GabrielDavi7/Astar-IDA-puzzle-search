#include "Heuristics.hpp"
#include <cmath>
#include <cstdlib>

// Calcula a distância de Manhattan para o tabuleiro atual a distancia de manhattan conflito é a soma das distâncias horizontais e verticais de cada peça para sua posição correta. 
// Quanto maior a distância, mais longe o estado atual está do estado objetivo.

int manhattan_distance(const std::vector<int>& tabuleiro) { 
    int distancia = 0;
    int lado = (tabuleiro.size() == 9) ? 3 : 4; // Se o tamanho for 9, lado é 3. Senão, assumimos que é o 15-puzzle (lado 4).

    for (size_t i = 0; i < tabuleiro.size(); ++i) {
        int valor = tabuleiro[i];
        
        if (valor != 0) { 
            int posXAtual = i % lado;
            int posYAtual = i / lado;

            int posXObjetivo = (valor - 1) % lado;
            int posYObjetivo = (valor - 1) / lado;

            distancia += std::abs(posXAtual - posXObjetivo) + std::abs(posYAtual - posYObjetivo);
        }
    }
    return distancia;
}

int conflict_linear(const std::vector<int>& tabuleiro) {
    int conflitos = 0;
    int lado = (tabuleiro.size() == 9) ? 3 : 4; 
    int heuristicaManhattan = manhattan_distance(tabuleiro);

for (int linha = 0; linha < lado; ++linha) {
        for (int col1 = 0; col1 < lado - 1; ++col1) {
            for (int col2 = col1 + 1; col2 < lado; ++col2) {
                int p1 = tabuleiro[linha * lado + col1]; 
                int p2 = tabuleiro[linha * lado + col2]; 

                if (p1 != 0 && p2 != 0) {
                    int obj_p1 = p1 - 1;
                    int obj_p2 = p2 - 1;

                    if (obj_p1 / lado == linha && obj_p2 / lado == linha) {
                        if (obj_p1 > obj_p2) {
                            conflitos++;
                        }
                    }
                }
            }
        }
    }

    for (int coluna = 0; coluna < lado; ++coluna) {
        for (int lin1 = 0; lin1 < lado - 1; ++lin1) {
            for (int lin2 = lin1 + 1; lin2 < lado; ++lin2) {
                int p1 = tabuleiro[lin1 * lado + coluna]; 
                int p2 = tabuleiro[lin2 * lado + coluna];

                if (p1 != 0 && p2 != 0) {
                    int obj_p1 = p1 - 1;
                    int obj_p2 = p2 - 1;

                    if (obj_p1 % lado == coluna && obj_p2 % lado == coluna) {
                        if (obj_p1 > obj_p2) {
                            conflitos++;
                        }
                    }
                }
            }
        }
    }

    return heuristicaManhattan + (2 * conflitos);
}