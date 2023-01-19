#pragma once

#include <string>
using std::string;

#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "ChunkReader.h"
#include "ChunkWriter.h"


using directory = string;

class ChunkIO
{
private:
	ChunkReader _chunkReader;
	ChunkWriter _chunkWriter;
	directory _dirName;

public:
	ChunkIO(directory dirName);

	void writeChunk(Chunk* chunk, int x, int y, int z);
	Chunk* readChunk(int x, int y, int z);

	// Test if the chunk is currently in the current chunk directory.
	// Used for determining if we need to make a new chunk file or not.
	bool containsChunk(int x, int y, int z);
};
