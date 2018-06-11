
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "util.h"
#include "graph.h"
#include "alg.h"

 
void PruefeBsp14(int bsp, std::shared_ptr<DistanceGraph> dg)
{
	EmptyVisualizer tv;
	std::vector<CostT> cost;
	for(size_t i = 0; i < dg->numVertices(); i++)
	{
		Dijkstra(*dg, tv, i, cost);
		std::cout << "Startknoten " << i << ": " << cost << std::endl;
		PruefeDijkstra(bsp, i, cost);
		
		for(size_t j = 0; j < dg->numVertices(); j++) {
			std::cout << "Pruefe Weg " << i << " -> " << j << std::endl;
			std::list<VertexT> weg;
			if(A_star(*dg, tv, i, j, weg))
				PruefeWeg(bsp, weg);
		}
	
	}
	
}


int main(int argc, char** argv)
{
	int n = 4;
	std::stringstream ss;
	std::fstream f;
	
	std::vector<std::shared_ptr<DistanceGraph> > bsp;
	
	for(int i = 1; i <= n; i++) {
		ss.str("");
		ss << "daten/Graph" << i << ".dat";
		std::cout << "ss = " << ss.str() << std::endl;
		f.open(ss.str(), std::fstream::in);
		CoordinateGraph cg;
		f >> cg;
		cg.Print();
		bsp.push_back(std::make_shared<CoordinateGraph>(cg));
		f.close();
	}
	
	for(int i = 1; i <= n; i++) {
		std::cout << "Beispiel " << i << ": " << (PruefeHeuristik(*bsp[i-1]) ? "Heuristik ok" : "Heuristik fehlerhaft") << std::endl;
		PruefeBsp14(i, bsp[i-1]);
	}
	
	
	return 0;
}

