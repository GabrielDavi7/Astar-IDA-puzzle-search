# comandos
#	 make
#	 .\puzzle_solver.exe


# Nome do executável final
TARGET = puzzle_solver
# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O3 -Iinclude

# Diretórios
SRC_DIR = src
OBJ_DIR = obj

# Encontra todos os .cpp em src/ e define os correspondentes .o em obj/
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Regra principal
all: $(TARGET)

# Como linkar o executável
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Como compilar cada .cpp em um .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cria a pasta obj se não existir
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Limpa os binários gerados
clean:
	rm -rf $(OBJ_DIR) $(TARGET)