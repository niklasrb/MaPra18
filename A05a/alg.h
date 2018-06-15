
#include "text_visualizer.h"
#include "unit.h"
#include <algorithm>


void Dijkstra(const DistanceGraph& g, GraphVisualizer& v, VertexT start, std::vector<CostT>& kostenZumStart) {
    kostenZumStart.clear();		// Initialisiere kostenZumStart
    kostenZumStart.resize(g.numVertices(), infty);
    kostenZumStart.at(start) = 0;
    for(auto it = g.getNeighbors(start).begin(); it != g.getNeighbors(start).end(); ++it) {	// mit Distanzen zu Nachbarn
		kostenZumStart.at(it->first) = it->second;
		v.updateVertex( it->first, it->second, 0, start, VertexStatus::InQueue);
	}
	std::vector<size_t> S; S.push_back(start);
	while(S.size() < g.numVertices()) {
		size_t m = minIndex(kostenZumStart, S);
		S.push_back(m);
		v.markVertex( m, VertexStatus::Active);
		auto neighbors = g.getNeighbors(m);
		for(auto it = neighbors.begin(); it != neighbors.end(); ++it) {
			auto k = kostenZumStart.at(m) + it->second;
			kostenZumStart.at(it->first) = std::min(k, kostenZumStart.at(it->first));
			v.updateVertex(it->first, kostenZumStart.at(it->first), 0, m, VertexStatus::InQueue);
		}
		v.markVertex( m, VertexStatus::Done);
	}
}


// VertexStatus { UnknownVertex, InQueue, Active, Done, Destination };
void reconstruct(VertexT start, VertexT ziel, const std::vector<VertexT>& origin, std::list<VertexT>& weg)
{
	weg.clear(); weg.push_back(ziel);
	if(start == ziel)
		return;
	VertexT cur = ziel;
	do {
		cur = origin.at(cur);
		weg.push_front(cur);
	} while(cur != start);
}


bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    std::vector<CostT> fKosten, gKosten; 
    fKosten.resize(g.numVertices(), infty); gKosten.resize(g.numVertices(), infty);
    fKosten.at(start) = g.estimatedCost(start, ziel); gKosten.at(start) = 0;
    
    std::vector<VertexT> origin; origin.resize(g.numVertices(), undefinedVertex);
    
    std::vector<VertexT> closed; 
    std::vector<VertexT> open; open.push_back(start);
    
    auto cmp = [&fKosten] (VertexT a, VertexT b)
					{ return fKosten.at(a) > fKosten.at(b); };
    
    VertexT cur;
    
    while(open.size() > 0) {
		//std::cout << open << std::endl;
		std::sort(open.begin(), open.end(), cmp);
		//std::cout << "nach sortieren: " << open << std::endl;
		cur = open.at(open.size()-1); open.pop_back();
		v.markVertex(cur, VertexStatus::Active);
		//std::cout << "cur = " << cur << std::endl;
		if(cur == ziel) { // Ziel gefunden
			v.markVertex(cur, VertexStatus::Destination);
			reconstruct(start, ziel, origin, weg);
			return true;
		}
		// Betrachte alle Nachbarn
		for(auto it = g.getNeighbors(cur).begin(); it != g.getNeighbors(cur).end(); ++it) {
			if( contains(closed, it->first))	// already done
				continue;
			
			if(!contains(open, it->first)) // neuen Nachbar hinzufuegen
				open.push_back(it->first);		
			
			// Schaetze Kosten
			CostT moegliche_gKosten = gKosten.at(cur) + it->second;
			if(moegliche_gKosten >= gKosten.at(it->first))
				continue;		// this path sucks
			origin.at(it->first) = cur;
			gKosten.at(it->first) = moegliche_gKosten;
			fKosten.at(it->first) = moegliche_gKosten + g.estimatedCost(it->first, ziel);
			v.updateVertex(it->first, moegliche_gKosten, g.estimatedCost(it->first, ziel), cur, VertexStatus::InQueue);

		}
		closed.push_back(cur);
		v.markVertex(cur, VertexStatus::Done);
	}
    
    
    return false; // Kein Weg gefunden.
}

 
