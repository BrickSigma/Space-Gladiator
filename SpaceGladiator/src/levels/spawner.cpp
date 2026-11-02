#include "levels/spawner.hpp"
#include "entities/asteroid.hpp"
#include "scenes/game_scene.hpp"

void spawnAsteroid(SpawnerData data, GameScene *scene)
{
    AsteroidSize size = static_cast<AsteroidSize>(data.flags);
    float vel_y = (static_cast<float>(data.height) - ((float)SPACE_CANVAS_HEIGHT / 2.0f)) / ((float)SPACE_CANVAS_HEIGHT / 2.0f);
    scene->spawnEntity(new Asteroid(size, {static_cast<float>(SPACE_CANVAS_WIDTH), static_cast<float>(data.height)}, -vel_y*0.5f));
}