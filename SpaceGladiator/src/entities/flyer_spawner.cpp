#include "entities/flyer_spawner.hpp"

#include "scenes/game_scene.hpp"
#include "utils/texture_manager.hpp"
#include "entities/explosion.hpp"

Flyer::Flyer(Vector2 pos, float target_y) : Entity(EntityType::ENEMY, pos, 16, 16), target_y(target_y)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    animation = new Animation(texture_manager.flyer, 16, 6, true);
}

Flyer::~Flyer()
{
    delete animation;
}

void Flyer::draw()
{
    animation->draw(pos);
}

void Flyer::handleCollision(GameScene *scene, Entity *entity)
{
    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (!checkCollision(*entity))
        return;

    // The flyer was hit by a player bullet, so let's delete it
    delete_entity = true;
    entity->delete_entity = true;

    scene->setCameraShake(CameraShake(2, 30));

    // Spawn an explosion
    scene->spawnEntity(new Explosion(1, {pos.x + 8, pos.y + 8}));
}

void Flyer::update(GameScene *scene)
{
    frame_counter++;
    switch (state)
    {
    case FlyerState::MOVING_UP:
        if (frame_counter % 2 == 0)
        {
            pos.x--;
        }
        pos.y -= 2;
        if (pos.y <= target_y)
            state = FlyerState::MOVING_LEFT;
        break;

    case FlyerState::MOVING_LEFT:
        pos.x -= 2;
        break;

    default:
        break;
    }
}

FlyerSpawner::FlyerSpawner(Vector2 pos, bool special) : Entity(EntityType::ENEMY, pos, FlyerSpawner::WIDTH, FlyerSpawner::HEIGHT), special(special)
{
    TextureManager &texture_manager = TextureManager::getInstance();

    if (special)
        texture = texture_manager.special_flyer_spawner;
    else
        texture = texture_manager.regular_flyer_spawner;
}

void FlyerSpawner::update(GameScene *scene)
{
    frame_counter++;

    if (frame_counter % 2 == 0)
        pos.x--;

    switch (state)
    {
    case FlyerSpawnerState::WAITING:
        if (frame_counter >= wait_duration)
        {
            state = FlyerSpawnerState::SPAWNING;
            frame_counter = 0;
        }
        break;

    case FlyerSpawnerState::SPAWNING:
        if (frame_counter % spawn_delay == 0)
        {
            // Get the player's current y position
            float target_y = scene->player.pos.y;
            Vector2 flyer_pos{pos.x + 24, pos.y - 24};
            scene->spawnEntity(new Flyer(pos, target_y));
            flyers_spawned++;
            if (flyers_spawned >= max_flyers)
            {
                state = FlyerSpawnerState::WAITING;
                flyers_spawned = 0;
                frame_counter = 0;
            }
        }

    default:
        break;
    }
}

void FlyerSpawner::draw()
{
    DrawTexture(texture, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
}

void FlyerSpawner::handleCollision(GameScene *scene, Entity *entity)
{
    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (!checkCollision(*entity))
        return;

    // The flyer spawner was hit by a player bullet, so let's delete the bullet first
    entity->delete_entity = true;

    // Let's reduce the health
    health--;
    if (health > 0)
        return;

    // Delete the flyer spawner if all health is gone
    delete_entity = true;

    // Shake the camera a bit
    scene->setCameraShake(CameraShake(2, 30));

    // Spawn an explosion
    scene->spawnEntity(new Explosion(1, {pos.x + WIDTH / 2, pos.y + HEIGHT / 2}));
}