#include "entities/entity.hpp"
#include "constants.hpp"

Entity::Entity(EntityType type, Vector2 pos, int width, int height) : type(type), pos(pos), width(width), height(height) {}

Entity::Entity(EntityType type) : type(type) {}

Rectangle Entity::getRect() const
{
    return Rectangle{pos.x, pos.y, static_cast<float>(width), static_cast<float>(height)};
}

bool Entity::checkCollision(const Entity &entity) const
{
    return CheckCollisionRecs(getRect(), entity.getRect());
}

void Entity::handleCollision(GameScene *scene, Entity *entity)
{
    // Does nothing by default
    (void)scene;
    (void)entity;
}

bool Entity::isOffScreen()
{
    Rectangle screen{0, 0, CANVAS_WIDTH, CANVAS_HEIGHT};
    return !CheckCollisionRecs(screen, getRect());
}