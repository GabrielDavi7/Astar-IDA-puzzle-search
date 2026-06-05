#ifndef IDASTAR_HPP
#define IDASTAR_HPP

#include "Puzzle.hpp"
#include <vector>

int execIDAStar(const std::vector<int>& estadoInicial);
int busca(estado& atual, int limite);

#endif // IDASTAR_HPP