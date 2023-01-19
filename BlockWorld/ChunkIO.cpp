#include "ChunkIO.h"

ChunkIO::ChunkIO(directory dirName) : _chunkWriter(dirName), _chunkReader(dirName)
{
	_dirName = dirName;
}

void ChunkIO::writeChunk(Chunk* chunk, int x, int y, int z)
{
	_chunkWriter.writeChunk(chunk, x, y, z);
}

Chunk* ChunkIO::readChunk(int x, int y, int z)
{
	return _chunkReader.readChunk(x, y, z);
}

bool ChunkIO::containsChunk(int x, int y, int z)
{
	string filePath = _dirName + "/" + "chunk_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z) + ".chf";

	return fs::exists(filePath);
}
