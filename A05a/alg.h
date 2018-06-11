
#include "text_visualizer.h"
#include "unit.h"





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

bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    // ...
    return false; // Kein Weg gefunden.
}

 
