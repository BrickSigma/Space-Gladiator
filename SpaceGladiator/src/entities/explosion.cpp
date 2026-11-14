#include "entities/explosion.hpp"

#include "utils/texture_manager.hpp"

Explosion::Explosion(int type_, Vector2 ceneter) : Entity(EntityType::NONE, ceneter, 16, 16)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    switch (type_)
    {
    case 2:
        animation = new Animation(texture_manager.explosion_2_frames, 16, 4, false);
        pos.x -= 8;
        pos.y -= 8;
        break;
    default:
        animation = new Animation(texture_manager.explosion_1_frames, 16, 4, false);
        pos.x -= 8;
        pos.y -= 8;
        break;
    }
}

Explosion::~Explosion()
{
    delete animation;
}

void Explosion::update(GameScene *scene)
{
    (void)scene;
    if (animation->completed)
    {
        delete_entity = true;
        return;
    }

    frame_counter++;
    if (frame_counter % DISTANCE_UPDATE_TIMER == 2)
        pos.x--;

    if (isOffScreen())
        delete_entity = true;
}

void Explosion::draw()
{
    animation->draw(pos);
}