#include "entities/player/player_multishot.hpp"

#include "utils/texture_manager.hpp"

PlayerMultishot::PlayerMultishot(Vector2 pos_, int direction_) : PlayerProjectile(PlayerProjectileType::MULTISHOT, pos_, 7, 7, {0, 0}), direction(direction_)
{
    velocity.x = MULTISHOT_SPEED;
    velocity.y = static_cast<float>(direction) * MULTISHOT_SPEED;

    TextureManager &texture_manager = TextureManager::getInstance();
    texture = texture_manager.player_multishot;
}

void PlayerMultishot::draw()
{
    DrawTextureRec(texture, {0, 0, 7.0f, static_cast<float>(direction) * -7.0f}, pos, WHITE);
}