#include "IDAStar.hpp"
#include "Heuristics.hpp"
#include <algorithm>
#include <limits>
#include <cmath>
#include <utility>

namespace {
    const int ENCONTRADO = -100; 
    const int NAO_ENCONTRADO = std::numeric_limits<int>::max(); 

    int busca(estado& atual, int limite, int posVazioAnterior, int& nosExpandidos) {
        int f = atual.custo + atual.heuristica;
        if (f > limite) return f; 
        if (atual.is_objetivo()) return ENCONTRADO; 

        int minLimite = NAO_ENCONTRADO;
        nosExpandidos++; 

        int posVazio = atual.posVazio;
        int tamanho = atual.tabuleiro.size();
        int lado = std::sqrt(tamanho);
        int movimentos[4] = {-lado, lado, -1, 1};

        for (int i = 0; i < 4; ++i) {
            int move = movimentos[i];
            int novaPosVazio = posVazio + move;

            if (novaPosVazio < 0 || novaPosVazio >= tamanho) continue;
            if (move == -1 && posVazio % lado == 0) continue; 
            if (move == 1 && (posVazio + 1) % lado == 0) continue; 
            if (novaPosVazio == posVazioAnterior) continue;

            std::swap(atual.tabuleiro[posVazio], atual.tabuleiro[novaPosVazio]);
            atual.posVazio = novaPosVazio;
            atual.custo++;
            
            int heuristicaAntiga = atual.heuristica;
            if (atual.tipoHeuristica == 1) atual.heuristica = conflict_linear(atual.tabuleiro);
            else atual.heuristica = pattern_database(atual.tabuleiro); 

            int resultado = busca(atual, limite, posVazio, nosExpandidos);

            std::swap(atual.tabuleiro[posVazio], atual.tabuleiro[novaPosVazio]);
            atual.posVazio = posVazio;
            atual.custo--;
            atual.heuristica = heuristicaAntiga;

            if (resultado == ENCONTRADO) return ENCONTRADO; 
            minLimite = std::min(minLimite, resultado); 
        }
        return minLimite; 
    }
} 

int execIDAStar(const std::vector<int>& estadoInicial, int tipoHeuristica, int& nosExpandidosRetorno) {
    int posVazio = -1;
    for (size_t i = 0; i < estadoInicial.size(); ++i) {
        if (estadoInicial[i] == 0) {
            posVazio = i;
            break;
        }
    }

    int inicialHeuristica = (tipoHeuristica == 1) ? conflict_linear(estadoInicial) : pattern_database(estadoInicial);
    estado inicial(estadoInicial, posVazio, 0, inicialHeuristica, tipoHeuristica);
    int limite = inicial.heuristica;
    int nosExpandidos = 0;

    while (true) {
        int resultado = busca(inicial, limite, -1, nosExpandidos);
        
        if (resultado == ENCONTRADO) {
            nosExpandidosRetorno = nosExpandidos; // Exporta para a main
            return limite;
        }
        if (resultado == NAO_ENCONTRADO) {
            nosExpandidosRetorno = nosExpandidos;
            return -1; 
        }
        limite = resultado; 
    }
}