#include "AStar.hpp"
#include "IDAStar.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <conio.h> 
#include <cstdlib>   

using namespace std;

void imprimirTabuleiro(const vector<int>& tabuleiro) { 
    int tamanho = tabuleiro.size();
    int lado = sqrt(tamanho);

    for(int i = 0; i < tamanho; ++i) { 
        if(tabuleiro[i] == 0) {
            cout << " - ";
        } else {
            if (tabuleiro[i] < 10) {
                cout << " "; 
            } 
            cout << tabuleiro[i] << " ";
        }
        if ((i + 1) % lado == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

bool validar(const vector<int>& tabuleiro, int tamanhoPuzzle) { 
    vector<bool> numeros(tamanhoPuzzle, false); 

    for(int num : tabuleiro) { 
        if(num < 0 || num >= tamanhoPuzzle) { 
            return false;
        }
        if(numeros[num]) { 
            return false; 
        }
        numeros[num] = true; 
    }
    return true; 
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
    char optAlgoritmo;
    char optTamanho;
    char optQuantidade;

    cout << "===================================================" << endl;
    cout << "         MENU DE INTERACAO RAPIDA - PUZZLE         " << endl;
    cout << "===================================================" << endl;

    cout << "\n[1/3] Escolha o Algoritmo:" << endl;
    cout << "  (1) A*" << endl;
    cout << "  (2) IDA*" << endl;
    cout << ">> Opcao: ";
    do {
        optAlgoritmo = _getch(); 
    } while (optAlgoritmo != '1' && optAlgoritmo != '2');
    cout << (optAlgoritmo == '1' ? "A*" : "IDA*") << endl;

    cout << "\n[2/3] Escolha o Tamanho do Quebra-Cabeca:" << endl;
    cout << "  (1) 8-Puzzle (3x3)" << endl;
    cout << "  (2) 15-Puzzle (4x4)" << endl;
    cout << ">> Opcao: ";
    do {
        optTamanho = _getch();
    } while (optTamanho != '1' && optTamanho != '2');
    cout << (optTamanho == '1' ? "8-Puzzle" : "15-Puzzle") << endl;

    cout << "\n[3/3] Executar quais instancias?" << endl;
    cout << "  (1) Apenas a PRIMEIRA instancia" << endl;
    cout << "  (2) TODAS as instancias do arquivo" << endl;
    cout << ">> Opcao: ";
    do {
        optQuantidade = _getch();
    } while (optQuantidade != '1' && optQuantidade != '2');
    cout << (optQuantidade == '1' ? "Apenas a primeira" : "Todas as instancias") << endl;


    system("cls"); 

    cout << "===================================================" << endl;
    cout << "               INICIANDO EXECUCAO                  " << endl;
    cout << "===================================================" << endl;

    int tipoPuzzle = (optTamanho == '1') ? 8 : 15;
    int tamanhoInstancia = (tipoPuzzle == 8) ? 9 : 16; 
    string nomeArquivo = (tipoPuzzle == 8) ? "input/8puzzle_instances.txt" : "input/15puzzle_instances.txt";

    cout << "Carregando dados de '" << nomeArquivo << "'..." << endl;
    vector<vector<int>> instancias = lerInstancias(nomeArquivo, tamanhoInstancia);

    if (instancias.empty()) {
        cerr << "Erro: Nenhuma instancia valida encontrada no arquivo!" << endl;
        return 1;
    }

    int instanciasInvalidas = 0;
    for (size_t i = 0; i < instancias.size(); ++i) {
        if (!validar(instancias[i], tamanhoInstancia)) {
            cout << "Erro: O Tabuleiro " << i + 1 << " e invalido!" << endl;
            instanciasInvalidas++;
        }
    }

    if (instanciasInvalidas == 0) {
        cout << "Sucesso! Foram carregadas " << instancias.size() << " instancias integras." << endl;
    } else {
        cout << "Foram encontradas " << instanciasInvalidas << " instancias corrompidas." << endl;
        return 1; 
    }
    
    cout << "---------------------------------------------------" << endl;
    cout << "Primeira instancia (Indice 0):" << endl;
    imprimirTabuleiro(instancias[0]);
    
    cout << "Ultima instancia (Indice " << instancias.size() - 1 << "):" << endl;
    imprimirTabuleiro(instancias.back());
    
    cout << "---------------------------------------------------" << endl;
    if (optAlgoritmo == '1') {
        cout << "               Executando Busca: A* " << endl;
    } else {
        cout << "              Executando Busca: IDA* " << endl;
    }
    cout << "---------------------------------------------------" << endl;

    int nInstancia = (optQuantidade == '1') ? 1 : (int)instancias.size();

    for (int i = 0; i < nInstancia; ++i) {
        cout << "Instancia " << i + 1 << " de " << nInstancia << "..." << endl;
        
        int movimentos = -1;
        if (optAlgoritmo == '1') {
            movimentos = execAstar(instancias[i]);
        } else {
            movimentos = execIDAStar(instancias[i]);
        }
        
        if (movimentos != -1) {
            cout << "Custo do caminho: " << movimentos << " movimentos." << endl;
        } else {
            cout << "Erro: Sem solucao possivel." << endl;
        }
        cout << "---" << endl;
    }

    return 0;
}