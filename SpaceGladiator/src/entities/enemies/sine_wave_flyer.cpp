#include "entities/enemies/sine_wave_flyer.hpp"

#include "scenes/game_scene.hpp"
#include "utils/texture_manager.hpp"
#include "entities/explosion.hpp"
#include "entities/player/player_projectile.hpp"

#include <cmath>
#include <numbers>

SineWaveFlyer::SineWaveFlyer(Vector2 pos_, PowerUpType power_up_type_) : Entity(EntityType::ENEMY, pos_, 16, 16), power_up_type(power_up_type_)
{
    origin = pos_.y;

    TextureManager &texture_manager = TextureManager::getInstance();
    Texture texture;
    if (power_up_type != PowerUpType::NONE)
        texture = texture_manager.special_sine_wave_flyer;
    else
        texture = texture_manager.regular_sine_wave_flyer;

    animation = new Animation(texture, 16, 10, true);
}

SineWaveFlyer::~SineWaveFlyer()
{
    delete animation;
}

void SineWaveFlyer::update(GameScene *scene)
{
    (void)scene;
    frame_counter++;
    if (frame_counter % update_x_time == 0)
        pos.x += x_vel;

    pos.y = origin + (max_height * sinf((static_cast<float>(frame_counter) / period) * 2 * std::numbers::pi_v<float>));

    if (isOffScreen() && pos.x < SPACE_CANVAS_WIDTH)
        delete_entity = true;
}

void SineWaveFlyer::draw()
{
    animation->draw(pos);
}

void SineWaveFlyer::handleCollision(GameScene *scene, Entity *entity)
{
    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (!checkCollision(*entity))
        return;

    // The flyer was hit by a player bullet, so let's delete it
    delete_entity = true;

    scene->addScore(score);

    PlayerProjectile *projectile = static_cast<PlayerProjectile *>(entity);
    projectile->handleDestruction(); // Call the projectile's own destruction handler, as the laser may not get destroyed

    scene->setCameraShake(CameraShake(2, 30));

    if (power_up_type != PowerUpType::NONE)
        scene->spawnEntity(new PowerUp(power_up_type, getCenter()));

    // Spawn an explosion
    scene->spawnEntity(new Explosion(1, getCenter()));
}