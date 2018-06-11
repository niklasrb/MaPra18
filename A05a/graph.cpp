

#include "graph.h"
#include <cmath>
#include <cassert>



const DistanceGraph::NeighborT& CoordinateGraph::getNeighbors( VertexT v)  const
{
	assert(v < this->numVertices());
	return neighbors.at(v);
}
    
CostT CoordinateGraph::estimatedCost( VertexT from, VertexT to) const // Use Euclidean distance for estimation
{
	auto a = this->coord.at(from), b = this->coord.at(to);
	return std::sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}
    
CostT CoordinateGraph::cost( VertexT from, VertexT to) const
{
	assert(false);
	return 0;
}

void CoordinateGraph::Print() const
{
	std::cout << "Graph: size = " << this->numVertices() << std::endl;
	for(size_t i = 0; i < this->numVertices(); i++) {
		std::cout << i << ": " << this->coord.at(i) << std::endl;
		for(auto it = this->neighbors.at(i).begin(); it != neighbors.at(i).end(); ++it) {
			std::cout << i << " -> " << it->first << "  : " << it->second << std::endl;
		}
	}	
}
    
std::istream& operator >>(std::istream& is, CoordinateGraph& cg)
{
	assert(is.good());
	size_t nvert, nedge;
	is >> nvert; is >> nedge;
	cg.vertexCount = nvert;
	cg.neighbors.clear(); cg.neighbors.resize(nvert);
	for(size_t i = 0; i < nedge; i++) {
		size_t v1, v2; double d;
		assert(is.good());
		is >> v1; is >> v2; is >> d;
		assert(v1 < nvert && v2 < nvert); assert(d >= 0.);
		cg.neighbors.at(v1).push_back(std::make_pair(v2, d));
	}
	cg.coord.clear(); cg.coord.resize(nvert);
	for(size_t i = 0; i < nvert; i++) {
		double a, b;
		assert(is.good());
		is >> a; is >> b;
		cg.coord.at(i) = std::make_pair(a, b);
	}
	return is;
}

CostT CircularGraph::estimatedCost( VertexT from, VertexT to) const
{
	auto a = this->coord.at(from), b = this->coord.at(to);
	return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

CostT SphericalGraph::estimatedCost( VertexT from, VertexT to) const
{
	
}
 
CostT Labyrinth::estimatedCost( VertexT from, VertexT to) const	// use manhattan metric for estimation
{
	auto a = this->coord.at(from), b = this->coord.at(to);
	return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

void Labyrinth::Initialize(const std::vector<CellType>& cells, size_t h, size_t b)	// takes cellType array and turns it into a graph
{
	assert(cells.size() == h*b);
	this->vertexCount = h*b;
	this->coord.clear(); this->coord.resize(h*b);
	this->neighbors.clear(); this->neighbors.resize(h*b);
	this->cells = cells;
	size_t k =0; CellType c;
	for(size_t i =0; i < h; i++) {
		for(size_t j = 0; j < b; j++) {
			c = cells.at(k);
			if(c == CellType::Ground || c == CellType::Start || c == CellType::Destination) {
				if(i > 0) 
					this->neighbors.at(k).push_back(std::make_pair(  (i-1)*b + j, 1.));
				if(i < h-1 )
					this->neighbors.at(k).push_back(std::make_pair(  (i+1)*b + j, 1.));
				if( j < b-1)
					this->neighbors.at(k).push_back(std::make_pair(  i*b + j+1, 1.));
				if(j > 0)
					this->neighbors.at(k).push_back(std::make_pair(  i*b + j-1, 1.));
			}
			this->coord.at(k) = std::make_pair(i, j);
			k++;
		}		
	}
}

std::istream& operator >>(std::istream& is, Labyrinth& lg)
{
	assert(is.good());
	size_t h, b;
	is >> h; is >> b;
	assert(h >0 && b >0);
	std::vector<CellType> cells; cells.resize(h*b);
	char c; size_t k = 0;
	for(size_t i = 0; i < h; i++) {
		for(size_t j = 0; j < b; j++) {
			assert(is.good());
			is >> c; assert(c == '.' || c == '#');
			cells.at(k) = ( c == '.' ? CellType::Ground : CellType::Wall);
			k++	;
		}
	}
	lg.Initialize(cells, h, b);
	return is;
}



