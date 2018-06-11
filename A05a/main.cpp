
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "util.h"
#include "graph.h"
#include "alg.h"

 

int main(int argc, char** argv)
{
	int n = 1;
	std::stringstream ss;
	ss << "daten/Graph" << n << ".dat";
	
	std::fstream f;
	f.open(ss.str(), std::fstream::in);
	
	CoordinateGraph cg;
	f >> cg;
	f.close();
	
	cg.Print();
	
	
	return 0;
}

