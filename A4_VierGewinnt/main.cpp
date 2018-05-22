
#include <cmath>
#include <iostream>
#include <random>
#include "4gspiel.h"
#include "4gki.h"
#include "unit.h"


std::shared_ptr<VG::NetzwerkSpieler> InitialisiereNetzwerk(VG::Feld& spieler1Farbe);

int main(int argc, char** argv)
{
	std::shared_ptr<VG::Spieler> spieler1, spieler2;
	std::vector<std::shared_ptr<VG::Beobachter> > beobachter;
	char c; int i; int anzSpiele = -1;
	auto spieler1Farbe = VG::Feld::gelb;
	
	std::cout << "Spieler 1 ist Mensch (M) oder KI (K)? " << std::endl;
	//std::cin.clear();
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::cin >> c;
    switch(c)  {
		case 'M':
		case 'm':
		default:
			spieler1 = std::make_shared<VG::LokalerSpieler>("Spieler 1");
			break;
		case 'K':
		case 'k':
			std::cout << "KI-tiefe: "; std::cin >> i; 
			spieler1 = std::make_shared<VG::KI>(std::max(i, 1), "KI 1");
			beobachter.push_back(std::make_shared<VG::LokalerBeobachter>() );
			break;		
	}
	
	std::cout << "Spieler 2 ist Praktikumsumgebung (P), KI (K), oder im Netzwerk(N)? " << std::endl;
	std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::cin >> c;
    switch(c) {
		case 'P':
		case 'p':
			std::cout << "Schwierigkeitsgrad (1-4):"; std::cin >> i;
			if(i < 1 || i > 5) i =1;
			spieler2 = std::make_shared<VG::Praktikumsumgebung>(i);
			spieler1Farbe = ((AnzahlSpiele % 2) == 0 ?  VG::Feld::rot : VG::Feld::gelb);
			anzSpiele = AnzahlSpiele;
			break;
		case 'K':
		case 'k':
		default:
			std::cout << "KI-tiefe: "; std::cin >> i; 
			spieler2 = std::make_shared<VG::KI>(std::max(i,1), "KI 2");
			break;
		case 'N':
		case 'n':
			spieler2 = InitialisiereNetzwerk(spieler1Farbe);
			break;
	}
	
	VG::Spiel S(AnzahlZeilen, AnzahlSpalten, spieler1, spieler2, beobachter);
	VG::Feld sieger;
	
	while( anzSpiele != 0) {	// 
		sieger = S.Spielen(spieler1Farbe);	// <- Spiel
		std::cout << (spieler1Farbe == sieger ? spieler1->Name : (sieger != VG::Feld::leer ? spieler2->Name : "Keiner ")) << " hat gewonnen" << std::endl;
		if(anzSpiele < 0) {		// Spiele bis keiner mehr Lust hat
			std::cin.clear();
			std::cout << "Noch ein Spiel? (y/n)" << std::endl;
			std::cin >> c;
			if(c != 'y')
				anzSpiele = 0;
		} else {				// Beim Spiel gegen die Prakitkumsumgebung
			anzSpiele--;
		}
		spieler1Farbe = S.NaechsterSpieler(spieler1Farbe);	// Wechsle Farben fuer das naechste Spiel
	} 
	 
	return 0;
}

// Sammle Parameter fuer Netzwerkspiel
std::shared_ptr<VG::NetzwerkSpieler> InitialisiereNetzwerk(VG::Feld& spieler1Farbe)
{
	int command = 0;
	while (true) {    
        std::cout << "\n";
        std::cout << "1 = VERBINDE  mit einem anderen Spieler\n";
        std::cout << "2 = WARTE     auf einen anderen Spieler" << std::endl;
        std::cin >> command;
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        } else if (command >= 1 && command <= 2) {
            break;
        }
    }
    assert(command == 1 || command == 2);
         
    if (command == 1) {
        std::string ip;        
        unsigned short port = 0;
        
        std::cout << "Bitte geben Sie die IP ein, mit der Sie sich verbinden wollen. \n";
        std::cout << "Format: xxx.xxx.xxx.xxx" << std::endl;
        std::cin >> ip;
        std::cout << "Bitte geben Sie den Port ein, mit dem Sie sich verbinden wollen (31000-31009): \n";
        std::cin >> port;
        spieler1Farbe = VG::Feld::rot;
        return std::make_shared<VG::NetzwerkSpieler>(true, port, ip);
    } else {
        unsigned short port = 0;
        
        std::cout << "Bitte geben Sie den Port ein, mit dem sich Ihr Mitspieler verbinden soll (31000-31009): \n";
        std::cin >> port;
        std::cout << "Warte auf Gegner..." << std::endl;
        spieler1Farbe = VG::Feld::gelb;
        return std::make_shared<VG::NetzwerkSpieler>(false, port);
    }
}

