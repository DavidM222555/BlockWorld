
#include "RunLengthEncoding.h"

string getRunLengthEncoding(Chunk* chunk)
{
	string rleString;
	char prevChar = '0'; // A character that won't be mapped
	int currentCount = 1;

	for (int y = 0; y < CHUNK_SIZE; y++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int x = 0; x < CHUNK_SIZE; x++) {
				// Handle the start character which doesn't have a preceeding character
				if (prevChar == '0') {
					if (chunk->_cubes[x][y][z].getActive() == false) {
						prevChar = 'z';
					}
					else
					{
						prevChar = static_cast<char>(chunk->_cubes[x][y][z].getTexture());
					}

					continue;
				}

				// Cast the enum to its character equivalent which is defined in the Texture struct
				auto textureChar = static_cast<char>(chunk->_cubes[x][y][z].getTexture());

				// If the chunk is just air we utilize the character 'z' instead of the texture char
				if (chunk->_cubes[x][y][z].getActive() == false) {
					textureChar = 'z';
				}

				if (textureChar == prevChar) {
					currentCount++;
				}
				else {
					rleString += std::to_string(currentCount) + prevChar;
					currentCount = 1;
				}

				prevChar = textureChar;
			}
		}
	}

	// Handle the case where the last run of characters didn't end in the main loop
	if (currentCount != 1) {
		rleString += std::to_string(currentCount) + prevChar;
	}

	return rleString;
}