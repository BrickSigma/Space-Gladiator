#include "entities/bosses/big_spaceship.hpp"

#include "utils/texture_manager.hpp"
#include "scenes/game_scene.hpp"
#include "entities/bosses/big_spaceship_bullet.hpp"
#include "utils/audio_manager.hpp"

#include <cmath>
#include <numbers>

static constexpr float Y_ORIGIN{static_cast<float>(SPACE_CANVAS_HEIGHT / 2.0f) - 24.0F};
static constexpr Vector2 SPAWN{static_cast<float>(SPACE_CANVAS_WIDTH), Y_ORIGIN};

BigSpaceship::BigSpaceship() : Entity(EntityType::ENEMY, SPAWN, 48, 48)
{
    // Set the y position so the spaceship doesn't "jump" into place
    pos.y = getYPos(0);

    TextureManager &texture_manager = TextureManager::getInstance();
    texture = texture_manager.big_spaceship;

    explosion = new Animation(texture_manager.giant_explosion, 48, 8, false);

    is_tile = true; // If the player hits the spaceship they'll die
}

BigSpaceship::~BigSpaceship()
{
    delete explosion;
}

void BigSpaceship::update(GameScene *scene)
{
    switch (state)
    {
    case BigSpaceshipState::SPAWNING:
        pos.x--;
        if (pos.x <= static_cast<float>(SPACE_CANVAS_WIDTH - 100))
            state = BigSpaceshipState::MOVING;
        break;

    case BigSpaceshipState::MOVING:
    {
        frame_counter++;
        pos.y = getYPos(frame_counter);
        if (frame_counter % fire_time == 0)
        {
            scene->spawnEntity(new BigSpaceshipBullet({pos.x, pos.y + 4.0f}));
            scene->spawnEntity(new BigSpaceshipBullet({pos.x - 16.0f, pos.y + 16.0f}));
            scene->spawnEntity(new BigSpaceshipBullet({pos.x - 16.0f, pos.y + 29.0f}));
            scene->spawnEntity(new BigSpaceshipBullet({pos.x, pos.y + 41.0f}));
        }
        break;
    }

    case BigSpaceshipState::DEAD:
        frame_counter++;
        if (frame_counter >= death_time)
        {
            delete_entity = true;
            scene->nextLevel();
        }
        break;

    default:
        break;
    }
}

void BigSpaceship::draw()
{
    if (state == BigSpaceshipState::DEAD)
        explosion->draw(pos);
    else
        DrawTexture(texture, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
}

void BigSpaceship::handleCollision(GameScene *scene, Entity *entity)
{
    if (is_dead || state != BigSpaceshipState::MOVING)
        return;

    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (!checkCollision(*entity))
        return;

    entity->delete_entity = true;

    Rectangle hit_box{pos.x, pos.y + 17.0f, 48, 14};
    if (!CheckCollisionRecs(hit_box, entity->getRect()))
        return;

    health -= 3;
    scene->setCameraShake(CameraShake(2, 30));
    scene->setBossHealth(health);

    if (health <= 0)
    {
        is_dead = true;
        state = BigSpaceshipState::DEAD;
        frame_counter = 0;
        scene->addScore(score);
        scene->setCameraShake(CameraShake(3, 90));
        AudioManager::getInstance().playSFX(SFXFile::BOSS_EXPLOSION);
    }
}

bool BigSpaceship::checkCollision(const Entity &entity) const
{
    if (is_dead)
        return false;

    return Entity::checkCollision(entity);
}

float BigSpaceship::getYPos(int frame)
{
    float t = (static_cast<float>(frame) / period) * 2 * std::numbers::pi_v<float>;
    float y = (-0.5f * sinf(t)) + (0.25f * sinf((t / 3) + (std::numbers::pi_v<float> / 3.0f))) + (0.25f * sinf((t / 5.0f) - (std::numbers::pi_v<float> / 5.0f)));
    return static_cast<float>(SPACE_CANVAS_HEIGHT / 2.0f) * (1 + y) - 24.0f;
}