#include "IDAStar.hpp"
#include "Heuristics.hpp"
#include <iostream>
#include <algorithm>

const int ENCONTRADO = -100; 
const int NAO_ENCONTRADO = 999999;

int busca(const estado& atual, int limite, int posVazioAnterior, int& nosExpandidos) {
    int f = atual.custo + atual.heuristica;
    
    if (f > limite) {
        return f; 
    }
    if (atual.is_objetivo()) {
        return ENCONTRADO; 
    }

    int minLimite = NAO_ENCONTRADO;
    nosExpandidos++; // Conta o nó apenas quando ele é realmente processado

    std::vector<estado> vizinhos = atual.gerarVizinhos();
    for (estado& vizinho : vizinhos) {
        
        if (vizinho.posVazio == posVazioAnterior) {
            continue;
        }

        vizinho.heuristica = manhattan_distance(vizinho.tabuleiro);
        
        int resultado = busca(vizinho, limite, atual.posVazio, nosExpandidos);
        
        if (resultado == ENCONTRADO) {
            return ENCONTRADO; 
        }
        minLimite = std::min(minLimite, resultado); 
    }
    return minLimite; 
}

int execIDAStar(const std::vector<int>& estadoInicial) {
    int posVazio = -1;
    for (size_t i = 0; i < estadoInicial.size(); ++i) {
        if (estadoInicial[i] == 0) {
            posVazio = i;
            break;
        }
    }

    estado inicial(estadoInicial, posVazio, 0, manhattan_distance(estadoInicial));
    int limite = inicial.heuristica;
    int nosExpandidos = 0;

    while (true) {
        int resultado = busca(inicial, limite, -1, nosExpandidos);
        
        if (resultado == ENCONTRADO) {
            std::cout << "Resolvido pelo IDA*, Nos expandidos: " << nosExpandidos << std::endl;
            return limite;
        }
        if (resultado == NAO_ENCONTRADO) {
            return -1; 
        }
        limite = resultado; 
    }
}