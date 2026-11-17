#include "entities/enemies/blockade_ball.hpp"

#include "utils/texture_manager.hpp"
#include "scenes/game_scene.hpp"
#include "entities/player/player_projectile.hpp"

BlockadeBall::BlockadeBall(Vector2 pos_) : Entity(EntityType::ENEMY, pos_, 6, 5)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    texture = texture_manager.blockade_ball;

    is_tile = true;
}

void BlockadeBall::update(GameScene *scene)
{
    (void)scene;
    frame_counter++;
    if (frame_counter % 2 == 0)
        pos.x--;

    if (isOffScreen() && pos.x < SPACE_CANVAS_WIDTH)
        delete_entity = true;
}

void BlockadeBall::draw()
{
    DrawTexture(texture, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
}

void BlockadeBall::handleCollision(GameScene *scene, Entity *entity)
{
    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (!checkCollision(*entity))
        return;

    // The player bullet and asteroid collided, so let's spawn in two more smaller asteroids
    delete_entity = true;

    // Also upadte the score for the player
    scene->addScore(score);

    PlayerProjectile *projectile = static_cast<PlayerProjectile *>(entity);
    projectile->handleDestruction(); // Call the projectile's own destruction handler, as the laser may not get destroyed
}