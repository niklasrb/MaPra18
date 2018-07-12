CXX = g++
FLAGS = -Wall -v -std=c++11  -g
LIB = -lsfml-graphics -lsfml-window -lsfml-system
#g++ -Wall -v -o "main" "main.cpp" -g -std=c++11  -lsfml-graphics -lsfml-window -lsfml-system obj/unit.o  (in directory: /media/data/Documents/University/18 SS/MaPra/A05b)

HEADER = include/unit.h include/alg.h include/graph.h include/util.h include.h include/graph_visualizer.h
OBJ = obj/unit.o 

main: main.cpp $(OBJ) $(HEADER) 
	$(CXX) -o $@ $< $(FLAGS) $(LIB) $(OBJ) 

clean:
	rm -f main
