#ifndef GRAPHVISUALIZER_H
#define GRAPHVISUALIZER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <chrono>
#include "util.h"
#include "graph.h"


class Arrow : public sf::Drawable, public sf::Transformable
{
public:
	Arrow();
	Arrow(sf::Vector2f base);	
	Arrow(sf::Vector2f base, sf::Color color);

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices ;
};



class SFMLVisualizer : public GraphVisualizer
{
protected:
	std::shared_ptr<sf::RenderWindow> rWnd;
	
	std::shared_ptr<CoordinateGraph> cGraph;
	
	std::map<VertexT, VertexStatus> vertState;
	std::map<EdgeT, EdgeStatus> edgeState;
	std::map<VertexT, std::pair<double, double> > vertCosts;
	
	sf::Color bgColor;
	sf::Font font;
	sf::Clock lastDraw;
	
	static sf::Vector2f mapToScreen(sf::FloatRect space, sf::FloatRect screen, sf::Vector2f spacePos);
	
	virtual std::function<sf::Vector2f(VertexT)> mapVertexPosition(sf::Vector2f& vertSize) const;
	
	virtual void drawVertex(VertexT v, std::function<sf::Vector2f(VertexT)> mVP, sf::Vector2f vertSize);
	
	virtual void drawEdges(VertexT v, std::function<sf::Vector2f(VertexT)> mVP, sf::Vector2f vertSize);
	
	void drawGraph();
	
public:
	bool DrawEstimates;
	double timeout;
	
	SFMLVisualizer(std::shared_ptr<sf::RenderWindow> renderWindow, std::shared_ptr<CoordinateGraph> coordinateGraph, sf::Font f) ;
	
	void ResetStates();
	
    void markVertex(VertexT vertex, VertexStatus status) override;
	
	void markPath(PathT p) override;

    void markEdge(EdgeT e, EdgeStatus status) override ;
    
    // Aktualisiere jegliche Daten eines Knotens.
    void updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status) override;
    
    void draw() override ;
	
};

class SFMLLabVisualizer : public SFMLVisualizer
{
protected:
	std::shared_ptr<Labyrinth> lGraph;
public:
	SFMLLabVisualizer(const std::shared_ptr<sf::RenderWindow>& renderWindow, const std::shared_ptr<Labyrinth>& lGraph);
	
	std::function<sf::Vector2f(VertexT)> mapVertexPosition(sf::Vector2f& vertSize) const override;
	
	void drawVertex(VertexT v, std::function<sf::Vector2f(VertexT)> mVP, sf::Vector2f vertSize) override;
	
	void drawEdges(VertexT v, std::function<sf::Vector2f(VertexT)> mVP, sf::Vector2f vertSize) override;
};


#include "graph_visualizer.cpp"

#endif
