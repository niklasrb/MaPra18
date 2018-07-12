#ifndef GRAPHVISUALIZER_CPP
#define GRAPHVISUALIZER_CPP
#include "graph_visualizer.h"


Arrow::Arrow() : m_vertices(sf::Lines, 6)
{
	m_vertices[0].position = sf::Vector2f(0.f, 0.f);
	m_vertices[1].position = sf::Vector2f(1.f, 0.f);
	m_vertices[2].position = sf::Vector2f(1.f, 0.f);
	m_vertices[3].position = sf::Vector2f(0.97f, 0.03f);
	m_vertices[4].position = sf::Vector2f(1.f, 0.f);
	m_vertices[5].position = sf::Vector2f(0.97f, -0.03f);
}

Arrow::Arrow(sf::Vector2f base) : Arrow()
{
	this->setRotation( 180.f/M_PI*atan2(base.y, base.x));
	this->setScale(norm(base), norm(base));
}
	
Arrow::Arrow(sf::Vector2f base, sf::Color color) : Arrow(base)
{
	for(uint i = 0; i < 6; i++)
		m_vertices[i].color = color;
}

void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;
	target.draw(m_vertices, states);
}



sf::Vector2f SFMLVisualizer::mapToScreen(sf::FloatRect space, sf::FloatRect screen, sf::Vector2f spacePos)
{
	return sf::Vector2f((spacePos.x - space.left)/space.width*screen.width + screen.left,
									(spacePos.y - space.top)/space.height*screen.height + screen.top); 
}
	
std::function<sf::Vector2f(VertexT)> SFMLVisualizer::mapVertexPosition(sf::Vector2f& vertSize) const
{
	float windowScale = 0.95;
	int nVert = cGraph->numVertices();
	sf::FloatRect windowSize(rWnd->getSize().x*0.05, rWnd->getSize().y*0.05, rWnd->getSize().x*0.9, rWnd->getSize().y*0.9);
	std::pair<double, double> coord;
	sf::FloatRect vertexSpace;
	
	vertSize = sf::Vector2f(insideOf(20.f, 100.f, windowSize.width/float(2*nVert)), insideOf(20.f, 100.f, windowSize.height/float(2*nVert)));
	vertSize.y = vertSize.x = std::min(vertSize.x, vertSize.y);
	
	coord = cGraph->position(0);
	vertexSpace.left = coord.first; vertexSpace.width = coord.first;
	vertexSpace.top = coord.second; vertexSpace.height = coord.second;
	
	for(VertexT v = 1; v < cGraph->numVertices(); v++) {
		coord = cGraph->position(v);
		vertexSpace.left = std::min(vertexSpace.left, (float)coord.first);
		vertexSpace.top = std::min(vertexSpace.top, (float)coord.second);
		vertexSpace.width = std::max(vertexSpace.width, (float)coord.first);
		vertexSpace.height = std::max(vertexSpace.height, (float)coord.second);
	}
	vertexSpace.width -= vertexSpace.left;
	vertexSpace.height -= vertexSpace.top;
	
	windowSize.width -= vertSize.x;
	windowSize.height -= vertSize.y;
	
	//std::cout << "window size " << windowSize << std::endl;
	//std::cout << "numVertices " << cGraph->numVertices() << std::endl;
	//std::cout << "Estimated vertex size to be " << vertSize << std::endl;
	//std::cout << "vertexSpace " << vertexSpace << std::endl;
	
	return [vertexSpace, windowSize, windowScale, this] (VertexT v)
			{	auto p = this->cGraph->position(v);
				return mapToScreen(vertexSpace,windowSize, sf::Vector2f(p.first, p.second));};
	
}
	
