#ifndef GRAPH_H
#define GRAPH_H

#include <functional>
#include <list>
#include <utility>
#include <vector>
#include <iostream>

#include "util.h"

typedef std::size_t                 VertexT;
typedef std::pair<VertexT,VertexT>  EdgeT;
typedef std::list<VertexT>			PathT;
typedef double                      CostT;

extern const double infty;
extern const VertexT undefinedVertex;

//-------------------------------------------------------------------------------------------------
// Abstrakte Basisklasse zur Repraesentation eines Distanzgraphen
//-------------------------------------------------------------------------------------------------

class DistanceGraph
{
  public:
    typedef std::pair<VertexT,CostT>  LocalEdgeT;
    typedef std::vector<LocalEdgeT>   NeighborT;
    
  protected:
    // Die Anzahl der Knoten im Graphen.
    std::size_t vertexCount;
    
  public:
    DistanceGraph( int num_verts= 0)
      : vertexCount(num_verts) {}
      
    virtual ~DistanceGraph() {}

    // Gibt die Anzahl der Knoten im Graphen zurueck.
    std::size_t numVertices() const { return vertexCount; }

    // Gibt die Nachbarknoten eines gegebenen Knotens v zurueck.
    virtual const DistanceGraph::NeighborT& getNeighbors( VertexT v) const = 0;
    
    // Heuristik: Schaetzt die Kosten, um vom Knoten "from" zum Knoten "to" zu gelangen.
    virtual CostT estimatedCost( VertexT from, VertexT to) const = 0;
    
    // Gibt die wahren Kosten c an, um vom Knoten "from" zum Knoten "to" zu gelangen.
    // Existiert zwischen den beiden Knoten keine Kante, werden die Kosten mit infty bewertet.
    virtual CostT cost( VertexT from, VertexT to) const = 0;
};


// Ein Graph, der Koordinaten von Knoten speichert.
class CoordinateGraph : public DistanceGraph {
protected:
	std::vector<NeighborT> neighbors;
	std::vector<std::pair<double, double> > coord;
public:
    const DistanceGraph::NeighborT& getNeighbors( VertexT v) const override;
    
    virtual CostT estimatedCost( VertexT from, VertexT to) const = 0;
    
    CostT cost( VertexT from, VertexT to) const override;
    
    std::pair<double, double> position(VertexT v) const;
    
    friend std::istream& operator >>(std::istream& is, CoordinateGraph& cg);
    
    void Print() const;
};


class EuclideanDistanceGraph : public CoordinateGraph { 
protected:
	double normModifier = 1.;
public:	
	EuclideanDistanceGraph(double normModifier);
	 CostT estimatedCost( VertexT from, VertexT to) const override;
};
//-------------------------------------------------------------------------------------------------
// Diese Enumeration beschreibt die moeglichen Zellentypen im Labyrinth.
//-------------------------------------------------------------------------------------------------
enum class CellType {
    Wall,
    Ground,
    Start,
    Destination
};

//--------------------------
// Diese Klasse ermoeglicht einfache Interaktion mit vector<CellType>
//---------------------------
class Labyrinth : public CoordinateGraph
{
protected:
	std::vector<CellType> cells;
public:
	void Initialize(const std::vector<CellType>& cells, size_t h, size_t b);
	CostT estimatedCost( VertexT from, VertexT to) const override;
	friend std::istream& operator >>(std::istream& is, Labyrinth& lg);
};



//-------------------------------------------------------------------------------------------------
// Eine Liste der moeglichen Zustaende, die ein Knoten haben kann.
//-------------------------------------------------------------------------------------------------
enum class VertexStatus { UnknownVertex, InQueue, Active, Done, Destination, Origin };

//-------------------------------------------------------------------------------------------------
// Eine Liste der moeglichen Zustaende, die eine Kante haben kann.
//-------------------------------------------------------------------------------------------------
enum class EdgeStatus   { UnknownEdge, Visited, Active, Optimal };

//-------------------------------------------------------------------------------------------------
// Eine abstrakes Interface, das Methoden fuer eine Visualisierung von Graphalgorithmen beinhaltet.
//-------------------------------------------------------------------------------------------------
class GraphVisualizer
{
  public:
    virtual ~GraphVisualizer() {}

    // Zeige an, dass sich ein Knoten jetzt in dem angegebenen Zustand befindet.
    virtual void markVertex(VertexT vertex, VertexStatus status) = 0;
    
    // Zeige an, dass sich eine Kante im angegebenen Zustand befindet.
    virtual void markEdge(EdgeT e, EdgeStatus status) = 0;
    
    virtual void markPath(PathT p) = 0;
    
    // Aktualisiere jegliche Daten eines Knotens.
    virtual void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) = 0;
    
    // Zeichne den aktuellen Zustand des Graphen.
    virtual void draw() = 0;
    
    virtual void ResetStates() = 0;
};








#include "graph.cpp"


#endif
