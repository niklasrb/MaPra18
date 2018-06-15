
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <ctime>
#include "util.h"
#include "graph.h"
#include "alg.h"

 
void PruefeBsp14(GraphVisualizer& gv);
void PruefeMaze15(GraphVisualizer& gv);


int main(int argc, char** argv)
{
	//TextVisualizer tv;
	EmptyVisualizer tv;
	
	std::cout << "------ Beispiele 1 - 4 --------" << std::endl;
	PruefeBsp14(tv);
	//return 0;
	std::cout << "------ Beispiele 5 - 9 --------" << std::endl;
	PruefeMaze15(tv);
	
	Labyrinth rand;
	auto lab = ErzeugeLabyrinth( 256, 256, time(NULL));
	rand.Initialize(lab, 256, 256);
	VertexT s = undefinedVertex, z = undefinedVertex, i = 0;
	while(s == undefinedVertex || z == undefinedVertex) {
		if(lab.at(i) == CellType::Start)	s = i;
		if(lab.at(i) == CellType::Destination)	z = i;
		i++;
	}
	std::cout  << "------ Beispiel 10 --------" << std::endl
			<< "Finde Weg " << s << " -> " << z << std::endl;
	std::list<VertexT> weg;
	if(A_star(rand, tv, s, z, weg))
		PruefeWeg(10, weg);
	else
		std::cout << "keinen Weg gefunden" << std::endl;
	
	
	return 0;
}


void PruefeBsp14(GraphVisualizer& gv)
{
	int n = 4;
	std::stringstream ss;
	std::fstream f;
	
	std::vector<std::shared_ptr<DistanceGraph> > bsp_ptr;
	
	for(int i = 1; i <= n; i++) {
		ss.str("");
		ss << "daten/Graph" << i << ".dat";
		std::cout << "ss = " << ss.str() << std::endl;
		f.open(ss.str(), std::fstream::in);
		EuclideanDistanceGraph cg((i == 2 ? 1./2. : 1.));
		f >> cg;
		//cg.Print();
		bsp_ptr.push_back(std::make_shared<EuclideanDistanceGraph>(cg));
		f.close();
	}
	
	for(int bsp = 1; bsp <= n; bsp++) {
		auto dg = bsp_ptr[bsp-1];
		std::cout << "Beispiel " << bsp << ": " << (PruefeHeuristik(*dg) ? "Heuristik ok" : "Heuristik fehlerhaft") << std::endl;
		
		for(size_t i = 0; i < dg->numVertices(); i++) {
			std::vector<CostT> cost;
			Dijkstra(*dg, gv, i, cost);
			std::cout << "Startknoten " << i << ": " << cost << std::endl;
			PruefeDijkstra(bsp, i, cost);
			
			for(size_t j = 0; j < dg->numVertices(); j++) {
				std::cout << "Pruefe Weg " << i << " -> " << j << std::endl;
				std::list<VertexT> weg;
				if(A_star(*dg, gv, i, j, weg))
					PruefeWeg(bsp, weg);
			}
		}
	}	
}


void PruefeMaze15(GraphVisualizer& gv)
{
	int n = 5; int offset = 4;
	std::stringstream ss;
	std::fstream f;
	std::vector<std::shared_ptr<DistanceGraph> > lab_ptr;
	
	for(int i = 1; i <= n; i++) {
		ss.str("");
		ss << "daten/Maze" << i << ".dat";
		std::cout << "ss = " << ss.str() << std::endl;
		f.open(ss.str(), std::fstream::in);
		Labyrinth lg;
		f >> lg;
		//lg.Print();
		lab_ptr.push_back(std::make_shared<Labyrinth>(lg));
		f.close();
	}
	
	for(int bsp = 1; bsp <= n; bsp++) {
		auto lg = lab_ptr[bsp-1];
		std::cout << "Maze " << bsp << ": " << (PruefeHeuristik(*lg) ? "Heuristik ok" : "Heuristik fehlerhaft") << std::endl;
		
		auto szp = StartZielPaare(bsp + offset);
		
		for(auto it = szp.begin(); it != szp.end(); ++it) {
			std::cout << "Finde Weg " << it->first << " -> " << it->second << std::endl;
			std::list<VertexT> weg;
			if(A_star(*lg, gv, it->first, it->second, weg))
				PruefeWeg(bsp + offset, weg);
			else
				std::cout << "keinen Weg gefunden" << std::endl;
		}
	}	
}
