#pragma once

#include "FastNoiseLite.h"
#include <vector>
#include <iostream>

using std::vector;

class NoiseGenerator
{
public:
	vector<vector<int>> _heightMap;
	NoiseGenerator(int minHeight, int maxHeight, int xRange, int yRange);

};

