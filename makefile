# Makefile

CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src
OBJ_DIR = obj
BIN = shape-shooter

# Source and object files
SRC = $(SRC_DIR)/Game.cpp # Just compile the Game.cpp file, as it includes everything else (very messy structure. sorry i was a beginner)
OBJ = $(OBJ_DIR)/Game.o

# Rule to build the final executable
$(BIN): $(OBJ)
	$(CXX) $(OBJ) -o $(BIN) $(LDFLAGS)


$(OBJ_DIR)/Game.o: $(SRC)
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ)


clean:
	rm -rf $(OBJ_DIR) $(BIN)


run: $(BIN)
	./$(BIN)

