CXXFLAGS = -O2 -Wall --std=c++11

main: vektor.o matrix.o main.o unit.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test: vektor.o matrix.o test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

vektor.o: vektor.cpp vektor.h
	$(CXX) $(CXXFLAGS) -c $<

matrix.o: matrix.cpp matrix.h vektor.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	rm -f test vektor.o matrix.o
