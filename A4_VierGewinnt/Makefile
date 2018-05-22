CXX = g++
CXXFLAGS = -O2 -g  -Wall --std=c++11
LIBS =  -pthread -lX11 -fopenmp

UNIT = unit.o plotx11.o
HEADER = 4gfeld.h 4gspieler.h 4gspiel.h 4gki.h unit.h plotx11.h
SRC = main.cpp

main:  $(HEADER) $(UNIT) $(SRC)
	$(CXX) $(CXXFLAGS) $(UNIT) $(HEADER) $(SRC) $(LIBS)  -o $@ 

clean:
	rm -f main 

.PHONY: clean
