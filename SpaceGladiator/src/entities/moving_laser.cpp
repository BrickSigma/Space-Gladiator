#include "entities/moving_laser.hpp"
#include "scenes/game_scene.hpp"

#include "utils/texture_manager.hpp"

#include <cmath>
#include <numbers>

constexpr Vector2 LASER_SPAWN_POS{
    static_cast<float>(SPACE_CANVAS_WIDTH),
    static_cast<float>(SPACE_CANVAS_HEIGHT / 2 - MOVING_LASER_HEIGHT / 2)};

constexpr float SCENE_CENTER{static_cast<float>(SPACE_CANVAS_HEIGHT / 2)};

MovingLaser::MovingLaser()
    : Entity(EntityType::ENEMY, LASER_SPAWN_POS, MOVING_LASER_WIDTH, MOVING_LASER_HEIGHT)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    laser_animation = new Animation(texture_manager.moving_laser, 48, 6, true);
    moving_laser_hole = texture_manager.moving_laser_hole;
    laser_loading_animation = new Animation(texture_manager.laser_loading, 5, 3, false);
}

MovingLaser::~MovingLaser()
{
    delete laser_animation;
    delete laser_loading_animation;
}

void MovingLaser::update(GameScene *scene)
{
    switch (state)
    {
    case MovingLaserState::SPAWNING:
        pos.x--;
        if (pos.x <= (SPACE_CANVAS_WIDTH - MOVING_LASER_WIDTH))
        {
            state = MovingLaserState::MOVING;
        }
        break;

    case MovingLaserState::MOVING:
        pos.y = (SCENE_CENTER * 2.0f / 3.0f) * sinf((static_cast<float>(frame_counter) / period) * 2 * std::numbers::pi_v<float>) + SCENE_CENTER - MOVING_LASER_HEIGHT / 2;
        frame_counter++;
        if (frame_counter >= move_duration)
        {
            state = MovingLaserState::TARGETING;
            // Lock onto the player's position
            const Player &player = scene->player;
            player_target_y = (player.pos.y + static_cast<float>(player.height) / 2.0f) - MOVING_LASER_HEIGHT / 2.0f;
            if (player_target_y < pos.y)
                target_direction = -1;
        }
        break;

    case MovingLaserState::TARGETING:
    {
        pos.y += static_cast<float>(target_direction * 2);
        // Check if we've reached or passed the player's last position
        bool reached_target = (target_direction < 0)
                                  ? (pos.y <= player_target_y)
                                  : ((pos.y >= player_target_y) || (pos.y + MOVING_LASER_HEIGHT >= SPACE_CANVAS_HEIGHT - 16));

        if (reached_target)
        {
            state = MovingLaserState::LOADING;
            frame_counter = 0;
        }
        break;
    }
    case MovingLaserState::LOADING:
        frame_counter++;
        if (frame_counter >= load_duration)
        {
            state = MovingLaserState::FIRING;
            scene->setCameraShake(CameraShake(3, static_cast<unsigned int>(firing_duration)));
            frame_counter = 0;
        }
        break;

    case MovingLaserState::FIRING:
        frame_counter++;
        if (frame_counter >= firing_duration)
        {
            state = MovingLaserState::RETRACTING;
        }
        break;

    case MovingLaserState::RETRACTING:
        pos.x++;
        if (pos.x >= SPACE_CANVAS_WIDTH)
        {
            delete_entity = true; // Delete the entity at the end of the state machine
        }
        break;
    default:
        break;
    }
}

void MovingLaser::draw()
{
    DrawTexture(moving_laser_hole, static_cast<int>(pos.x), static_cast<int>(pos.y) + 8, WHITE);

    if (state == MovingLaserState::LOADING)
    {
        laser_loading_animation->draw({pos.x, pos.y + 8.0f});
    }
    else if (state == MovingLaserState::FIRING)
    {
        // Draw the laser beam here
        const Rectangle laser_rect = getLaserRect();
        DrawRectangle(static_cast<int>(laser_rect.x), static_cast<int>(laser_rect.y), static_cast<int>(laser_rect.width), static_cast<int>(laser_rect.height), RED);
    }

    laser_animation->draw(pos);
}

bool MovingLaser::checkCollision(const Entity &entity) const
{
    if (entity.type != EntityType::PLAYER)
        return false;

    // Check if the player has hit the body of the laser beam
    if (Entity::checkCollision(entity))
        return true;

    // Also check if the player has hit the laser beam itself if it is fired
    if (state == MovingLaserState::FIRING)
    {
        if (CheckCollisionRecs(entity.getRect(), getLaserRect()))
        {
            return true;
        }
    }

    return false;
}

Rectangle MovingLaser::getLaserRect() const
{
    return {0, pos.y + 8.0f, pos.x + 5, 18.0f};
}