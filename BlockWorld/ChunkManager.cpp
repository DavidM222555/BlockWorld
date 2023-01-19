#include "ChunkManager.h"

const float vertices[] = {
    // Front face
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    // Back face
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    // Leftward face
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    // Rightward face
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

    // Bottom facing face
   -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f,  0.5f,
   -0.5f, -0.5f, -0.5f,

   // Top facing face
   -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f,  0.5f,
   -0.5f,  0.5f, -0.5f,
};


ChunkManager::ChunkManager(int xRange, int yRange, int zRange) : cio("SavedChunks")
{
    auto chunkManagerStart = std::chrono::steady_clock::now();

	_xRange = xRange;
	_yRange = yRange;
	_zRange = zRange;

    NoiseGenerator ng(MIN_HEIGHT, MAX_HEIGHT, xRange * CHUNK_SIZE, zRange * CHUNK_SIZE);

    // Iterate over our entire map chunk by chunk
	for (int i = -1 * xRange; i < xRange; i++) {
		for (int j = -1 * yRange; j < yRange; j++) {
			for (int k = -1 * zRange; k < zRange; k++) {

                if (!cio.containsChunk(i, j, k)) {
                    // Chunk* chunkToInsert = new Chunk();
                    // Chunk* currentChunk = new Chunk();
                    unique_ptr<Chunk> currentChunk = make_unique<Chunk>();

                    // From here we now utilize the height map for generating just a single chunk.
                    // The alternative is to generate all of the chunks outside of this loop but since we are utilizing many of the same 
                    // looping variables (i, j, and k) it is more efficent to just do it here.
                    for (int x = 0; x < CHUNK_SIZE; x++) {
                        for (int z = 0; z < CHUNK_SIZE; z++) {
                            int xPosition = i * CHUNK_SIZE + x;
                            int zPosition = k * CHUNK_SIZE + z;
                            int heightMapValue = ng._heightMap[xRange * CHUNK_SIZE - xPosition - 1][zRange * CHUNK_SIZE - zPosition - 1];

                            int yCounter = 0;

                            for (int y = j * CHUNK_SIZE; y < std::min(j * CHUNK_SIZE + CHUNK_SIZE, heightMapValue); y++) {
                                currentChunk.get()->_cubes[x][yCounter][z].setActive(true);

                                yCounter += 1;
                            }
                        }
                    }

                    cio.writeChunk(currentChunk.get(), i, j, k);

                }
			}
		}
	}

    // std::cout << "Block at 32, 32, 32: " << accessCubeAtPosition(32, 32, 32).getActive() << "\n";

    auto chunkManagerEnd = std::chrono::steady_clock::now();
    std::chrono::duration<double> generateBufferTime = chunkManagerEnd - chunkManagerStart;
    std::cout << "Time to generate map: " << generateBufferTime.count() << "\n";
}


void ChunkManager::updateChunkAndNeighbors(int x, int y, int z)
{
    for (int i = max(x - 1, -1 * _xRange); i <= min(x + 1, _xRange); i++) {
        for (int j = max(y - 1, -1 * _yRange); j <= min(y + 1, _yRange); j++) {
            for (int k = max(z - 1, -1 * _zRange); k <= min(z + 1, _zRange); k++) {

                auto* chunkToModify = _chunkMap.at(GridCoordinate(i, j, k)).get();
                auto newBuffer = createBuffer(chunkToModify, i, j, k);

                chunkToModify->setBuffer(newBuffer);

                // cio.writeChunk(chunkToModify, i, j, k);
            }
        }
    }
}

