#include "entity.hpp"

Entity::Entity(Vector2 pos, int width, int height, GameScene *scene) : pos(pos), width(width), height(height), scene(scene) {}

Rectangle Entity::getRect() {
    return Rectangle{pos.x, pos.y, static_cast<float>(width), static_cast<float>(height)};
}

bool Entity::checkCollision(Entity &entity) {
    return CheckCollisionRecs(getRect(), entity.getRect());
}