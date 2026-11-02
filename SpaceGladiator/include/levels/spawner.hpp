#ifndef SPAWNER_HPP
#define SPAWNER_HPP

class GameScene;

typedef struct SpawnerData SpawnerData;

typedef void (*SpawnerCB)(SpawnerData data, GameScene *scene);

// Data struct used for representing the spawner callback function to call
struct SpawnerData
{
    unsigned long location; // The point at which to spawn the entity, based on the scene's scroll distance
    SpawnerCB cb;           // Callback function for the spawner
    int height;             // Height of the entity to spawn
    int flags;              // Extra flags carrying information for the spawner
};

/**
 * Used to spawn an asteroid.
 *
 * @param data.height indicated the height at which to spawn the asteroid
 * @param flags is set to match the asteroid size enum
 */
void spawnAsteroid(SpawnerData data, GameScene *scene);

#endif // SPAWNER_HPP