vector<float> ChunkManager::getBuffersInArea(int centerX, int centerY, int centerZ, int radius)
{
    vector<float> buffer;

    // Used for removing chunk coordinates from activeChunkCoordinates and also deallocating cube arrays for chunks not 
    // currently in view.
    unordered_set<GridCoordinate> seenCoordinates;

    // First we have to go through and initialize any parts of the map from storage that are necessary
    for (int i = max(centerX - radius - 1, -1 * _xRange); i < min(centerX + radius + 1, _xRange); i++) {
        for (int j = max(centerY - radius - 1, -1 * _yRange); j < min(centerY + radius + 1, _yRange); j++) {
            for (int k = max(centerZ - radius - 1, -1 * _zRange); k < min(centerZ + radius + 1, _zRange); k++) {
                // Generate the chunk at this point in the map if it doesn't currently exist
                if (_chunkMap.find(glm::vec3(i, j, k)) == _chunkMap.end()) {
                    // auto* chunk = cio.readChunk(i, j, k);
                    _chunkMap.insert({ glm::vec3(i, j, k), unique_ptr<Chunk> (cio.readChunk(i, j, k))});

                }
            }
        }
    }

    for (int i = max(centerX - radius, -1 * _xRange); i < min(centerX + radius, _xRange); i++) {
        for (int j = max(centerY - radius, -1 * _yRange); j < min(centerY + radius, _yRange); j++) {
            for (int k = max(centerZ - radius, -1 * _zRange); k < min(centerZ + radius, _zRange); k++) {
                // If this is an active chunk coordinate then we just use its existing buffer
                if (activeChunkCoordinates.find(GridCoordinate(i, j, k)) != activeChunkCoordinates.end()) 
                {
                    auto otherBuffer = _chunkMap.at(GridCoordinate(i, j, k))->getBuffer();
                    buffer.insert(buffer.end(), otherBuffer.begin(), otherBuffer.end());

                    seenCoordinates.insert(GridCoordinate(i, j, k));
                }
                // Otherwise, we have to create the buffer.
                else 
                {
                    auto* chunk = _chunkMap.at(GridCoordinate(i, j, k)).get();
                    auto newBuffer = createBuffer(chunk, i, j, k);

                    chunk->setBuffer(newBuffer);
                    buffer.insert(buffer.end(), newBuffer.begin(), newBuffer.end());
                    activeChunkCoordinates.insert(GridCoordinate(i, j, k));

                    seenCoordinates.insert(GridCoordinate(i, j, k));
                }
            }
        }
    }

    // Go through and free memory associated with chunks in the map that are not currently visible
    auto mapIt = _chunkMap.begin();

    while (mapIt != _chunkMap.end())
    {
        if (seenCoordinates.find(mapIt->first) == seenCoordinates.end()) {
            activeChunkCoordinates.erase(mapIt->first);
            mapIt = _chunkMap.erase(mapIt++);
        }
        else
        {
            mapIt++;
        }
    }

    return buffer;
}


vector<float> ChunkManager::getBuffersCurrentlyInMap()
{
    vector<float> buffer;

    for (auto& chunk : _chunkMap)
    {
        auto otherBuffer = chunk.second.get()->getBuffer();
        buffer.insert(buffer.end(), otherBuffer.begin(), otherBuffer.end());
    }

    return buffer;
}


GridCoordinate ChunkManager::getChunkPlayerIsIn(float x, float y, float z)
{
	auto xChunkValue = floor(x / CHUNK_SIZE);
	auto yChunkValue = floor(y / CHUNK_SIZE);
	auto zChunkValue = floor(z / CHUNK_SIZE);

	return GridCoordinate(xChunkValue, yChunkValue, zChunkValue);
}


Chunk* ChunkManager::accessChunkAtPosition(float x, float y, float z)
{
	int xChunkValue = int(floor(x / CHUNK_SIZE));
	int yChunkValue = int(floor(y / CHUNK_SIZE));
	int zChunkValue = int(floor(z / CHUNK_SIZE));

	return _chunkMap.at(GridCoordinate(xChunkValue, yChunkValue, zChunkValue)).get();
}


void ChunkManager::setCubeAtPosition(float x, float y, float z, bool value)
{
	Chunk* chunkToModify = accessChunkAtPosition(x, y, z);

	int xPosition = int(x - floor(x / CHUNK_SIZE) * CHUNK_SIZE);
	int yPosition = int(y - floor(y / CHUNK_SIZE) * CHUNK_SIZE);
	int zPosition = int(z - floor(z / CHUNK_SIZE) * CHUNK_SIZE);

	chunkToModify->_cubes[xPosition][yPosition][zPosition].setActive(value);
}


Cube& ChunkManager::accessCubeAtPosition(float x, float y, float z)
{
	Chunk* chunkToAccess = accessChunkAtPosition(x, y, z);

	int xPosition = int(x - floor(x / CHUNK_SIZE) * CHUNK_SIZE);
	int yPosition = int(y - floor(y / CHUNK_SIZE) * CHUNK_SIZE);
	int zPosition = int(z - floor(z / CHUNK_SIZE) * CHUNK_SIZE);

	return chunkToAccess->_cubes[xPosition][yPosition][zPosition];
}


bool ChunkManager::testIfPieceIsVisible(int x, int y, int z)
{
    if (inBounds(x, y, z)) {
        auto aboveCube = accessCubeAtPosition(float(x), float(y + 1), float(z)).getActive();
        auto belowCube = accessCubeAtPosition(float(x), float(y - 1), float(z)).getActive();
        
        auto forwardCube = accessCubeAtPosition(float(x + 1), float(y), float(z)).getActive();
        auto behindCube = accessCubeAtPosition(float(x - 1), float(y), float(z)).getActive();

        auto rightCube = accessCubeAtPosition(float(x), float(y), float(z + 1)).getActive();
        auto leftCube = accessCubeAtPosition(float(x), float(y), float(z - 1)).getActive();

        return !(aboveCube && belowCube && forwardCube && behindCube
            && rightCube && leftCube);
    }

    return false;
}


