SRC_DIR = OpenGL_hackathon
OBJ_DIR = obj
INC_DIR = include
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst %.cpp,%.o,$(patsubst $(SRC_DIR)%, $(OBJ_DIR)%, $(SRC)))
BIN = oglhack.exe

CC = g++

FLAGS = -Wall -std=gnu++11 -fmax-errors=5
INC = -I$(INC_DIR)

$(BIN): $(OBJ)
	@echo linking $@
	@$(CC) -lglfw -lglut -lGLEW -lGL -lSDL2 $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo compiling $(notdir $^)
	@$(CC) -c $^ $(FLAGS) $(INC) -o $@

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@echo cleaning..
	@rm -rf $(OBJ_DIR)
	@rm $(BIN)
debug:
	@echo SRC = $(SRC)
	@echo OBJ = $(OBJ)
	@echo INC = $(INC)
