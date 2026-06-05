#include "Heuristics.hpp"
#include <cmath>
#include <cstdlib>

// Calcula a distância de Manhattan para o tabuleiro atual a distancia de manhattan é a soma das distâncias horizontais e verticais de cada peça para sua posição correta. 
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