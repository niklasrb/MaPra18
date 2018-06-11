/*****************************************************************
*  Name       : unit.h                                           *
*  Verwendung : Schnittstelle zu Praktikumsumgebung (MAPRA),     *
*               Distanzberechnung mit A* / Dijkstra              *
*  Autor      : Sven Gross, Hauke Sass, Johannes Stemick,        *
*               IGPM RWTH Aachen                                 *
*  Datum      : Mai 2018                                         *
*****************************************************************/

#ifndef _UNIT
#define _UNIT

#include "graph.h"

//-------------------------------------------------------------------------------------------------
// Prueft den gegebenen Distanzvektor D auf Korrektheit. Der i-te Eintrag von D enthaelt
// die Kosten, um vom Startvertex v0 zum Vertex i zu gelangen.
// Bsp ist die jeweilig untersuchte Beispielnummer.
//-------------------------------------------------------------------------------------------------
void PruefeDijkstra (int Bsp, VertexT v0, const std::vector<CostT>& D);

//-------------------------------------------------------------------------------------------------
// Prueft einen gegebenen Weg zwischen zwei Knoten fuer ein Beispiel auf Optimalitaet.
// Bsp ist die jeweilig untersuchte Beispielnummer.
//-------------------------------------------------------------------------------------------------
void PruefeWeg(int Bsp, const std::list<VertexT>& weg);

//-------------------------------------------------------------------------------------------------
// Diese Funktion prueft einen gegebenen Distanzgraphen auf Zulaessigkeit, d.h. ob die 
// Kostenschaetzung dieses Graphen die tatsaechlichen Kosten nie ueberschaetzt.
//-------------------------------------------------------------------------------------------------
bool PruefeHeuristik( const DistanceGraph& g);

//-------------------------------------------------------------------------------------------------
// Diese Methode gibt ein linearisiertes Labyrinth der Groesse breite x hoehe zurueck.
// Im zurueckgegebenen std::vector stehen die ersten <breite> Eintraege fuer die erste
// Zeile des Labyrinths, die naechsten <breite> Eintraege fuer die zweite Zeile usw.
// Es gibt jeweils genau eine Zelle mit dem Zelltyp Start und mit Destination, die den Start-
// bzw Zielknoten des zu findenden Weges markieren.
// Der Parameter seed wird benutzt, um den Zufallsgenerator zur Labyrinth-Erzeugung zu
// initialisieren - bei fester groesse erzeugt der gleiche seed das gleiche Labyrinth.
//-------------------------------------------------------------------------------------------------
std::vector<CellType> ErzeugeLabyrinth( int breite, int hoehe, unsigned int seed );

//-------------------------------------------------------------------------------------------------
// Diese Methode gibt fuer die Beispiele 5-9 einen std::vector von Start-End-Paaren
// zurueck, die der A*-Algorithmus loesen soll.
// Beispiel:
// for ( auto pair : StartZielPaare(9)) {
//   auto start = pair.first;
//   auto goal  = pair.second;
//   (Berechne den kuerzesten Weg von start zu goal)
// }
// Sollte Ihnen dieser Code kryptisch vorkommen, informieren Sie sich (online oder bei
// den HiWis) ueber den "range-based for loop" in C++11.
//-------------------------------------------------------------------------------------------------
std::vector<std::pair<VertexT,VertexT>> StartZielPaare( int Bsp );



#endif
