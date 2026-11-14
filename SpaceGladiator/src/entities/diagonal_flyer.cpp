#include "entities/diagonal_flyer.hpp"

#include "scenes/game_scene.hpp"
#include "utils/texture_manager.hpp"
#include "entities/explosion.hpp"

#include <cmath>

DiagonalFlyer::DiagonalFlyer(Vector2 pos_, bool special_) : Entity(EntityType::ENEMY, pos_, 16, 16), special(special_)
{
    TextureManager &texture_manager = TextureManager::getInstance();

    if (special_)
        textures = texture_manager.special_diagonal_flyer;
    else
        textures = texture_manager.regular_diagonal_flyer;
}

void DiagonalFlyer::update(GameScene *scene)
{
    float player_y_pos = floorf(scene->player.pos.y);
    float flyer_y_pos = floorf(pos.y);

    if ((flyer_y_pos + static_cast<float>(height)) <= player_y_pos)
    {
        state = DiagonalFlyerState::MOVING_DOWN;
        update_x_time = 1;
        update_y_time = 3;
        velocity = {-1, 1};
    }
    else if (flyer_y_pos >= (player_y_pos + static_cast<float>(scene->player.height)))
    {
        state = DiagonalFlyerState::MOVING_UP;
        update_x_time = 1;
        update_y_time = 3;
        velocity = {-1, -1};
    }
    else
    {
        state = DiagonalFlyerState::MOVING_STRAIGHT;
        update_x_time = 2;
        velocity = {-3, 0};
    }

    frame_counter++;
    if (frame_counter % update_x_time == 0)
        pos.x += velocity.x;

    if (frame_counter % update_y_time == 0)
        pos.y += velocity.y;

    if (isOffScreen() && pos.x < SPACE_CANVAS_WIDTH)
        delete_entity = true;
}

void DiagonalFlyer::draw()
{
    DrawTextureRec(textures, {static_cast<float>(state * 16), 0.0f, 16.0f, 16.0f}, pos, WHITE);
}

void DiagonalFlyer::handleCollision(GameScene *scene, Entity *entity)
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