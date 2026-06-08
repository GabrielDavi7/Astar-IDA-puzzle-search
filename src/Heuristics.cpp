#include "Heuristics.hpp"
#include <cmath>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <queue>      
#include <fstream>    
#include <iostream>   
#include <cstdint>    


std::vector<uint8_t> TabelaPDB_8;
std::vector<uint8_t> TabelaPDB_15_G1;
std::vector<uint8_t> TabelaPDB_15_G2;

const std::vector<int> alvos_8 = {1, 2, 3, 4, 5, 6, 7, 8};
const std::vector<int> alvos_15_g1 = {1, 2, 3, 4, 5, 6};
const std::vector<int> alvos_15_g2 = {7, 8, 9, 10, 11, 12};


int manhattan_distance(const std::vector<int>& tabuleiro) { 
    int distancia = 0;
    int lado = (tabuleiro.size() == 9) ? 3 : 4; 

    for (size_t i = 0; i < tabuleiro.size(); ++i) {
        int valor = tabuleiro[i];
        if (valor != 0) { 
            int posXAtual = i % lado;
            int posYAtual = i / lado;
            int posXObjetivo = valor % lado;
            int posYObjetivo = valor / lado;
            distancia += std::abs(posXAtual - posXObjetivo) + std::abs(posYAtual - posYObjetivo);
        }
    }
    return distancia;
}

int conflict_linear(const std::vector<int>& tabuleiro) {
    int pecas_removidas = 0;
    int lado = (tabuleiro.size() == 9) ? 3 : 4; 

    for (int linha = 0; linha < lado; ++linha) {
        std::vector<int> linha_atual;
        for (int col = 0; col < lado; ++col) {
            int val = tabuleiro[linha * lado + col];
            if (val != 0 && (val / lado) == linha) linha_atual.push_back(val);
        }
        bool resolvido = false;
        while (!resolvido) {
            int max_conflitos = 0;
            int idx_pior_peca = -1;
            std::vector<int> conflitos_por_peca(linha_atual.size(), 0);

            for (size_t i = 0; i < linha_atual.size(); ++i) {
                if (linha_atual[i] == -1) continue; 
                for (size_t j = i + 1; j < linha_atual.size(); ++j) {
                    if (linha_atual[j] == -1) continue;
                    if (linha_atual[i] > linha_atual[j]) {
                        conflitos_por_peca[i]++;
                        conflitos_por_peca[j]++;
                    }
                }
                if (conflitos_por_peca[i] > max_conflitos) {
                    max_conflitos = conflitos_por_peca[i];
                    idx_pior_peca = i;
                }
            }
            if (max_conflitos > 0) {
                linha_atual[idx_pior_peca] = -1; 
                pecas_removidas++;
            } else { resolvido = true; }
        }
    }

    for (int col = 0; col < lado; ++col) {
        std::vector<int> coluna_atual;
        for (int linha = 0; linha < lado; ++linha) {
            int val = tabuleiro[linha * lado + col];
            if (val != 0 && (val % lado) == col) coluna_atual.push_back(val);
        }
        bool resolvido = false;
        while (!resolvido) {
            int max_conflitos = 0;
            int idx_pior_peca = -1;
            std::vector<int> conflitos_por_peca(coluna_atual.size(), 0);
            for (size_t i = 0; i < coluna_atual.size(); ++i) {
                if (coluna_atual[i] == -1) continue; 
                for (size_t j = i + 1; j < coluna_atual.size(); ++j) {
                    if (coluna_atual[j] == -1) continue;
                    if (coluna_atual[i] > coluna_atual[j]) {
                        conflitos_por_peca[i]++;
                        conflitos_por_peca[j]++;
                    }
                }
                if (conflitos_por_peca[i] > max_conflitos) {
                    max_conflitos = conflitos_por_peca[i];
                    idx_pior_peca = i;
                }
            }
            if (max_conflitos > 0) {
                coluna_atual[idx_pior_peca] = -1; 
                pecas_removidas++;
            } else { resolvido = true; }
        }
    }
    return manhattan_distance(tabuleiro) + (2 * pecas_removidas);
}

int calcular_hash(const std::vector<int>& tabuleiro, const std::vector<int>& alvos) {
    int hash = 0;
    int multiplicador = 1;
    int n = tabuleiro.size(); 
    std::vector<bool> ocupado(n, false);

    for (size_t i = 0; i < alvos.size(); ++i) {
        int peca = alvos[i];
        int pos_no_tabuleiro = -1;
        for (int j = 0; j < n; ++j) {
            if (tabuleiro[j] == peca) {
                pos_no_tabuleiro = j;
                break;
            }
        }
        int rank = 0;
        for (int j = 0; j < pos_no_tabuleiro; ++j) {
            if (!ocupado[j]) rank++;
        }
        hash += rank * multiplicador;
        multiplicador *= (n - i);
        ocupado[pos_no_tabuleiro] = true;
    }
    return hash;
}


