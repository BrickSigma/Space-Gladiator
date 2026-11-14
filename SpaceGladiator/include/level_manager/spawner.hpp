#ifndef SPAWNER_HPP
#define SPAWNER_HPP

class GameScene;

// Spawner type, used for the spawner level data
enum class SpawnerType
{
    ASTEROID,
    DIAGONAL_FLYER,
    SINE_WAVE_FLYER,
    SPINNING_LASER,
    MOVING_LASER,
    FLYER_SPAWNER,
    CANON,
    GROUND_SHOOTER
};

// Convert a string to a spawner type, or throw an error on failure
SpawnerType spawnerStringToType(const char *str);

// Data struct used for representing the spawner callback function to call
struct SpawnerData
{
    int x_tile_pos;   // X position of entity in terms of tiles
    int y_tile_pos;   // Y position of entity in terms of tiles
    SpawnerType type; // The type of entity we want to spawn
    int flags;        // Extra flags carrying information for the spawner
};

// Callback function type for spawning an entity
typedef void (*SpawnerCB)(const SpawnerData &data, GameScene *scene);

/**
 * Used to call the correct spawner function based on the data type
 */
void spawnerDispatch(const SpawnerData &data, GameScene *scene);

#endif // SPAWNER_HPP