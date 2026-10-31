#include "entities/projectile.hpp"
#include "constants.hpp"

Projectile::Projectile(EntityType type, Vector2 pos, int width, int height, Vector2 velocity) : Entity(type, pos, width, height), velocity(velocity) {}

void Projectile::update(GameScene *scene) {
    (void)scene;
    pos.x += velocity.x;
    pos.y += velocity.y;

    // Check if the projectile is off screen and automatically mark it for deletion
    if (isOffScreen()) {
        delete_entity = true;
    }
}

bool Projectile::isOffScreen() {
    Rectangle screen{0, 0, CANVAS_WIDTH, CANVAS_HEIGHT};
    return !CheckCollisionRecs(screen, getRect());
}

void Projectile::draw() {
    DrawRectangle(static_cast<int>(pos.x), static_cast<int>(pos.y), width, height, RED);
}