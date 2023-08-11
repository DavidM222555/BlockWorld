#pragma once

#include <string>
using std::string;

#include <fstream>

using directory = string;

#include "Chunk.h"

#include "RunLengthDecoding.h"

class ChunkReader
{
private:
	directory _dirName;

public:
	ChunkReader(directory dirName);
	Chunk* readChunk(int x, int y, int z);
};

