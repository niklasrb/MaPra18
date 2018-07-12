CXX = g++
FLAGS = -Wall -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 -g
LIB = -lsfml-graphics -lsfml-window -lsfml-system

HEADER = include/unit.h include/alg.h include/text_visualizer.h include/graph.h include/util.h include.h
OBJ = obj/unit.o 

main: main.cpp $(OBJ) $(HEADER) 
	$(CXX) $(FLAGS) $(OBJ) $(LIB) -o $@ $<




clean:
	rm -f main obj/graph.o 
