#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <vector>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <unordered_set>
#include <utility>
#include <memory>

#include "NoiseGenerator.h"
#include "Chunk.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "RunLengthEncoding.h"
#include "RunLengthDecoding.h"
#include "ChunkIO.h"

using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::unique_ptr;
using std::make_unique;
using std::min;
using std::max;

// Variables utilized when creating our height map, represents the minimum and maximum values that can occur in our noise 
// generation function.
const int MIN_HEIGHT = -1;
const int MAX_HEIGHT = 20;

using GridCoordinate = glm::vec3;

class ChunkManager
{
private:
	unordered_map<GridCoordinate, unique_ptr<Chunk>> _chunkMap;
	unordered_set<GridCoordinate> activeChunkCoordinates;

	int _xRange;
	int _yRange;
	int _zRange;

	ChunkIO cio;

public:
	// These represent the number of chunks to the right and left, above and below, and forward and behind
	// To just represent a single slice on the xy-grid consider the following:
	// | (-1, 1)  | (0, 1)  | (1, 1)
	// | (-1, 0)  | (0, 0)  | (1, 0)
	// | (-1, -1) | (0, -1) | (-1, 1)
	ChunkManager(int xRange, int yRange, int zRange);

	// Gets all buffers within a given radius of a center.
	// This algorithm can effectively be viewed as handling the rendering of items within
	// a given render distance -- which we call radius.
	vector<float> getBuffersInArea(int centerX, int centerY, int centerZ, int radius);

	vector<float> getBuffersCurrentlyInMap();

	// Used for modifying the chunk and buffers for a given 
	void updateChunkAndNeighbors(int x, int y, int z);

	GridCoordinate getChunkPlayerIsIn(float x, float y, float z);
	Chunk* accessChunkAtPosition(float x, float y, float z);

	void setCubeAtPosition(float x, float y, float z, bool value);
	Cube& accessCubeAtPosition(float x, float y, float z);

	inline bool inBounds(int x, int y, int z) {
		return x > -1 * CHUNK_SIZE * _xRange && x < CHUNK_SIZE * _xRange - 1
			&& y > MIN_HEIGHT - 1 && y < MAX_HEIGHT + 1
			&& z > -1 * CHUNK_SIZE * _zRange && z < CHUNK_SIZE * _zRange - 1;
	}

	Cube& accessCubeAtIntegerPosition(Chunk* chunk, int x, int y, int z);
	void setCubeAtIntegerPosition(Chunk* chunk, int x, int y, int z);

	bool testIfPieceIsVisible(int x, int y, int z);
	bool testIfPieceIsVisibleAtIntegerPosition(Chunk* chunk, int x, int y, int z);

	inline bool isChunkInMap(int x, int y, int z) { return _chunkMap.find(glm::vec3(x, y, z)) != _chunkMap.end(); }

	// Generates the necessary buffer to render a given chunk. This method is implemented here in the chunk manager as opposed to 
	// in the chunk class itself so we can access blocks on the border of a given chunk. 
	vector<float> createBuffer(Chunk* chunk, int chunkX, int chunkY, int chunkZ);
};
