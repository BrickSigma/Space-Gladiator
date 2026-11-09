#include "entities/spinning_laser.hpp"

#include "utils/texture_manager.hpp"
#include "scenes/game_scene.hpp"

#include <raymath.h>

LaserBall::LaserBall(Vector2 pos, Vector2 rotation_origin, int rotation_direction) : Entity(EntityType::ENEMY_PROJECTILE, pos, 6, 6), origin(rotation_origin), direction(rotation_direction)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    bullet = new Animation(texture_manager.enemy_bullet, 6, 6, true);
}

LaserBall::~LaserBall()
{
    delete bullet;
}

void LaserBall::update(GameScene *scene)
{
    (void)scene;
    Vector2 center{pos.x + 3.0f, pos.y + 3.0f};
    Vector2 new_center = Vector2Add(Vector2Rotate(Vector2Subtract(center, origin), angle * direction), origin);
    pos = {new_center.x - 3.0f, new_center.y - 3.0f};
}

void LaserBall::draw()
{
    bullet->draw(pos);
}

SpinningLaser::SpinningLaser(Vector2 pos, int no_lasers, SpinningLaserType type)
    : Entity(EntityType::ENEMY, pos, 16, 16)
{
    int direction{1};
    int alternate{1};

    switch (type)
    {
    case SpinningLaserType::LEFT:
        direction = -1;
        break;
    case SpinningLaserType::RIGHT:
        break;
    case SpinningLaserType::ALTERNATE_LEFT:
        direction = -1;
        alternate = -1;
        break;
    case SpinningLaserType::ALTERNATE_RIGHT:
        alternate = -1;
        break;
    default:
        break;
    }

    Vector2 origin{pos.x + 8, pos.y + 8};
    Vector2 ball_pos{origin.x - 3, origin.y - 3};

    for (int i = 0; i < no_lasers; i++)
    {
        ball_pos.y -= spacing;
        laser_balls.push_back(new LaserBall(ball_pos, origin, direction));
        direction *= alternate;
    }

    TextureManager &texture_manager = TextureManager::getInstance();
    texture = texture_manager.spinning_laser_base;
}

SpinningLaser::~SpinningLaser()
{
    for (LaserBall *laser_ball : laser_balls)
        delete laser_ball;
}

void SpinningLaser::update(GameScene *scene)
{
    (void)scene;
    frame_counter++;
    if (frame_counter % 2 == 0)
    {
        pos.x--;
        for (LaserBall *laser_ball : laser_balls)
        {
            laser_ball->pos.x--;
            laser_ball->origin.x--;
        }
    }

    for (LaserBall *laser_ball : laser_balls)
        laser_ball->update(scene);

    if (pos.x < -(spacing * (laser_balls.size() + 1)))
        delete_entity = true;
}

void SpinningLaser::draw()
{
    DrawTexture(texture, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);

    for (LaserBall *laser_ball : laser_balls)
        laser_ball->draw();
}

void SpinningLaser::handleCollision(GameScene *scene, Entity *entity)
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

bool SpinningLaser::checkCollision(const Entity &entity) const
{
    if (entity.type != EntityType::PLAYER)
        return false;

    // Check if the player is colliding with the base of the spinner
    if (Entity::checkCollision(entity))
        return true;

    // Check with each laser ball now
    for (LaserBall *laser_ball : laser_balls)
        if (laser_ball->checkCollision(entity))
            return true;

    return false;
}