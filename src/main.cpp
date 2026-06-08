#include "AStar.hpp"
#include "IDAStar.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <conio.h> 
#include <cstdlib>   
#include <chrono> 
#include <cstdint> 

using namespace std;

// --- DECLARAÇÃO DAS TABELAS PARA CÁLCULO DE RAM ---
extern std::vector<uint8_t> TabelaPDB_8;
extern std::vector<uint8_t> TabelaPDB_15_G1;
extern std::vector<uint8_t> TabelaPDB_15_G2;

void mostrarUsoMemoria() {
    double totalBytes = TabelaPDB_8.size() + TabelaPDB_15_G1.size() + TabelaPDB_15_G2.size();
    double totalMB = totalBytes / (1024.0 * 1024.0);
    
    cout << "\n===================================================" << endl;
    cout << "               CONSUMO DE MEMORIA (RAM)            " << endl;
    cout << "===================================================" << endl;
    cout << "Tabela PDB 8-Puzzle:        " << TabelaPDB_8.size() / 1024.0 << " KB" << endl;
    cout << "Tabela PDB 15-Puzzle (G1):  " << TabelaPDB_15_G1.size() / 1024.0 << " KB" << endl;
    cout << "Tabela PDB 15-Puzzle (G2):  " << TabelaPDB_15_G2.size() / 1024.0 << " KB" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "TOTAL PDBs na RAM:          " << totalMB << " MB" << endl;
    cout << "===================================================\n" << endl;
}

void imprimirTabuleiro(const vector<int>& tabuleiro) { 
    int tamanho = tabuleiro.size();
    int lado = sqrt(tamanho);

    for(int i = 0; i < tamanho; ++i) { 
        if(tabuleiro[i] == 0) cout << " - ";
        else {
            if (tabuleiro[i] < 10) cout << "  "; 
            else cout << " ";
            cout << tabuleiro[i];
        }
        if ((i + 1) % lado == 0) cout << endl;
    }
    cout << endl;
}

bool tem_solucao(const vector<int>& tabuleiro) {
    int tamanho = tabuleiro.size();
    int lado = sqrt(tamanho);
    int inversoes = 0;
    int linhaVazio = 0;

    for (int i = 0; i < tamanho - 1; ++i) {
        if (tabuleiro[i] == 0) {
            linhaVazio = lado - (i / lado); 
            continue;
        }
        for (int j = i + 1; j < tamanho; ++j) {
            if (tabuleiro[j] != 0 && tabuleiro[i] > tabuleiro[j]) inversoes++;
        }
    }
    if (tabuleiro[tamanho - 1] == 0) linhaVazio = 1;

    if (lado % 2 != 0) return (inversoes % 2 == 0);
    else return (inversoes % 2) == (linhaVazio % 2);
}

vector<vector<int>> lerInstancias(const string& nomeArquivo, int tamanhoInstancia) { 
    ifstream arquivo(nomeArquivo);
    vector<vector<int>> instancias; 
    vector<int> buffer;
    int numero;

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return instancias;
    }
    while (arquivo >> numero) {
        buffer.push_back(numero);
        if (buffer.size() == (size_t)tamanhoInstancia) {
            instancias.push_back(buffer);
            buffer.clear();
        }
    }
    arquivo.close();
    return instancias;
}

