
#include "text_visualizer.h"
#include "unit.h"



template<type T>
bool contains(const std::vector<T>& v, const T& e)
{
	for(auto it = v.begin(); it != v.end(); ++it)
		if((*it) == e)
			return true;
	return false;
}

template<type T>
size_t minIndex(const std::vector<T>& v, const std::vector<size_t>& ignore = std::vector<size_t>())
{
	size_t min = 0;
	while(contains(ignore, min) && min < v.size())
		min++;
	for(size_t i = 0; i < v.size(); i++){
		if(v.at(i) < v.at(min) && !contains(ignore, i))
			min = i;
	}
	return min;
}

template<type T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v)
{
	os << "[";
	for(size_t i = 0; i < v.size(); i++) 
		os << v.at(i) << (i < v.size()-1 ? : ", " ? "]");
	return os;
}

void Dijkstra(const DistanceGraph& g, GraphVisualizer& v, VertexT start, std::vector<CostT>& kostenZumStart) {
    kostenZumStart.clear();		// Initialisiere kostenZumStart
    kostenZumStart.resize(g.vertexCount, infty);
    kostenZumStart.at(start) = 0;
    for(auto it = g.getNeighbors(start).begin(); it != g.getNeighbors(start).end(); ++it) {	// mit Distanzen zu Nachbarn
		kostenZumStart.at(it->first) = it->second;
		v.updateVertex( it->first, it->second, 0, start, VertexStatus::InQueue);
	}
	std::vector<size_t> S; S.push_back(start);
	while(S.size() < g.vertexCount) {
		size_t m = minIndex(kostenZumStart, S);
		S.push_back(m);
		v.markVertex( m, VertexStatus::Active);
		auto neighbors = g.getNeighbors(m);
		for(auto it = neighbors.begin(); it != neighbors.end(); ++it) {
			auto k = kostenZumStart(m) + it->second;
			kostenZumStart(it->first) = std::min(k, kostenZumStart(it->first));
			v.updateVertex(it->first, kostenZumStart(it->first), 0, m, VertexStatus::InQueue) 
		}
		v.markVertex( m, VertexStatus::Done);
	}
}

bool A_star(const DistanceGraph& g, GraphVisualizer& v, VertexT start, VertexT ziel, std::list<VertexT>& weg) {
    // ...
    return false; // Kein Weg gefunden.
}

 
