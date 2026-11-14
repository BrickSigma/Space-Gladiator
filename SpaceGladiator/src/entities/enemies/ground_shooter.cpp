#include "entities/enemies/ground_shooter.hpp"

#include "utils/texture_manager.hpp"
#include "utils/camera_shake.hpp"
#include "scenes/game_scene.hpp"
#include "entities/enemies/enemy_projectile.hpp"
#include "entities/explosion.hpp"
#include "utils/math.hpp"

#include <cmath>

GroundShooter::GroundShooter(Vector2 pos_, bool facing_up_, bool special_)
    : Entity(EntityType::ENEMY, pos_, 16, 16), facing_up(facing_up_), special(special_)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    if (special)
        sprites = texture_manager.special_ground_shooter;
    else
        sprites = texture_manager.regular_ground_shooter;
}

void GroundShooter::update(GameScene *scene)
{
    frame_counter++;
    if (frame_counter % 2 == 0)
        pos.x--;

    // Get the angle towards the player
    Vector2 player_pos = scene->player.getCenter();
    Vector2 shooter_center = getCenter();
    float dx = player_pos.x - shooter_center.x;
    float dy = player_pos.y - shooter_center.y;
    target_angle = atan2f(dy, dx); // This will return an angle between -180 to 180 degrees. We don't want to adjust it as the signage of the angle is actually quite useful

    if (frame_counter % fire_time == 0)
    {
        // Shoot if the player is in reach of the shooter
        if ((target_angle <= 0 && facing_up) || (target_angle >= 0 && !facing_up))
        {
            // Move the bullet a bit outwards so it doesn't spawn on top of the shooter
            EnemyProjectile *bullet = new EnemyProjectile(shooter_center, target_angle, SHOOTER_SPEED);
            bullet->update(scene);
            bullet->update(scene);
            bullet->update(scene);
            bullet->update(scene);
            bullet->update(scene);
            scene->spawnEntity(bullet);
        }
    }
}

void GroundShooter::draw()
{
    float deg = fabsf(rad2deg(target_angle));
    float sprite_index = 0.0f;
    float flip_x = 1.0f;
    float flip_y = facing_up ? 1.0f : -1.0f;
    if (deg < 30)
        sprite_index = 2.0f;
    else if (deg < 60)
        sprite_index = 0.0f;
    else if (deg < 90)
        sprite_index = 1.0f;
    else if (deg < 120)
        sprite_index = 1.0f;
    else if (deg < 160)
        sprite_index = 0.0f;
    else
        sprite_index = 2.0f;

    if (deg >= 90)
        flip_x = -1.0f;

    DrawTextureRec(sprites, {16.0f * sprite_index, 0, 16.0f * flip_x, 16.0f * flip_y}, pos, WHITE);
}

void GroundShooter::handleCollision(GameScene *scene, Entity *entity)
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