int main() { 
    char optAlgoritmo, optHeuristica, optTamanho, optQuantidade;

    cout << "===================================================" << endl;
    cout << "         MENU DE INTERACAO RAPIDA - PUZZLE         " << endl;
    cout << "===================================================" << endl;

    cout << "\n[1/4] Escolha o Algoritmo:\n  (1) A*\n  (2) IDA*\n>> Opcao: ";
    do { optAlgoritmo = _getch(); } while (optAlgoritmo != '1' && optAlgoritmo != '2');
    cout << (optAlgoritmo == '1' ? "A*" : "IDA*") << endl;

    cout << "\n[2/4] Escolha a Heuristica:\n  (1) Manhattan + Conflito Linear\n  (2) Pattern Database (PDB)\n>> Opcao: ";
    do { optHeuristica = _getch(); } while (optHeuristica != '1' && optHeuristica != '2');
    cout << (optHeuristica == '1' ? "Manhattan + Conflito" : "Pattern Database (PDB)") << endl;

    cout << "\n[3/4] Escolha o Tamanho:\n  (1) 8-Puzzle (3x3)\n  (2) 15-Puzzle (4x4)\n>> Opcao: ";
    do { optTamanho = _getch(); } while (optTamanho != '1' && optTamanho != '2');
    cout << (optTamanho == '1' ? "8-Puzzle" : "15-Puzzle") << endl;

    cout << "\n[4/4] Executar quais instancias?\n  (1) Apenas PRIMEIRA (Passo a Passo)\n  (2) TODAS as instancias\n  (3) ESCOLHER uma instancia especifica (Passo a Passo)\n>> Opcao: ";
    do { optQuantidade = _getch(); } while (optQuantidade != '1' && optQuantidade != '2' && optQuantidade != '3');
    
    if (optQuantidade == '1') cout << "Primeira instancia" << endl;
    else if (optQuantidade == '2') cout << "Todas as instancias" << endl;
    else cout << "Escolher instancia especifica" << endl;

    int tipoPuzzle = (optTamanho == '1') ? 8 : 15;
    int tamanhoInstancia = (tipoPuzzle == 8) ? 9 : 16; 
    string nomeArquivo = (tipoPuzzle == 8) ? "input/8puzzle_instances.txt" : "input/15puzzle_instances.txt";

    vector<vector<int>> instancias = lerInstancias(nomeArquivo, tamanhoInstancia);
    if (instancias.empty()) return 1;

    int nInstanciaTotal = (int)instancias.size();
    int indexInicio = 0;
    int indexFim = nInstanciaTotal;

    // Lógica para definir os limites do For Loop
    if (optQuantidade == '1') {
        indexFim = 1;
    } else if (optQuantidade == '3') {
        int escolha;
        cout << "\n>> O arquivo possui " << nInstanciaTotal << " instancias." << endl;
        cout << ">> Digite o numero da instancia que deseja resolver: ";
        cin >> escolha;
        
        // Validação básica
        if (escolha < 1 || escolha > nInstanciaTotal) {
            cout << "Numero invalido! Executando a instancia 1 por padrao." << endl;
            escolha = 1;
        }
        indexInicio = escolha - 1;
        indexFim = escolha;
    }

    system("cls"); // Limpa a tela antes de começar a execução

    cout << "===================================================" << endl;
    cout << " ALGORITMO:  " << (optAlgoritmo == '1' ? "A*" : "IDA*") << endl;
    cout << " HEURISTICA: " << (optHeuristica == '1' ? "Manhattan + Conflito Linear" : "Pattern Database (PDB)") << endl;
    cout << "===================================================" << endl;

    int tipoHeuristica = optHeuristica - '0';
    
    // VARIÁVEL DE CONTROLE: Grava o caminho se for a Opção 1 ou a Opção 3
    bool guardarCaminho = (optQuantidade == '1' || optQuantidade == '3');

    double totalTempoMs = 0.0;
    long long totalMovimentos = 0;   
    long long totalNosExpandidos = 0; 
    int instanciasResolvidas = 0;

    // --- LOOP AGORA USA indexInicio e indexFim ---
    for (int i = indexInicio; i < indexFim; ++i) {
        if (!tem_solucao(instancias[i])) {
            cout << "Instancia " << i + 1 << ": Insoluvel." << endl;
            continue;
        }

        cout << "Resolvendo Instancia " << i + 1 << "..." << endl;

        int movimentos = -1;
        int nosExpandidosAtual = 0;
        std::vector<std::vector<int>> caminhoSolucao; 

        auto start = chrono::high_resolution_clock::now();

        if (optAlgoritmo == '1') movimentos = execAstar(instancias[i], tipoHeuristica, nosExpandidosAtual, caminhoSolucao, guardarCaminho);
        else movimentos = execIDAStar(instancias[i], tipoHeuristica, nosExpandidosAtual, caminhoSolucao, guardarCaminho);

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> duracao = end - start;
        double tempoMs = duracao.count();

        if (movimentos != -1) {
            totalTempoMs += tempoMs;
            totalMovimentos += movimentos;
            totalNosExpandidos += nosExpandidosAtual;
            instanciasResolvidas++;

            cout << "  Custo: " << movimentos << " movimentos | "
                 << "Nos: " << nosExpandidosAtual << " | "
                 << "Tempo: " << tempoMs << " ms" << endl;
                 
            // IMPRIME PASSO A PASSO
            if (guardarCaminho) {
                cout << "\n============= SOLUCAO PASSO A PASSO ===============" << endl;
                for (size_t step = 0; step < caminhoSolucao.size(); ++step) {
                    cout << ">>> MOVIMENTO " << step << " <<<" << endl;
                    imprimirTabuleiro(caminhoSolucao[step]);
                }
                cout << "===================================================" << endl;
            } else {
                cout << "---------------------------------------------------" << endl;
            }
        }
    }

    if (optHeuristica == '2') mostrarUsoMemoria();

    if (instanciasResolvidas > 0 && optQuantidade == '2') {
        cout << "\n>>> " << instanciasResolvidas << " instancias resolvidas com sucesso!\n" << endl;
        cout << "===================================================" << endl;
        cout << "               RESUMO DE DESEMPENHO                " << endl;
        cout << "===================================================" << endl;
        cout << "Media de Movimentos:   " << (double)totalMovimentos / instanciasResolvidas << endl;
        cout << "Media de Nos Exp.:     " << (double)totalNosExpandidos / instanciasResolvidas << endl;
        cout << "Tempo Medio/Instancia: " << totalTempoMs / instanciasResolvidas << " ms" << endl;
        cout << "---------------------------------------------------" << endl;
        cout << "Tempo Total Gasto:     " << totalTempoMs / 1000.0 << " segundos" << endl;
        cout << "===================================================" << endl;
    }

    return 0;
}