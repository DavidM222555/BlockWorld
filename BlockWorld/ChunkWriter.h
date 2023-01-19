#pragma once

#include <string>
using std::string;

#include <fstream>
using std::ofstream;

#include <iostream>

#include "Chunk.h"
#include "RunLengthEncoding.h"

using directory = string;

class ChunkWriter
{
private:
	directory _dirName;

public:
	ChunkWriter(directory dirName);

	void writeChunk(Chunk* chunk, int x, int y, int z);
};

