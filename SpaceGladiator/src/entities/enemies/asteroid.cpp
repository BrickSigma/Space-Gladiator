#include "entities/enemies/asteroid.hpp"

#include "entities/player/player_bullet.hpp"
#include "scenes/game_scene.hpp"
#include "utils/texture_manager.hpp"
#include "entities/explosion.hpp"

Asteroid::Asteroid(AsteroidSize size_, Vector2 pos_, int direction)
    : Entity(EntityType::ENEMY, pos_, static_cast<int>(size_), static_cast<int>(size_)),
      size(size_)
{
    TextureManager &texture_manager = TextureManager::getInstance();

    float direction_f = static_cast<float>(direction);
    switch (size_)
    {
    case AsteroidSize::SMALL:
        velocity = {-1, direction_f * 1.0f};
        update_x_time = 1;
        update_y_time = 2;
        texture = texture_manager.asteroid_small;
        break;
    case AsteroidSize::MEDIUM:
        velocity = {-1, direction_f * 1.0f};
        update_x_time = 1;
        update_y_time = 3;
        texture = texture_manager.asteroid_medium;
        break;
    case AsteroidSize::LARGE:
        velocity = {-1, direction_f * 1.0f};
        update_x_time = 2;
        update_y_time = 4;
        texture = texture_manager.asteroid_large;
        break;
    }
}

Asteroid::~Asteroid() {}

void Asteroid::update(GameScene *scene)
{
    (void)scene;

    frame_counter++;
    if (frame_counter % update_x_time == 0)
        pos.x += velocity.x;

    if (frame_counter % update_y_time == 0)
        pos.y += velocity.y;

    // Check if the asteroid is off screen and automatically mark it for deletion
    if (isOffScreen() && pos.x < SPACE_CANVAS_WIDTH)
        delete_entity = true;
}

void Asteroid::draw()
{
    DrawTexture(texture, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
}

void Asteroid::handleCollision(GameScene *scene, Entity *entity)
{
    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (!checkCollision(*entity))
        return;

    // The player bullet and asteroid collided, so let's spawn in two more smaller asteroids
    delete_entity = true;
    entity->delete_entity = true;

    scene->setCameraShake(CameraShake(2, 30));

    // Spawn an explosion
    scene->spawnEntity(new Explosion(2, {pos.x + static_cast<float>(width) / 2, pos.y + static_cast<float>(height) / 2}));

    // Create two smaller asteroids
    AsteroidSize next_size{AsteroidSize::MEDIUM};
    switch (size)
    {
    case AsteroidSize::LARGE:
        next_size = AsteroidSize::MEDIUM;
        break;
    case AsteroidSize::MEDIUM:
        next_size = AsteroidSize::SMALL;
        break;
    default:
        return;
    }

    scene->spawnEntity(new Asteroid(next_size, {pos.x, pos.y + static_cast<float>(height) / 2 - static_cast<float>(next_size) / 2 + static_cast<float>(next_size) / 2}, 1));
    scene->spawnEntity(new Asteroid(next_size, {pos.x, pos.y + static_cast<float>(height) / 2 - static_cast<float>(next_size) / 2 - static_cast<float>(next_size) / 2}, -1));
}