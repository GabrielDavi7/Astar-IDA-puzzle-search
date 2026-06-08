#ifndef ASTAR_HPP
#define ASTAR_HPP

#include "Puzzle.hpp"
#include <vector>

int execAstar(const std::vector<int>& estadoInicial, int tipoHeuristica, int& nosExpandidosRetorno);
#endif // ASTAR_HPP