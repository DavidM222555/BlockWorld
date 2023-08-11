#pragma once

#include <vector>
#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/hash.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "BufferLayout.h"

#include "Cube.h"

using std::vector;
using std::unordered_set;
using std::array;
using std::unordered_map;

const int CHUNK_SIZE = 16;

const unsigned int BUFFER_SIZE = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 36 * 6 * sizeof(float);

class Chunk
{
private:
	vector<float> chunkBuffer;

public:
	std::array<std::array<std::array<Cube, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> _cubes;

	// Used to find if a given piece should be drawn or not and factored into consideration when drawing other pieces
	inline bool findIfPieceIsActive(int x, int y, int z) { return _cubes[x][y][z].getActive(); }

	// Test whether or not a given set of coordinates falls within the minimum and maximum ranges for x, y, and z
	inline bool pieceInBounds(int x, int y, int z) 
	{
		return x > 0 && y > 0 && z > 0 && x < CHUNK_SIZE - 1 && y < CHUNK_SIZE - 1 && z < CHUNK_SIZE - 1;
	}

	inline vector<float> getBuffer() { return chunkBuffer; }
	inline void setBuffer(vector<float>& otherBuffer) { chunkBuffer = otherBuffer; }
	inline void clearBuffer() { chunkBuffer.clear(); }

};
