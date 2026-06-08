#include "AStar.hpp"
#include "Heuristics.hpp"
#include <set>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>

int execAstar(const std::vector<int>& estadoInicial, int tipoHeuristica, int& nosExpandidosRetorno, std::vector<std::vector<int>>& caminhoRetorno, bool guardarCaminho) {
    std::priority_queue<estado> naFila;
    std::set<std::vector<int>> visitados;
    std::map<std::vector<int>, std::vector<int>> parentMap; // O Mapa do GPS

    int posVazio = -1;
    for (size_t i = 0; i < estadoInicial.size(); ++i) {
        if (estadoInicial[i] == 0) { posVazio = i; break; }
    }
    
    int inicialHeuristica = (tipoHeuristica == 1) ? conflict_linear(estadoInicial) : pattern_database(estadoInicial);
    estado inicial(estadoInicial, posVazio, 0, inicialHeuristica, tipoHeuristica);
    naFila.push(inicial);

    int nosExpandidos = 0;

    while(!naFila.empty()) { 
        estado atual = naFila.top();
        naFila.pop();

        if (atual.is_objetivo()) {
            nosExpandidosRetorno = nosExpandidos;
            
            // Se pedimos o passo a passo, reconstruímos o caminho de trás para a frente
            if (guardarCaminho) {
                std::vector<int> passo = atual.tabuleiro;
                while (passo != estadoInicial) {
                    caminhoRetorno.push_back(passo);
                    passo = parentMap[passo];
                }
                caminhoRetorno.push_back(estadoInicial);
                std::reverse(caminhoRetorno.begin(), caminhoRetorno.end());
            }
            return atual.custo; 
        }
        
        if (!visitados.insert(atual.tabuleiro).second) continue; 
        
        nosExpandidos++;

        std::vector<estado> vizinhos = atual.gerarVizinhos(); 
        for(estado& vizinho : vizinhos) {
            if (visitados.find(vizinho.tabuleiro) != visitados.end()) continue; 
            
            // Grava a origem deste vizinho para depois reconstruirmos o caminho
            if (guardarCaminho && parentMap.find(vizinho.tabuleiro) == parentMap.end()) {
                parentMap[vizinho.tabuleiro] = atual.tabuleiro;
            }
            
            naFila.push(vizinho); 
        }
    }
    
    nosExpandidosRetorno = nosExpandidos;
    return -1;
}