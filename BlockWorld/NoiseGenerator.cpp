#include "NoiseGenerator.h"

NoiseGenerator::NoiseGenerator(int minHeight, int maxHeight, int xRange, int yRange)
{
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);

	for (int i = -1 * xRange; i < xRange; i++) {
		vector<int> rowVector;

		for (int j = -1 * yRange; j < yRange; j++) {
			rowVector.push_back(round(abs(noise.GetNoise((float)i, (float)j)) * maxHeight));
		}

		_heightMap.push_back(rowVector);
	}

}
