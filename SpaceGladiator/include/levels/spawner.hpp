#ifndef SPAWNER_HPP
#define SPAWNER_HPP

class GameScene;

typedef struct SpawnerData SpawnerData;

typedef void (*SpawnerCB)(SpawnerData data, GameScene *scene);

// Data struct used for representing the spawner callback function to call
struct SpawnerData
{
    int x_tile_pos; // X position of entity in terms of tiles
    int y_tile_pos;           // Y position of entity in terms of tiles
    SpawnerCB cb;             // Callback function for the spawner
    int flags;                // Extra flags carrying information for the spawner
};

/**
 * Used to spawn an asteroid.
 *
 * @param data.height indicated the height at which to spawn the asteroid
 * @param flags is set to match the asteroid size enum
 */
void spawnAsteroid(SpawnerData data, GameScene *scene);

#endif // SPAWNER_HPP