#include "level_manager/spawner.hpp"
#include "entities/asteroid.hpp"
#include "scenes/game_scene.hpp"

#include <cstring>

SpawnerType spawnerStringToType(const char *str)
{
    if (!strcmp(str, "ASTEROID"))
        return SpawnerType::ASTEROID;
    else
        throw "Invalid spawner type string passed!";
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
    AsteroidSize size = static_cast<AsteroidSize>(data.flags);
    float vel_y = (static_cast<float>(y_pos) - ((float)SPACE_CANVAS_HEIGHT / 2.0f)) / ((float)SPACE_CANVAS_HEIGHT / 2.0f);
    scene->spawnEntity(new Asteroid(size, {static_cast<float>(x_pos), static_cast<float>(y_pos)}, -vel_y * 0.5f));
}