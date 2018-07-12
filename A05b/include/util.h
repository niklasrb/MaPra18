#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <utility>
#include <vector>
#include <functional>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


template<typename T>
bool contains(const std::vector<T>& v, const T& e)
{
	for(auto it = v.begin(); it != v.end(); ++it)
		if((*it) == e)
			return true;
	return false;
}

template<typename T>
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

template<typename T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& v)
{
	os << "[";
	for(size_t i = 0; i < v.size(); i++) 
		os << v.at(i) << (i < v.size()-1 ?  ", " : "]");
	return os;
}

template<typename T, typename S>
std::ostream& operator <<(std::ostream& os, const std::pair<T, S>& p)
{
	return os << "[" << p.first << ", " << p.second << "]";
}

template<typename T>
std::ostream& operator <<(std::ostream& os, const sf::Vector2<T> v)
{
	return os << "[" << v.x << ", " << v.y << "]";
}

template<typename T>
std::ostream& operator <<(std::ostream& os, const sf::Rect<T> r)
{
	return os << "[ " << sf::Vector2<T>(r.left, r.top) << " ,[" <<  sf::Vector2<T>(r.left+r.width, r.top+r.height)  << "]";
}

template<typename T> 
T norm(const sf::Vector2<T> v)
{
	return sqrt(pow(v.x, 2) + pow(v.y, 2));
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

template<typename T>
T insideOf(const T& left, const T& right, const T& x)
{
	if(x > right)
		return right;
	if(x < left) 
		return left;
	return x;
}





#endif
