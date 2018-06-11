CXX = g++
FLAGS = -Wall -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0 -g

HEADER = unit.h alg.h text_visualizer.h graph.h util.h
SRC = main.cpp
OBJ = unit.o graph.o

main: $(OBJ) $(HEADER) $(SRC)
	$(CXX) $(FLAGS) $(HEADER) $(OBJ) $(SRC) -o main

graph.o: graph.h graph.cpp util.h
	$(CXX) $(FLAGS) graph.h graph.cpp util.h -c


clean:
	rm -f main graph.o 
