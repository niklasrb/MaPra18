#include "include.h"


void WaitForKeyPress(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<SFMLVisualizer> gv)
{
	while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            switch (event.type) {
				case sf::Event::Closed:
					window->close();
					break;
				case sf::Event::KeyPressed:
					return;
				case  sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Right)	{
						gv->DrawEstimates = !gv->DrawEstimates;
					}
					break;
				case sf::Event::MouseWheelScrolled:
					if(event.mouseWheelScroll.delta > 0)
						gv->timeout *= 1.1;
					else
						gv->timeout *= 0.9;
					std::cout << "changed timeout " << gv->timeout << std::endl;
					break;
			}
			auto to = gv->timeout;
			gv->timeout = 0;
			gv->draw();
			gv->timeout = to;
        }
    }
    return;
}


void LoadExample(std::shared_ptr<sf::RenderWindow> rWnd, sf::Font font, int bsp = 1)
{
	std::stringstream ss;
	std::fstream f;
	std::shared_ptr<CoordinateGraph> dg;
	std::shared_ptr<SFMLVisualizer> gv;

	ss.str("");
	ss << "daten/Graph" << bsp << ".dat";
	f.open(ss.str(), std::fstream::in);
	EuclideanDistanceGraph cg((bsp == 2 ? 1./2. : 1.));
	f >> cg; f.close();
	
	dg = std::make_shared<EuclideanDistanceGraph>(cg);
	gv = std::make_shared<SFMLVisualizer>(rWnd, dg, font);
	
	
	for(size_t i = 0; i < dg->numVertices(); i++) {
		std::vector<CostT> cost;
		rWnd->setTitle(std::string("Beispiel ") + std::to_string(bsp) + std::string(" - Dijkstra von vertex ") + std::to_string(i));
		Dijkstra(*dg, *gv, i, cost);
		//PruefeDijkstra(bsp, i, cost);
		
		WaitForKeyPress(rWnd, gv);
		if(!rWnd->isOpen())
			return;
		
		for(size_t j = 0; j < dg->numVertices(); j++) {
			if(i == j)continue;
			std::cout << "Pruefe Weg " << i << " -> " << j << std::endl;
			std::list<VertexT> weg;
			rWnd->setTitle(std::string("Beispiel ") + std::to_string(bsp) + std::string(" - A* von vertex ") + std::to_string(i));
			A_star(*dg, *gv, i, j, weg);
			
			WaitForKeyPress(rWnd, gv);
			if(!rWnd->isOpen())
				return;
		}
	}	
}

void LoadLab(std::shared_ptr<sf::RenderWindow> rWnd, int bsp = 1)
{
	int offset = 4;
	std::stringstream ss;
	std::fstream f;
	std::shared_ptr<Labyrinth>  lab_ptr;
	
	ss.str("");
	ss << "daten/Maze" << bsp << ".dat";
	std::cout << "ss = " << ss.str() << std::endl;
	f.open(ss.str(), std::fstream::in);
	Labyrinth lg;
	f >> lg;
	lab_ptr = std::make_shared<Labyrinth>(lg);
	f.close();
	auto gv = std::make_shared<SFMLLabVisualizer>(rWnd, lab_ptr);
	
	//std::cout << "Maze " << bsp << ": " << (PruefeHeuristik(*lab_ptr) ? "Heuristik ok" : "Heuristik fehlerhaft") << std::endl;
	
	auto szp = StartZielPaare(bsp + offset);
	
	for(auto it = szp.begin(); it != szp.end(); ++it) {
		std::cout << "Finde Weg " << it->first << " -> " << it->second << std::endl;
		std::list<VertexT> weg;
		rWnd->setTitle(std::string("Labyrint ") + std::to_string(bsp) + std::string(" - A* von vertex ") + std::to_string(it->first) + std::string(" -> ") + std::to_string(it->second));
		A_star(*lab_ptr, *gv, it->first, it->second, weg);
		WaitForKeyPress(rWnd, gv);
		if(!rWnd->isOpen())
			return;
		//if(A_star(*lab_ptr, *gv, it->first, it->second, weg))
			//PruefeWeg(bsp + offset, weg);
		//else
		//	std::cout << "keinen Weg gefunden" << std::endl;
	}
		
}


int main()
{
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 800), "SFML works!");
    sf::Font font;
	assert(font.loadFromFile("font/BebasNeue-Regular.ttf"));

	int i = 1;

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
                
            if (event.type == sf::Event::KeyPressed) {
				if( i < 5)
					LoadExample(window, font, i);
				else if(i >= 5 && i < 10)
					LoadLab(window, i-5);
				else
					i = 0;
				i++;
			}
        }

        window->clear(sf::Color::White);
        window->display();
    }

    return 0;
}
