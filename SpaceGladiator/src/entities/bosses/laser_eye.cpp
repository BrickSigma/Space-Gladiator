#include "entities/bosses/laser_eye.hpp"

#include "utils/texture_manager.hpp"
#include "scenes/game_scene.hpp"

#include <raymath.h>
#include <cmath>

LaserEye::LaserEye(Vector2 pos_) : Entity(EntityType::ENEMY, pos_, WIDTH, HEIGHT)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    laser_eye = texture_manager.laser_eye;
    eye_ball = texture_manager.laser_eye_ball;
    background = texture_manager.laser_eye_back;

    frames = new Animation(texture_manager.laser_eye_frames, 64, 5, false);
    center = {pos.x + 32.0f, pos.y + 32.0f};
    is_tile = true;
}

LaserEye::~LaserEye()
{
    delete frames;
}

bool LaserEye::checkCollision(const Entity &entity) const
{
    if (is_dead)
        return false;

    Rectangle rect{pos.x + 2.0f, pos.y + 14.0f, 60, 34};
    if (CheckCollisionRecs(rect, entity.getRect()))
        return true;

    return false;
}

void LaserEye::handleCollision(GameScene *scene, Entity *entity)
{
    if (is_dead || current_state == LaserEyeState::DELAY || current_state == LaserEyeState::SPAWNING)
        return;

    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (!checkCollision(*entity))
        return;

    entity->delete_entity = true;
    scene->setCameraShake(CameraShake(2, 30));

    if (current_state != LaserEyeState::WAITING)
        return; // The player can only deal damage to the eye when it is in the waiting state

    health--;
    scene->setBossHealth(health);
    if (health <= 0)
    {
        is_dead = true;
        scene->addScore(score);
        scene->setCameraShake(CameraShake(3, 90));
    }
}

void LaserEye::draw()
{
    if (current_state == LaserEyeState::DELAY)
    {
        DrawTexture(background, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
        return;
    } else if (current_state == LaserEyeState::SPAWNING)
    {
        frames->draw(pos);
        return;
    }

    DrawTexture(laser_eye, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
    
    // Eye ball drawing
    // Calculate the eye's position based on the player's position
    Vector2 direction{8.0f, 0.0f};
    float magnitude = Remap(player_distance, 0.0f, distance_clamp, 0.0f, 1.0f);
    direction = Vector2Scale(direction, magnitude);
    direction = Vector2Rotate(direction, player_angle);
    // Direction will now hold the eye ball's center coordinate
    direction.x -= 12.0f;
    direction.y -= 12.0f;
    direction = Vector2Add(direction, center);

    DrawTexture(eye_ball, static_cast<int>(direction.x), static_cast<int>(direction.y), WHITE);
}

void LaserEye::update(GameScene *scene)
{
    if (is_dead)
    {
        frame_counter++;
        if (frame_counter >= death_time)
        {
            delete_entity = true;
            scene->nextLevel();
        }
        return;
    }

    switch (current_state)
    {
    case LaserEyeState::DELAY:
        frame_counter++;
        if (frame_counter % 2 == 0)
        {
            pos.x--; // Update the x position as it loads in
            if (pos.x <= (SPACE_CANVAS_WIDTH / 2.0f - WIDTH / 2.0f))
                pos.x = SPACE_CANVAS_WIDTH / 2.0f - WIDTH / 2.0f;
        }
        if (frame_counter >= delay_time)
        {
            current_state = LaserEyeState::SPAWNING;
            frame_counter = 0;
        }
        break;

    case LaserEyeState::SPAWNING:
        if (frames->completed)
            current_state = LaserEyeState::WAITING;
        break;

    case LaserEyeState::ATTACK_1:
        attack1(scene);
        break;

    case LaserEyeState::ATTACK_2:
        attack2(scene);
        break;

    case LaserEyeState::ATTACK_3:
        attack3(scene);
        break;

    case LaserEyeState::ATTACK_4:
        attack4(scene);
        break;

    default:
        break;
    }

    // Update the eye's center at the end of the frame
    center = {pos.x + 32.0f, pos.y + 32.0f};
    
    // Track the player's position relative to the eye's center
    Vector2 player_pos = scene->player.getCenter();
    float dx = player_pos.x - center.x;
    float dy = player_pos.y - center.y;
    player_angle = atan2f(dy, dx);

    // Also get the player's distance from the eye's ceneter
    player_distance = Vector2Distance(player_pos, center);
}

void LaserEye::attack1(GameScene *scene)
{
    (void)scene;
}

void LaserEye::attack2(GameScene *scene)
{
    (void)scene;
}

void LaserEye::attack3(GameScene *scene)
{
    (void)scene;
}

void LaserEye::attack4(GameScene *scene)
{
    (void)scene;
}