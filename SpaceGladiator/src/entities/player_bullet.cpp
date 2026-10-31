#include "entities/player_bullet.hpp"
#include "texture_manager.hpp"

PlayerBullet::PlayerBullet(Vector2 pos, Vector2 velocity) : Projectile(EntityType::PLAYER_PROJECTILE, pos, 8, 3, velocity) {}

void PlayerBullet::draw() {
    TextureManager &texture_manager = TextureManager::getInstance();
    DrawTexture(texture_manager.player_bullet, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
}