#pragma once

#include <string>
using std::string;

#include "Chunk.h"

std::array<std::array<std::array<Cube, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> decodeRunLengthEncoding(string encodedChunk);