void SFMLVisualizer::drawVertex(VertexT v, std::function<sf::Vector2f(VertexT)> mVP, sf::Vector2f vertSize)
{
	double rad = std::min(vertSize.x, vertSize.y)/2.;
	auto pos = mVP(v);
	sf::CircleShape cs(rad);
	sf::Color inside, outline = sf::Color::Black;
	switch(vertState.at(v)) {
		case VertexStatus::Origin:
			inside = sf::Color::Green;	break;
		case VertexStatus::Active:
			inside = sf::Color::Blue;	break;
		case VertexStatus::Destination:
			inside = sf::Color::Red;	break;
		case VertexStatus::Done:
			inside = sf::Color(0, 0, 200);	break;
		case VertexStatus::InQueue:
			inside = sf::Color(106, 171, 142);	break;
		default:
			inside = sf::Color(128, 128, 128);
	}
	cs.setFillColor(inside);
	cs.setOutlineThickness(rad*0.1);
	cs.setOutlineColor(outline);
	cs.setPosition(pos);
	rWnd->draw(cs);
	
	sf::Text label;
	int textSize = 20;
	label.setFont(font); // font is a sf::Font
	label.setString(std::to_string(v));
	label.setCharacterSize(textSize); // in pixels, not points!
	label.setColor(sf::Color::Black);
	label.setStyle(sf::Text::Bold);
	label.setPosition(pos + vertSize/2.f - sf::Vector2f(label.getLocalBounds().width/2.f, label.getLocalBounds().height/2.f)); 
	rWnd->draw(label);
	
	if(!DrawEstimates) return;
	sf::Text est;
	est.setFont(font); // font is a sf::Font
	auto costs = vertCosts.at(v);
	est.setString((costs.first == infty ? std::string("inf") : to_string_with_precision(costs.first, 2)) + 
							(costs.second != 0 ? (costs.second == infty ? std::string(" - inf") : std::string(" - ") + to_string_with_precision(costs.second, 2)) : std::string("")));
	est.setCharacterSize(textSize); // in pixels, not points!
	est.setColor(sf::Color::Black);
	est.setStyle(sf::Text::Bold);
	est.setPosition(pos + vertSize - sf::Vector2f(label.getLocalBounds().width/2.f, label.getLocalBounds().height/2.f)); 
	rWnd->draw(est);
}
	
void SFMLVisualizer::drawEdges(VertexT v, std::function<sf::Vector2f(VertexT)> mVP, sf::Vector2f vertSize)
{
	sf::Vector2f connectingVector;
	sf::Color edgeColor;
	float rad = vertSize.x*0.5;
	auto neighbors = cGraph->getNeighbors(v);
	for(auto it = neighbors.begin(); it != neighbors.end(); ++it) {
		EdgeT e = std::make_pair(v, it->first);
		switch(edgeState.at(e)) {
			case EdgeStatus::Active:
				edgeColor = sf::Color::Blue;	break;
			case EdgeStatus::Optimal:
				edgeColor = sf::Color::Green;	break;
			case EdgeStatus::Visited:
				edgeColor =  sf::Color(50, 50, 50);	break;
			default:
				edgeColor = sf::Color(128, 128, 128);	break;
		}
		connectingVector = mVP(e.second) - mVP(e.first);

		Arrow a(Arrow((1.f - 2.f*rad/norm(connectingVector) )*connectingVector, edgeColor));
		a.setPosition(mVP(e.first) + vertSize*0.5f + (1.01f*rad/norm(connectingVector))*connectingVector);
		rWnd->draw(a);
		
		sf::Text cost;	cost.setFont(font);
		cost.setString(to_string_with_precision(it->second, 2));
		cost.setCharacterSize(20); 
		cost.setColor(sf::Color::Black); cost.setStyle(sf::Text::Bold);
		cost.setPosition(mVP(e.first) + vertSize*0.5f + 0.55f*connectingVector);
		rWnd->draw(cost);
	}
}
	
void SFMLVisualizer::drawGraph()
{
	sf::Vector2f vertSize;
	auto mVP = mapVertexPosition(vertSize);
	
	for(VertexT v = 0; v < cGraph->numVertices(); v++) {
		drawVertex(v, mVP, vertSize);
		drawEdges(v, mVP, vertSize);
	}
}

	
SFMLVisualizer::SFMLVisualizer(std::shared_ptr<sf::RenderWindow> renderWindow, std::shared_ptr<CoordinateGraph> coordinateGraph, sf::Font f) 
	: rWnd(renderWindow), cGraph(coordinateGraph), bgColor(sf::Color::White), font(f)
{	
	DrawEstimates = true;
	timeout = 300;
	ResetStates();
}
	
void SFMLVisualizer::ResetStates()
{
	for(VertexT v = 0; v < cGraph->numVertices(); v++) {
		vertState[v] = VertexStatus::UnknownVertex;
		vertCosts[v] = std::make_pair(infty, infty);
		auto neighbors = cGraph->getNeighbors(v);
		for(auto it = neighbors.begin(); it != neighbors.end(); ++it) {
			EdgeT e = std::make_pair(v, it->first);
			edgeState[e] = EdgeStatus::UnknownEdge;
		}
	}
}
	

void SFMLVisualizer::markVertex(VertexT vertex, VertexStatus status)
{
	vertState[vertex] = status;
	draw();
}
	
void SFMLVisualizer::markPath(PathT p)
{
	/*for(VertexT v = 0; v < cGraph->numVertices(); v++) {
		auto neighbors = cGraph->getNeighbors(v);
		for(auto it = neighbors.begin(); it != neighbors.end(); ++it) {
			EdgeT e = std::make_pair(v, it->first);
			edgeState[e] = EdgeStatus::UnknownEdge;
		}
	}*/
	auto v = *p.begin();
	for(auto it = ++p.begin(); it != p.end(); ++it) {
		EdgeT e = std::make_pair(v, *it); 
		v = *it;
		edgeState[e] = EdgeStatus::Optimal;
	}
	
	draw();
}

