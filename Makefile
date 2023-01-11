TARGET = Task
CC = g++

PREF_SRC = ./src/
PREF_OBJ = ./obj/

SRC = $(shell find . -name "*.cpp")
OBJ = $(patsubst $(PREF_SRC)%.cpp, $(PREF_OBJ)%.o, $(SRC))



FLAGS = -std=c++17 -I./src -pthread


all : $(TARGET) srun

compile : $(TARGET)

$(TARGET) : $(OBJ) 	
	$(CC) $(FLAGS) $(OBJ) -o $(TARGET) 

$(PREF_OBJ)%.o : $(PREF_SRC)%.cpp
	mkdir -p $(dir $@)
	$(CC) -c $(FLAGS) $< -o $@

run :
	./$(TARGET)

srun :
	sudo ./$(TARGET)

clean :
	rm $(TARGET)$(OBJ)
