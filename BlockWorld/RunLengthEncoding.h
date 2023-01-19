#pragma once

#include <string>
using std::string;

#include <memory>
using std::weak_ptr;

#include "Chunk.h"

// Get the run length encoding for a given chunk. 
// Chunks are 3D arrays of cubes where the total size 
// is CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE.
// The way this algorithm works is we go from 0 to y, 0 to x, and 0 to z.
// The reason we go from 0 to y is because due to the way the terrain is generated we are more likely
// to see runs along the vertical axis.
string getRunLengthEncoding(Chunk* chunk);