void SFMLVisualizer::markEdge(EdgeT e, EdgeStatus status) 
{
	edgeState[e] = status;
	draw();
}

void SFMLVisualizer::updateVertex(VertexT vertex, double cost, double estimate, VertexT parent, VertexStatus status)
{
	vertCosts[vertex] = std::make_pair(cost, estimate);
	markVertex(vertex, status);
}
    
void SFMLVisualizer::draw() 
{
	double elapsed = lastDraw.getElapsedTime().asMilliseconds();
	
	if( elapsed < timeout)
		 sf::sleep(sf::milliseconds(timeout - elapsed));
	rWnd->clear(bgColor);
	drawGraph();
	rWnd->display();
	lastDraw.restart();
}



SFMLLabVisualizer::SFMLLabVisualizer(const std::shared_ptr<sf::RenderWindow>& renderWindow, const std::shared_ptr<Labyrinth>& lGraph)
	: 	SFMLVisualizer(renderWindow, lGraph, sf::Font()), lGraph(lGraph)
{	
	DrawEstimates = false;
	timeout = 100;
}

std::function<sf::Vector2f(VertexT)> SFMLLabVisualizer::mapVertexPosition(sf::Vector2f& vertSize) const 
{
	float windowScale = 0.95;
	sf::FloatRect windowSize(rWnd->getSize().x*0.05, rWnd->getSize().y*0.05, rWnd->getSize().x*0.9, rWnd->getSize().y*0.9);
	std::pair<double, double> coord;
	sf::FloatRect vertexSpace;
	
	coord = cGraph->position(0);
	vertexSpace.left = coord.first; vertexSpace.width = coord.first;
	vertexSpace.top = coord.second; vertexSpace.height = coord.second;
	
	for(VertexT v = 1; v < cGraph->numVertices(); v++) {
		coord = cGraph->position(v);
		vertexSpace.left = std::min(vertexSpace.left, (float)coord.first);
		vertexSpace.top = std::min(vertexSpace.top, (float)coord.second);
		vertexSpace.width = std::max(vertexSpace.width, (float)coord.first);
		vertexSpace.height = std::max(vertexSpace.height, (float)coord.second);
	}
	vertexSpace.width -= vertexSpace.left;
	vertexSpace.height -= vertexSpace.top;
	
	vertSize = sf::Vector2f(windowSize.width/vertexSpace.width, windowSize.height/vertexSpace.height);
	
	windowSize.width -= vertSize.x;
	windowSize.height -= vertSize.y;
	
	//std::cout << "window size " << windowSize << std::endl;
	//std::cout << "numVertices " << cGraph->numVertices() << std::endl;
	//std::cout << "Estimated vertex size to be " << vertSize << std::endl;
	//std::cout << "vertexSpace " << vertexSpace << std::endl;
	
	return [vertexSpace, windowSize, windowScale, this] (VertexT v)
			{	auto p = this->cGraph->position(v);
				return mapToScreen(vertexSpace,windowSize, sf::Vector2f(p.first, p.second));};
}
	
void SFMLLabVisualizer::drawVertex(VertexT v, std::function<sf::Vector2f(VertexT)> mVP, sf::Vector2f vertSize) 
{
	auto pos = mVP(v);
	sf::RectangleShape rs(vertSize);
	sf::Color inside = sf::Color(180, 180, 180);
	switch(vertState.at(v)) {
		case VertexStatus::Origin:
			inside = sf::Color::Green;	break;
		case VertexStatus::Active:
			inside = sf::Color::Blue;	break;
		case VertexStatus::Destination:
			inside = sf::Color::Red;	break;
		case VertexStatus::Done:
			inside = sf::Color(200, 200, 200);	break;
		case VertexStatus::InQueue:
			inside = sf::Color(50, 50, 180);	break;
		default:
			inside = sf::Color(50, 50, 50);
	}
	if(cGraph->getNeighbors(v).size() == 0)
		inside = sf::Color::Black;
	rs.setFillColor(inside);
	rs.setPosition(pos);
	rWnd->draw(rs);
}
	
void SFMLLabVisualizer::drawEdges(VertexT v, std::function<sf::Vector2f(VertexT)> mVP, sf::Vector2f vertSize) 
{
	sf::Color edgeColor;
	auto neighbors = lGraph->getNeighbors(v);
	for(auto it = neighbors.begin(); it != neighbors.end(); ++it) {
		EdgeT e = std::make_pair(v, it->first);
		if(edgeState.at(e) != EdgeStatus::Optimal)
			continue;
		sf::RectangleShape rs(vertSize);
		rs.setFillColor(sf::Color(0, 130, 0, 100));
		rs.setPosition(mVP(e.first));
		rWnd->draw(rs);
	}
}

	

#endif


