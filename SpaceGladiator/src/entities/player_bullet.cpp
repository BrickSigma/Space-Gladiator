#include "entities/player_bullet.hpp"
#include "utils/texture_manager.hpp"

PlayerBullet::PlayerBullet(Vector2 pos_, Vector2 velocity_) : Projectile(EntityType::PLAYER_PROJECTILE, pos_, 8, 3, velocity_) {}

void PlayerBullet::draw() {
    TextureManager &texture_manager = TextureManager::getInstance();
    DrawTexture(texture_manager.player_bullet, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
}