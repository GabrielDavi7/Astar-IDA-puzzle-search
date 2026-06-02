#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

// Este código é parte de um projeto de pesquisa sobre algoritmos A* e IDA* para resolver puzzles deslizantes (8-puzzle e 15-puzzle).
    // Teste de leitura
        //.\puzzle_solver.exe input\8puzzle_instances.txt 8 ; .\puzzle_solver.exe input\15puzzle_instances.txt 15

using namespace std;

void imprimirTabuleiro(const vector<int>& tabuleiro) { 
    int tamanho = tabuleiro.size();
    int lado = sqrt(tamanho);

    // CORREÇÃO: O laço deve ir até 'tamanho' (ex: 9), não até 'lado' (ex: 3)
    for(int i = 0; i < tamanho; ++i) { 
        if(tabuleiro[i] == 0) {
            cout << " - ";
        } else {
            if (tabuleiro[i] < 10) {
                cout << " "; 
            } 
            cout << tabuleiro[i] << " ";
        }
        // Quebra a linha quando atinge a borda direita
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

int main(int argc, char* argv[]) { 
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <nome_arquivo.txt> <tipo: 8 ou 15>" << endl;
        return 1;
    }

    string nomeArquivo = argv[1];
    int tipoPuzzle = stoi(argv[2]);
    int tamanhoInstancia = (tipoPuzzle == 8) ? 9 : 16; 

    vector<vector<int>> instancias = lerInstancias(nomeArquivo, tamanhoInstancia);

    if (instancias.empty()) {
        cerr << "Nenhuma instancia valida encontrada no arquivo!" << endl;
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
        cout << "Sucesso! Foram carregadas " << instancias.size() << " instancias." << endl;
        cout << "Todas as instancias estao 100% integras e validas!" << endl;
    } else {
        cout << "Foram encontradas " << instanciasInvalidas << " instancias corrompidas." << endl;
        return 1; 
    }
    
    cout << "---------------------------------------------------" << endl;
    
    cout << "Primeira instancia (Indice 0):" << endl;
    imprimirTabuleiro(instancias[0]);
    
    cout << "Ultima instancia (Indice " << instancias.size() - 1 << "):" << endl;
    imprimirTabuleiro(instancias.back());
    
    return 0;
}