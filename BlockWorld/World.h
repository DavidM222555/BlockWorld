#pragma once

#include <glm/glm.hpp>

#include "ChunkManager.h"
#include "Player.h"

const float X_THRESHOLD = 0.85f;
const float Y_THRESHOLD = 0.50f;
const float Z_THRESHOLD = 0.25f;

const float COLLISION_INCREMENT = 0.2f;

const float RAYCAST_INCREMENT = 0.02f;
const float MAX_DISTANCE_FOR_RAYCAST = 3.0f;

class World
{
private:
	ChunkManager& _cm;
	Player _player;

public:
	World(ChunkManager& cm, Player player);

	inline glm::vec3 getPlayerPosition() { return _player.getPosition(); }
	inline Player& getPlayer() { return _player;  }

	// Used to test if the player should fall until they hit an object
	bool playerCollidesOnY();

	bool collidesOnX(glm::vec3 pos, glm::vec3 oldPos);
	bool collidesOnY(glm::vec3 pos, glm::vec3 oldPos);
	bool collidesOnZ(glm::vec3 pos, glm::vec3 oldPos);

	bool handleBlockDestruction(glm::vec3 startPosition, glm::vec3 dirVec);
};

