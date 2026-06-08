#ifndef IDASTAR_HPP
#define IDASTAR_HPP

#include "Puzzle.hpp"
#include <vector>

int execIDAStar(const std::vector<int>& estadoInicial, int tipoHeuristica, int& nosExpandidosRetorno, std::vector<std::vector<int>>& caminhoRetorno, bool guardarCaminho);
#endif // IDASTAR_HPP