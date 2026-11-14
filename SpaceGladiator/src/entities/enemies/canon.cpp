#include "entities/enemies/canon.hpp"

#include "utils/texture_manager.hpp"
#include "scenes/game_scene.hpp"
#include "entities/enemies/enemy_projectile.hpp"
#include "utils/math.hpp"

Canon::Canon(Vector2 pos_, bool facing_up_) : Entity(EntityType::ENEMY, pos_, 16, 12), facing_up(facing_up_)
{
    if (facing_up_)
        pos.y += 4.0f;

    TextureManager &texture_manager = TextureManager::getInstance();
    texture = texture_manager.canon;
}

void Canon::update(GameScene *scene)
{
    frame_counter++;
    if (frame_counter % 2 == 0)
        pos.x--;

    if (frame_counter % fire_time == 0)
    {
        Vector2 bullet_center_pos{pos.x + 5, (facing_up ? pos.y - 6 : pos.y + static_cast<float>(height))};
        Vector2 bullet_left_pos{bullet_center_pos.x - 4, bullet_center_pos.y + (facing_up ? -2 : 2)};
        Vector2 bullet_right_pos{bullet_center_pos.x + 4, bullet_center_pos.y + (facing_up ? -2 : 2)};

        float bullet_center_angle{facing_up ? 270.0f : 90.0f};
        float bullet_left_angle{facing_up ? 225.0f : 135.0f};
        float bullet_right_angle{facing_up ? 315.0f : 45.0f};

        scene->spawnEntity(new EnemyProjectile(bullet_center_pos, deg2rad(bullet_center_angle), CANON_SPEED));
        scene->spawnEntity(new EnemyProjectile(bullet_left_pos, deg2rad(bullet_left_angle), CANON_SPEED));
        scene->spawnEntity(new EnemyProjectile(bullet_right_pos, deg2rad(bullet_right_angle), CANON_SPEED));
    }
}

void Canon::draw()
{
    DrawTextureRec(texture, {0.0f, 0.0f, 16.0f, (facing_up ? 12.0f : -12.0f)}, pos, WHITE);
}

void Canon::handleCollision(GameScene *scene, Entity *entity)
{
    (void)scene;
    if (!(entity->type == EntityType::PLAYER_PROJECTILE || entity->type == EntityType::ENEMY_PROJECTILE))
        return;

    // Can't use the regular `checkCollision` function as the player uses it
    if (!CheckCollisionRecs(getRect(), entity->getRect()))
        return;

    // Delete just the bullet
    entity->delete_entity = true;
}