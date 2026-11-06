#include "entities/player.hpp"
#include "scenes/game_scene.hpp"

#include "constants.hpp"
#include "texture_manager.hpp"
#include "entities/player_bullet.hpp"

#include <algorithm>

const float GAMEPAD_DEADZONE = 0.5f;

Player::Player() : Entity(EntityType::PLAYER, {30, (CANVAS_HEIGHT / 2) - (PLAYER_HITBOX_HEIGHT / 2)}, PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT)
{
    reset(true);

    // Load up the player's assets and other components needed
    TextureManager &texture_manager = TextureManager::getInstance();

    textures[0] = texture_manager.player_normal;
    textures[1] = texture_manager.player_down;
    textures[2] = texture_manager.player_up;
}

void Player::reset(bool restore_health)
{
    // Anything to do with the player's state can be reset here
    pos.x = 30;
    pos.y = static_cast<float>((CANVAS_HEIGHT / 2) - (PLAYER_HITBOX_HEIGHT / 2));
    speed = PLAYER_BASE_SPEED; // Reset the speed back to normal
    is_dead = false;

    if (restore_health)
        lives = PLAYER_MAX_LIVES;
}

Player::~Player()
{
    // Free any allocated objects
}

void Player::clearMovementArray()
{
    movement[0] = false;
    movement[1] = false;
    movement[2] = false;
    movement[3] = false;
}

void Player::handleInput()
{
    // Reset any input states to false
    clearMovementArray();
    is_shooting = false;

    // Keyboard input controls
    if (IsKeyDown(KEY_UP))
        movement[PLAYER_UP] = true;
    if (IsKeyDown(KEY_DOWN))
        movement[PLAYER_DOWN] = true;
    if (IsKeyDown(KEY_LEFT))
        movement[PLAYER_LEFT] = true;
    if (IsKeyDown(KEY_RIGHT))
        movement[PLAYER_RIGHT] = true;
    if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_S))
        is_shooting = true;

    // Gamepad controls
    if (IsGamepadAvailable(0))
    {
        float x_axis = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
        float y_axis = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

        if (y_axis < -GAMEPAD_DEADZONE)
            movement[PLAYER_UP] = true;
        if (y_axis > GAMEPAD_DEADZONE)
            movement[PLAYER_DOWN] = true;
        if (x_axis < -GAMEPAD_DEADZONE)
            movement[PLAYER_LEFT] = true;
        if (x_axis > GAMEPAD_DEADZONE)
            movement[PLAYER_RIGHT] = true;

        if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
            is_shooting = true;
    }
}

void Player::handlePhysics()
{
    if (movement[PLAYER_UP])
    {
        pos.y -= speed;
    }
    else if (movement[PLAYER_DOWN])
    {
        pos.y += speed;
    }

    if (movement[PLAYER_LEFT])
    {
        pos.x -= speed;
    }
    else if (movement[PLAYER_RIGHT])
    {
        pos.x += speed;
    }

    // Make sure the player can't go off screen
    if (pos.x < 16)
        pos.x = 16;
    if (pos.x >= SPACE_CANVAS_WIDTH - (16 + width))
        pos.x = static_cast<float>(SPACE_CANVAS_WIDTH - (16 + width));
    if (pos.y < 16)
        pos.y = 16;
    if (pos.y >= SPACE_CANVAS_HEIGHT - (16 + height))
        pos.y = static_cast<float>(SPACE_CANVAS_HEIGHT - (16 + height));
}

void Player::handleShooting(GameScene *scene)
{
    // First iterate through the entities in the game to find any that are the player's projectile type
    size_t projectiles = std::count_if(scene->entities.begin(), scene->entities.end(), [](Entity *e)
                                       { return e->type == EntityType::PLAYER_PROJECTILE; });

    // Spawn a bullet if all conditions are met
    if (is_shooting && projectiles < 2 && shooting_timer >= SHOOT_DELAY)
    {
        scene->spawnEntity(new PlayerBullet(
            {pos.x + width, pos.y + height / 2}, {PROJECTILE_SPEED, 0}));
        shooting_timer = 0; // Reset the timer
    }

    // Increment the shoot timer
    shooting_timer++;
}

void Player::update(GameScene *scene)
{
    if (!is_dead)
    {
        handleInput();
        handlePhysics();
        handleShooting(scene);
    }
}

void Player::draw()
{
    int texture = PLAYER_TEXTURE_NORMAL;
    if (movement[PLAYER_UP])
    {
        texture = PLAYER_TEXTURE_UP;
    }
    else if (movement[PLAYER_DOWN])
    {
        texture = PLAYER_TEXTURE_DOWN;
    }

    Color tint = is_dead ? RED : WHITE;

    DrawTexture(textures[texture], static_cast<int>(pos.x - 6), static_cast<int>(pos.y - 3), tint);
}

void Player::handleCollision(GameScene *scene, Entity *entity)
{
    (void)scene;

    if (is_dead)
        return;

    if ((entity->type == EntityType::PLAYER_PROJECTILE) || (entity->type == EntityType::PLAYER))
        return;

    if (!checkCollision(*entity))
        return;

    is_dead = true;
    lives--;
    clearMovementArray();

    // Tell the game scene that the player has died
    scene->pushStateEvent(GameSceneState::PLAYER_KILLED);
}