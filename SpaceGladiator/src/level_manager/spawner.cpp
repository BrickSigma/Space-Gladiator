#include "level_manager/spawner.hpp"
#include "scenes/game_scene.hpp"

#include "entities/asteroid.hpp"
#include "entities/diagonal_flyer.hpp"

#include <format>
#include <cstring>

SpawnerType spawnerStringToType(const char *str)
{
    if (!strcmp(str, "ASTEROID"))
        return SpawnerType::ASTEROID;
    else if (!(strcmp(str, "DIAGONAL_FLYER")))
        return SpawnerType::DIAGONAL_FLYER;
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

static SpawnerCB spawner_cb_table[]{
    spawnAsteroid,
    spawnDiagonalFlyer};

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