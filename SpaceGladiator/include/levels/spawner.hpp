#ifndef SPAWNER_HPP
#define SPAWNER_HPP

class GameScene;

// Spawner type, used for the spawner level data
enum class SpawnerType
{
    ASTEROID
};

// Data struct used for representing the spawner callback function to call
struct SpawnerData
{
    int x_tile_pos;   // X position of entity in terms of tiles
    int y_tile_pos;   // Y position of entity in terms of tiles
    SpawnerType type; // The type of entity we want to spawn
    int flags;        // Extra flags carrying information for the spawner
};

// Callback function type for spawning an entity
typedef void (*SpawnerCB)(SpawnerData data, GameScene *scene);

/**
 * Used to call the correct spawner function based on the data type
 */
void spawnerDispatch(SpawnerData data, GameScene *scene);

#endif // SPAWNER_HPP