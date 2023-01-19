#include "ChunkReader.h"

ChunkReader::ChunkReader(directory dirName)
{
	_dirName = dirName;
}

Chunk* ChunkReader::readChunk(int x, int y, int z)
{
	Chunk* decodedChunk = new Chunk();
	string inputFileName = _dirName + "/chunk" + "_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z) + ".chf";

	std::ifstream inputFile;
	inputFile.open(inputFileName);

	if (!inputFile)
	{
		std::cout << "Error reading file: " << inputFileName << "\n";
		return nullptr;
	}

	// Write the file to a string
	std::stringstream ss;
	ss << inputFile.rdbuf();
	string rleString = ss.str();

	decodedChunk->_cubes = decodeRunLengthEncoding(rleString);

	return decodedChunk;
}