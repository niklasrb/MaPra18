CXX = g++
CXXFLAGS = -O2 -g  -Wall --std=c++11

sort: sort.o unit.o student.o
	$(CXX) $(CXXFLAGS) -o $@ $^

sort.o: sort.cpp student.h
	$(CXX) $(CXXFLAGS) -c $<

student.o: student.cpp student.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f student.o sort.o sort

.PHONY: clean
