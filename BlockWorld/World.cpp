#include "World.h"

World::World(ChunkManager& cm, Player player) : _cm(cm)
{
    _player = player;
}

bool World::playerCollidesOnY()
{
    auto playerPosition = _player.getPosition();

    int roundedX = int(round(playerPosition.x));
    int flooredY = int(floor(playerPosition.y) - 1);
    int roundedZ = int(round(playerPosition.z));

    auto& blockNearestPlayer = _cm.accessCubeAtPosition(float(roundedX), float(flooredY), float(roundedZ));

    if (abs(playerPosition.y - 1 - flooredY) < Y_THRESHOLD && blockNearestPlayer.getActive()) {
        return true;
    }

    return false;
}


bool World::collidesOnX(glm::vec3 pos, glm::vec3 oldPos) 
{
    float increment = 0.0f;

    if (oldPos.x - pos.x < 0) {
        increment = 0.2f;
    }
    else {
        increment = -0.2f;
    }

    int roundedX = int(round(pos.x + increment));
    int roundedY = int(round(pos.y));
    int roundedZ = int(round(pos.z));

    auto& blockNearestPlayer = _cm.accessCubeAtPosition(float(roundedX), float(roundedY), float(roundedZ));

    if (blockNearestPlayer.getActive()) {
        return true;
    }

    return false;
}

bool World::collidesOnY(glm::vec3 pos, glm::vec3 oldPos)
{
    float increment = 0.0f;

    if (oldPos.y - pos.y < 0) {
        increment = 0.2f;
    }
    else {
        increment = -0.2f;
    }

    int roundedX = int(round(pos.x));
    int roundedY = int(round(pos.y + increment));
    int roundedZ = int(round(pos.z));

    auto& blockNearestPlayer = _cm.accessCubeAtPosition(float(roundedX), float(roundedY), float(roundedZ));

    if (blockNearestPlayer.getActive()) {
        return true;
    }

    return false;
}


bool World::collidesOnZ(glm::vec3 pos, glm::vec3 oldPos)
{
    float increment = 0.0f;

    if (oldPos.z - pos.z < 0) {
        increment = 0.2f;
    }
    else {
        increment = -0.2f;
    }

    int roundedX = int(round(pos.x));
    int roundedY = int(round(pos.y));
    int roundedZ = int(round(pos.z + increment));

    auto& blockNearestPlayer = _cm.accessCubeAtPosition(float(roundedX), float(roundedY), float(roundedZ));

    if (blockNearestPlayer.getActive()) {
        return true;
    }

    return false;
}

bool World::handleBlockDestruction(glm::vec3 startPosition, glm::vec3 dirVec)
{
    float numberOfIterations = MAX_DISTANCE_FOR_RAYCAST / RAYCAST_INCREMENT;

    for (float i = 0; i < numberOfIterations; i++)
    {
        // Move the current position in the direction of the dirVec
        glm::vec3 currentPos = startPosition +
            glm::vec3(dirVec.x * i * RAYCAST_INCREMENT, dirVec.y * i * RAYCAST_INCREMENT, dirVec.z * i * RAYCAST_INCREMENT);


        int x = int(round(currentPos.x));
        int y = int(round(currentPos.y));
        int z = int(round(currentPos.z));

        if (_cm.accessCubeAtPosition(x, y, z).getActive())
        {
            _cm.setCubeAtPosition(x, y, z, false);

            auto chunkPlayerIsAt = _cm.getChunkPlayerIsIn(x, y, z);
            _cm.updateChunkAndNeighbors(chunkPlayerIsAt.x, chunkPlayerIsAt.y, chunkPlayerIsAt.z);

            return true;
        }
    }
    
    return false;
}