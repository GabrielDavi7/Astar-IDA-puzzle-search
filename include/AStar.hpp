#ifndef ASTAR_HPP
#define ASTAR_HPP

#include "Puzzle.hpp"
#include <vector>

int execAstar(const std::vector<int>& estadoInicial, int tipoHeuristica, int& nosExpandidosRetorno, std::vector<std::vector<int>>& caminhoRetorno, bool guardarCaminho);
#endif // ASTAR_HPP