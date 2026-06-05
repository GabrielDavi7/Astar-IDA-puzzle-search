//Arquivo aStar implementa o algoritmo A* para resolver o quebra-cabeça. Ele utiliza uma fila de prioridade para explorar os estados do quebra-cabeça, 
//onde a prioridade é determinada pela soma do custo do caminho percorrido e da heurística (neste caso, a distância de Manhattan). 
//O algoritmo continua expandindo os estados até encontrar o estado objetivo ou esgotar todas as possibilidades. O número de nós expandidos é contado e exibido ao final da execução.

#include "AStar.hpp"
#include "Heuristics.hpp"
#include <set>
#include <queue>
#include <vector>
#include <iostream>

int execAstar(const std::vector<int>& estadoInicial) {

    std::priority_queue<estado> naFila;
    std::set<std::vector<int>> visitados;

    int posVazio = -1;
    for (size_t i = 0; i < estadoInicial.size(); ++i) {
        if (estadoInicial[i] == 0) {
            posVazio = i;
            break;
        }
    }

    int inicialHeuristica = manhattan_distance(estadoInicial);
    estado inicial(estadoInicial, posVazio, 0, inicialHeuristica);
    naFila.push(inicial);

    int nosExpandidos = 0;

    while(!naFila.empty()) { //enquanto a fila de prioridade não estiver vazia
        estado atual = naFila.top();
        naFila.pop();

        //verifica se chegamos no estado objetivo
        if (atual.is_objetivo()) {
            std::cout << "Resolvido pelo Astar, Nos expandidos: " << nosExpandidos << std::endl;
            return atual.custo; 
        }
        if (!visitados.insert(atual.tabuleiro).second) {
            continue; 
        }
        nosExpandidos++;

        std::vector<estado> vizinhos = atual.gerarVizinhos(); //cria um vetor de estados vizinhos a partir do estado atual
        for(estado& vizinho : vizinhos) {
            if (visitados.find(vizinho.tabuleiro) != visitados.end()) { //verifica se o estado vizinho já foi visitado
                continue; 
            }
            vizinho.heuristica = manhattan_distance(vizinho.tabuleiro); //calcula a heurística para o estado vizinho
            naFila.push(vizinho); //adiciona o estado vizinho à fila de prioridade
        }
    }
    //devemos retornar -1 para caso não seja possivel resolver o quebra-cabeça.
    return -1;
}