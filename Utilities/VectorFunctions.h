#pragma once

#include <algorithm>
#include "RandomFunctions.h"

#include <iostream>

// Get whether the vector contains the specified element
template<class T>
bool contains(std::vector<T> &vec, const T& value)
{
	return std::find(vec.begin(), vec.end(), value) != vec.end();
}

// Return a random element in the vector
// Do not use if the vector has no elements inside
template<class T>
T& randomElement(std::vector<T> &vec)
{
	/*if (vec.size() == 0) {
	return NULL;
	}*/
	return vec.at(randint(0, (int)(vec.size()) - 1));
}

template<class T>
void removeElementAtIndex(std::vector<T> &vec, int index) {
	vec.erase(vec.begin() + index);
}

// Remove the specified element from the vector, if it exists
template<class T>
void removeFromVector(std::vector<T> &vec, T element) {
	auto it = std::find(vec.begin(), vec.end(), element);

	if (it != vec.end()) {
		std::swap(*it, vec.back());
		vec.pop_back();
	}
}