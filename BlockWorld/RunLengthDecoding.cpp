
#include "RunLengthDecoding.h"

const char AIR_CHARACTER = 'z';

std::array<std::array<std::array<Cube, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> decodeRunLengthEncoding(string encodedChunk)
{
	std::array<std::array<std::array<Cube, CHUNK_SIZE>, CHUNK_SIZE>, CHUNK_SIZE> _cubes;

	// Tracks where we are in the string
	int charCounter = 0;

	// Used for determining how far we are to finishing creating the chunk
	int chunkProgressCounter = 0;

	// Used for tracking the number of the current character we have to get
	int numberOfCharLeft = 0;

	char currentChar = 'z';

	while (chunkProgressCounter < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE) {
		// Go and find the new char and the number of that char we have to get
		if (numberOfCharLeft == 0) {
			string numberString = "";

			// Find the number of the character we have to get
			while (isdigit(encodedChunk[charCounter])) {
				numberString += encodedChunk[charCounter];
				charCounter++;
			}

			numberOfCharLeft = stoi(numberString);
			currentChar = encodedChunk[charCounter];
		}

		while (numberOfCharLeft != 0) {
			int yCoordinate = int(floor(chunkProgressCounter / (CHUNK_SIZE * CHUNK_SIZE)));
			int zCoordinate = int(chunkProgressCounter / CHUNK_SIZE) % 32;
			int xCoordinate = chunkProgressCounter % 32;

			// Check if the current character is air or not, if it is then we set the cube to inactive and don't worry about
			// the texture.
			if (currentChar == AIR_CHARACTER) 
			{
				_cubes[xCoordinate][yCoordinate][zCoordinate] = Cube(false);
			}
			else 
			{
				// Otherwise convert the character to its associated Texture
				_cubes[xCoordinate][yCoordinate][zCoordinate] = Cube(true, static_cast<Texture>(currentChar));
			}

			numberOfCharLeft--;
			chunkProgressCounter++;
		}

		charCounter++;
	}


	return _cubes;
}

