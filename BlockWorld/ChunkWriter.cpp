#include "ChunkWriter.h"


ChunkWriter::ChunkWriter(directory dirName)
{
	_dirName = dirName;
}

void ChunkWriter::writeChunk(Chunk* chunk, int x, int y, int z)
{
	string encodedString = getRunLengthEncoding(chunk);
	string outputFile = _dirName + "/" + "chunk_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z) + ".chf";

	ofstream outFile;
	outFile.open(outputFile, std::ofstream::out | std::ofstream::trunc);

	if (!outFile) {
		std::cerr << "Error opening file " << std::endl;
		return;
	}

	outFile << encodedString;

	outFile.close();
}