#include "AStar.hpp"
#include "Heuristics.hpp"
#include <set>
#include <queue>
#include <vector>

int execAstar(const std::vector<int>& estadoInicial, int tipoHeuristica, int& nosExpandidosRetorno) {
    std::priority_queue<estado> naFila;
    std::set<std::vector<int>> visitados;

    int posVazio = -1;
    for (size_t i = 0; i < estadoInicial.size(); ++i) {
        if (estadoInicial[i] == 0) {
            posVazio = i;
            break;
        }
    }
    
    int inicialHeuristica = (tipoHeuristica == 1) ? conflict_linear(estadoInicial) : pattern_database(estadoInicial);
    estado inicial(estadoInicial, posVazio, 0, inicialHeuristica, tipoHeuristica);
    naFila.push(inicial);

    int nosExpandidos = 0;

    while(!naFila.empty()) { 
        estado atual = naFila.top();
        naFila.pop();

        if (atual.is_objetivo()) {
            nosExpandidosRetorno = nosExpandidos; // Exporta os nós para a main
            return atual.custo; 
        }
        
        if (!visitados.insert(atual.tabuleiro).second) continue; 
        
        nosExpandidos++;

        std::vector<estado> vizinhos = atual.gerarVizinhos(); 
        for(estado& vizinho : vizinhos) {
            if (visitados.find(vizinho.tabuleiro) != visitados.end()) continue; 
            naFila.push(vizinho); 
        }
    }
    
    nosExpandidosRetorno = nosExpandidos;
    return -1;
}