#include "entities/bosses/spiral_bullet.hpp"

#include <raymath.h>
#include <cmath>
#include "utils/math.hpp"

#include "utils/texture_manager.hpp"

SpiralBullet::SpiralBullet(Vector2 pos_, Vector2 origin_)
    : Projectile(EntityType::ENEMY_PROJECTILE, pos_, 6, 6, {0, 0}), origin(origin_)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    animation = new Animation(texture_manager.enemy_bullet, 6, 6, true);
}

SpiralBullet::~SpiralBullet()
{
    delete animation;
}

void SpiralBullet::update(GameScene *scene)
{
    (void)scene;

    // Update the distance of the bullet
    float distance = Vector2Distance(pos, origin);
    distance += vel;

    // Rotate the bullet around the origin
    Vector2 temp = Vector2Subtract(pos, origin);
    temp = Vector2Normalize(temp);
    temp = Vector2Rotate(temp, deg2rad(angular_vel));

    // Get the new position of the bullet
    temp = Vector2Scale(temp, distance);
    pos = Vector2Add(temp, origin);

    if (isOffScreen())
        delete_entity = true;
}

void SpiralBullet::draw()
{
    animation->draw(pos);
}