#include "level_manager/spawner.hpp"
#include "entities/asteroid.hpp"
#include "scenes/game_scene.hpp"

#include <format>
#include <cstring>

SpawnerType spawnerStringToType(const char *str)
{
    if (!strcmp(str, "ASTEROID"))
        return SpawnerType::ASTEROID;
    else
        throw std::runtime_error(std::format("Invalid spawner type string passed: {}", str));
}

static void spawnAsteroid(SpawnerData data, GameScene *scene);

static SpawnerCB spawner_cb_table[] {
    spawnAsteroid,
};

void spawnerDispatch(SpawnerData data, GameScene *scene) {
    spawner_cb_table[static_cast<int>(data.type)](data, scene);
}

static void spawnAsteroid(SpawnerData data, GameScene *scene)
{
    int x_pos = (data.x_tile_pos * TILE_SIZE) - scene->scroll_distance;
    int y_pos = data.y_tile_pos * TILE_SIZE;
    AsteroidSize size;

    switch (data.flags) {
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

    int direction = (y_pos > SPACE_CANVAS_HEIGHT/2) ? -1 : 1;
    scene->spawnEntity(new Asteroid(size, {static_cast<float>(x_pos), static_cast<float>(y_pos)}, direction));
}