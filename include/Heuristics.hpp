#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include <vector>

int manhattan_distance(const std::vector<int>& tabuleiro);
int conflict_linear(const std::vector<int>& tabuleiro);
int pattern_database(const std::vector<int>& tabuleiro);
#endif // HEURISTICS_HPP