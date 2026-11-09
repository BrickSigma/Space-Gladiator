#include "level_manager/spawner.hpp"
#include "scenes/game_scene.hpp"

#include "entities/asteroid.hpp"
#include "entities/diagonal_flyer.hpp"
#include "entities/sine_wave_flyer.hpp"
#include "entities/spinning_laser.hpp"

#include <format>
#include <cstring>

SpawnerType spawnerStringToType(const char *str)
{
    if (!strcmp(str, "ASTEROID"))
        return SpawnerType::ASTEROID;
    else if (!(strcmp(str, "DIAGONAL_FLYER")))
        return SpawnerType::DIAGONAL_FLYER;
    else if (!strcmp(str, "SINE_WAVE_FLYER"))
        return SpawnerType::SINE_WAVE_FLYER;
    else if (!strcmp(str, "SPINNING_LASER"))
        return SpawnerType::SPINNING_LASER;
    else
        throw std::runtime_error(std::format("Invalid spawner type string passed: {}", str));
}

// Map the spawner's position to a vector describing the entity's position
static Vector2 spawnPositionToVector(const SpawnerData &data, int scroll_distance)
{
    int x_pos = (data.x_tile_pos * TILE_SIZE) - scroll_distance;
    int y_pos = data.y_tile_pos * TILE_SIZE;
    return {static_cast<float>(x_pos), static_cast<float>(y_pos)};
}

static void spawnAsteroid(const SpawnerData &data, GameScene *scene);
static void spawnDiagonalFlyer(const SpawnerData &data, GameScene *scene);
static void spawnSineWaveFlyer(const SpawnerData &data, GameScene *scene);
static void spawnSpinningLaser(const SpawnerData &data, GameScene *scene);

static SpawnerCB spawner_cb_table[]{
    spawnAsteroid,
    spawnDiagonalFlyer,
    spawnSineWaveFlyer,
    spawnSpinningLaser};

void spawnerDispatch(const SpawnerData &data, GameScene *scene)
{
    spawner_cb_table[static_cast<int>(data.type)](data, scene);
}

/**
 * Spawn an asteroid. The `data` struct flags indicates the size of the asteroid to spawn.
 */
static void spawnAsteroid(const SpawnerData &data, GameScene *scene)
{
    Vector2 pos = spawnPositionToVector(data, scene->scroll_distance);
    AsteroidSize size;

    switch (data.flags)
    {
    case 0:
        size = AsteroidSize::SMALL;
        break;
    case 1:
        size = AsteroidSize::MEDIUM;
        break;
    case 2:
        size = AsteroidSize::LARGE;
        break;
    default:
        throw std::runtime_error(std::format("Invalid asteroid flag passed in: {}", data.flags));
    }

    int direction = (pos.y > SPACE_CANVAS_HEIGHT / 2) ? -1 : 1;
    scene->spawnEntity(new Asteroid(size, pos, direction));
}

// Spawn a diagonal flyer
static void spawnDiagonalFlyer(const SpawnerData &data, GameScene *scene)
{
    Vector2 pos = spawnPositionToVector(data, scene->scroll_distance);
    bool special = data.flags != 0;
    scene->spawnEntity(new DiagonalFlyer(pos, special));
}

// Spawn a sine wave flyer
static void spawnSineWaveFlyer(const SpawnerData &data, GameScene *scene)
{
    Vector2 pos = spawnPositionToVector(data, scene->scroll_distance);
    bool special = data.flags != 0;
    scene->spawnEntity(new SineWaveFlyer(pos, special));
}

// Spawn a spinning laser. The `data.flags` indicates the spinning type
static void spawnSpinningLaser(const SpawnerData &data, GameScene *scene)
{
    Vector2 pos = spawnPositionToVector(data, scene->scroll_distance);

    SpinningLaserType type{SpinningLaserType::LEFT};

    int type_flag = data.flags >> 4;  // The type of spawner is stored in the upper 4 bits of the flag
    int no_lasers = data.flags & 0xf;  // The number of lasers the entity has is in the lower 4 bits

    switch (type_flag)
    {
    case 0:
        type = SpinningLaserType::LEFT;
        break;
    case 1:
        type = SpinningLaserType::RIGHT;
        break;
    case 2:
        type = SpinningLaserType::ALTERNATE_LEFT;
        break;
    case 3:
        type = SpinningLaserType::ALTERNATE_RIGHT;
        break;
    default:
        break;
    }

    scene->spawnEntity(new SpinningLaser(pos, no_lasers, type));
}