bool ChunkManager::testIfPieceIsVisibleAtIntegerPosition(Chunk* chunk, int x, int y, int z)
{
    auto aboveCube = accessCubeAtIntegerPosition(chunk, x, y + 1, z).getActive();
    auto belowCube = accessCubeAtIntegerPosition(chunk, x, y - 1, z).getActive();

    auto forwardCube = accessCubeAtIntegerPosition(chunk, x + 1, y, z).getActive();
    auto behindCube = accessCubeAtIntegerPosition(chunk, x - 1, y, z).getActive();

    auto rightCube = accessCubeAtIntegerPosition(chunk, x, y, z + 1).getActive();
    auto leftCube = accessCubeAtIntegerPosition(chunk, x, y, z - 1).getActive();

    return !(aboveCube && belowCube && forwardCube && behindCube
        && rightCube && leftCube);
}


Cube& ChunkManager::accessCubeAtIntegerPosition(Chunk* chunk, int x, int y, int z)
{
    return chunk->_cubes[x][y][z];
}


void ChunkManager::setCubeAtIntegerPosition(Chunk* chunk, int x, int y, int z)
{
    chunk->_cubes[x][y][z].setActive(true);
}


vector<float> ChunkManager::createBuffer(Chunk* chunk, int chunkX, int chunkY, int chunkZ) 
{
    int x = CHUNK_SIZE * chunkX;
    int y = CHUNK_SIZE * chunkY;
    int z = CHUNK_SIZE * chunkZ;

    vector<float> verticeBuffer;

    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_SIZE; k++) {

                int xPos = x + i;
                int yPos = y + j;
                int zPos = z + k;

                auto& val = accessCubeAtIntegerPosition(chunk, i, j, k);

                if (!val.getActive()) {
                    continue;
                }

                auto loopHelper = [&verticeBuffer, xPos, yPos, zPos](auto start, auto end) {
                    for (int i = start; i < end; i++) {
                        float xVert = vertices[i * 3 + 0];
                        float yVert = vertices[i * 3 + 1];
                        float zVert = vertices[i * 3 + 2];

                        verticeBuffer.push_back(xPos + xVert);
                        verticeBuffer.push_back(yPos + yVert);
                        verticeBuffer.push_back(zPos + zVert);

                        verticeBuffer.push_back((float)rand() / RAND_MAX);
                    }
                };

                // Test if we can utilize just this chunk by itself to gather information about surrounding blocks
                bool boundsCondition = i > 1 && i < CHUNK_SIZE - 1 && j > 1 && j < CHUNK_SIZE - 1 && k > 1 && k < CHUNK_SIZE - 1;

                if (boundsCondition) {
                    if (testIfPieceIsVisibleAtIntegerPosition(chunk, i, j, k)) {
                        // Front face 
                        if (!accessCubeAtIntegerPosition(chunk, i, j, k - 1).getActive()) {
                            loopHelper(0, 6);
                        }

                        // Back face
                        if (!accessCubeAtIntegerPosition(chunk, i, j, k + 1).getActive()) {
                            loopHelper(6, 12);
                        }

                        // Left face 
                        if (!accessCubeAtIntegerPosition(chunk, i - 1, j, k).getActive()) {
                            loopHelper(12, 18);

                        }

                        // Right face
                        if (!accessCubeAtIntegerPosition(chunk, i + 1, j, k).getActive()) {
                            loopHelper(18, 24);
                        }

                        // Bottom face
                        if (!accessCubeAtIntegerPosition(chunk, i, j - 1, k).getActive()) {
                            loopHelper(24, 30);
                        }

                        // Top face 
                        if (!accessCubeAtIntegerPosition(chunk, i, j + 1, k).getActive()) {
                            loopHelper(30, 36);
                        }

                    }
                }
                // If this is the case then we have to take into consideration blocks on the border and we can't simply just
                // use the one primary chunk.
                else 
                {
                    if (testIfPieceIsVisible(xPos, yPos, zPos)) {

                        // Front face
                        if (!accessCubeAtPosition(float(xPos), float(yPos), float(zPos - 1)).getActive()) {
                            loopHelper(0, 6);
                        }

                        // Back face
                        if (!accessCubeAtPosition(float(xPos), float(yPos), float(zPos + 1)).getActive()) {

                            loopHelper(6, 12);
                        }

                        // Leftward face
                        if (!accessCubeAtPosition(float(xPos - 1), float(yPos), float(zPos)).getActive()) {
                            loopHelper(12, 18);
                        }

                        // Rightward face
                        if (!accessCubeAtPosition(float(xPos + 1), float(yPos), float(zPos)).getActive()) {
                            loopHelper(18, 24);
                        }

                        // Bottom face
                        if (!accessCubeAtPosition(float(xPos), float(yPos - 1), float(zPos)).getActive()) {
                            loopHelper(24, 30);
                        }

                        // Top face
                        if (!accessCubeAtPosition(float(xPos), float(yPos + 1), float(zPos)).getActive()) {
                            loopHelper(30, 36);
                        }

                    }
                }
                
            }
        }
    }

    return verticeBuffer;
}