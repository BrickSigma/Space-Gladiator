#include "entities/power_up.hpp"

#include "utils/texture_manager.hpp"

PowerUp::PowerUp(PowerUpType type_, Vector2 pos_) : Entity(EntityType::POWER_UP, {pos_.x - 6.0f, pos_.y - 6.0f}, 12, 12), type(type_)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    texture = texture_manager.power_ups;

    sprite_index = static_cast<float>(type);
}

void PowerUp::update(GameScene *scene)
{
    (void)scene;
    frame_counter++;
    if (frame_counter % 2 == 0)
        pos.x--;

    if (isOffScreen())
        delete_entity = true;
}

void PowerUp::draw()
{
    DrawTextureRec(texture, {12.0f * sprite_index, 0, 12, 12}, pos, WHITE);
}