void gerar_pdb_generico(std::vector<uint8_t>& tabela, const std::vector<int>& alvos, const std::string& arquivo_nome, int tamanho_puzzle) {
    std::cout << ">>> Gerando " << arquivo_nome << "..." << std::endl;
    
    int lado = std::sqrt(tamanho_puzzle);
    long long total_estados = 1;
    for (size_t i = 0; i < alvos.size(); ++i) total_estados *= (tamanho_puzzle - i);
    tabela.assign(total_estados, 255);

    std::vector<int> estado_inicial(tamanho_puzzle, -1);
    for (int peca : alvos) estado_inicial[peca] = peca;

    std::queue<std::vector<int>> fila; 
    int hash_inicial = calcular_hash(estado_inicial, alvos);
    tabela[hash_inicial] = 0; 
    fila.push(estado_inicial);
    
    int dLinha[] = {-1, 1, 0, 0};
    int dColuna[] = {0, 0, -1, 1};
    
    while(!fila.empty()) {
        std::vector<int> atual = fila.front();
        fila.pop();
        int custo = tabela[calcular_hash(atual, alvos)];
        
        for (int pos = 0; pos < tamanho_puzzle; ++pos) {
            if (atual[pos] != -1) { 
                int linha = pos / lado;
                int coluna = pos % lado;
                
                for (int d = 0; d < 4; ++d) {
                    int novaLinha = linha + dLinha[d];
                    int novaColuna = coluna + dColuna[d];
                    
                    if (novaLinha >= 0 && novaLinha < lado && novaColuna >= 0 && novaColuna < lado) {
                        int novaPos = novaLinha * lado + novaColuna;
                        
                        if (atual[novaPos] == -1) {
                            std::vector<int> vizinho = atual;
                            std::swap(vizinho[pos], vizinho[novaPos]);
                            
                            int hash_vizinho = calcular_hash(vizinho, alvos);
                            if (tabela[hash_vizinho] == 255) { 
                                tabela[hash_vizinho] = custo + 1; 
                                fila.push(vizinho);
                            }
                        }
                    }
                }
            }
        }
    }
    std::ofstream arquivo(arquivo_nome, std::ios::binary);
    arquivo.write(reinterpret_cast<const char*>(tabela.data()), tabela.size());
    arquivo.close();
}

void carregar_pdb(std::vector<uint8_t>& tabela, const std::vector<int>& alvos, const std::string& arquivo_nome, int tamanho_puzzle) {
    std::ifstream arquivo(arquivo_nome, std::ios::binary);
    if(arquivo.is_open()) {
        arquivo.seekg(0, std::ios::end);
        size_t tamanho = arquivo.tellg();
        arquivo.seekg(0, std::ios::beg);
        tabela.resize(tamanho);
        arquivo.read(reinterpret_cast<char*>(tabela.data()), tamanho);
        arquivo.close();
    } else {
        gerar_pdb_generico(tabela, alvos, arquivo_nome, tamanho_puzzle);
    }
}


int pattern_database(const std::vector<int>& tabuleiro) {
    if(tabuleiro.size() == 9) { 
        if(TabelaPDB_8.empty()) carregar_pdb(TabelaPDB_8, alvos_8, "pdb_8puzzle.dat", 9); 
        return TabelaPDB_8[calcular_hash(tabuleiro, alvos_8)]; 
    } else {
        if(TabelaPDB_15_G1.empty()) carregar_pdb(TabelaPDB_15_G1, alvos_15_g1, "pdb_15puzzle_g1.dat", 16);
        if(TabelaPDB_15_G2.empty()) carregar_pdb(TabelaPDB_15_G2, alvos_15_g2, "pdb_15puzzle_g2.dat", 16);
        
        int h1 = TabelaPDB_15_G1[calcular_hash(tabuleiro, alvos_15_g1)];
        int h2 = TabelaPDB_15_G2[calcular_hash(tabuleiro, alvos_15_g2)];
        
        int h3 = 0;
        for (size_t i = 0; i < 16; ++i) {
            int val = tabuleiro[i];
            if (val == 13 || val == 14 || val == 15) {
                int posXAtual = i % 4;
                int posYAtual = i / 4;
                int posXObjetivo = val % 4;
                int posYObjetivo = val / 4;
                h3 += std::abs(posXAtual - posXObjetivo) + std::abs(posYAtual - posYObjetivo);
            }
        }
        
        int custo_pdb = h1 + h2 + h3;
        int custo_conflito = conflict_linear(tabuleiro);
        
        return std::max(custo_pdb, custo_conflito);
    }
}