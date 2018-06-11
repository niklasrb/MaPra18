#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <utility>
#include <vector>
#include <functional>


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





#endif
