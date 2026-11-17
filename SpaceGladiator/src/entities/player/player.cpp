#include "entities/player/player.hpp"
#include "scenes/game_scene.hpp"

#include "utils/constants.hpp"
#include "utils/texture_manager.hpp"
#include "entities/player/player_bullet.hpp"
#include "entities/player/player_laser.hpp"
#include "entities/player/player_multishot.hpp"
#include "entities/explosion.hpp"

#include <algorithm>

const float GAMEPAD_DEADZONE = 0.5f;

Player::Player() : Entity(EntityType::PLAYER, {30, (CANVAS_HEIGHT / 2) - (PLAYER_HITBOX_HEIGHT / 2)}, PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT)
{
    // Load up the player's assets and other components needed
    TextureManager &texture_manager = TextureManager::getInstance();

    textures[0] = texture_manager.player_normal;
    textures[1] = texture_manager.player_down;
    textures[2] = texture_manager.player_up;

    // Setup the player explosion as well
    explosion = new Animation(texture_manager.player_explosion, 28, 12, false);

    reset(true);
}

void Player::reset(bool restore_health)
{
    // Anything to do with the player's state can be reset here
    pos.x = 30;
    pos.y = static_cast<float>((CANVAS_HEIGHT / 2) - (PLAYER_HITBOX_HEIGHT / 2));
    speed = PLAYER_BASE_SPEED;          // Reset the speed back to normal
    max_shots = PLAYER_MAX_PROJECTILES; // Reset the number of shots a player can take
    shield = 0;                         // Remove the sheild
    has_laser = false;                  // Disable the laser
    has_multishot = false;              // Disable multishot
    shoot_delay = 10;
    is_dead = false;
    explosion->reset();

    if (restore_health)
        lives = PLAYER_MAX_LIVES;
}

Player::~Player()
{
    // Free any allocated objects
    delete explosion;
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
        pos.y -= speed;
    else if (movement[PLAYER_DOWN])
        pos.y += speed;

    if (movement[PLAYER_LEFT])
        pos.x -= speed;
    else if (movement[PLAYER_RIGHT])
        pos.x += speed;

    // Make sure the player can't go off screen
    if (pos.x < 16)
        pos.x = 16;
    if (pos.x >= static_cast<float>(SPACE_CANVAS_WIDTH - (16 + width)))
        pos.x = static_cast<float>(SPACE_CANVAS_WIDTH - (16 + width));
    if (pos.y < 16)
        pos.y = 16;
    if (pos.y >= static_cast<float>(SPACE_CANVAS_HEIGHT - (8 + height)))
        pos.y = static_cast<float>(SPACE_CANVAS_HEIGHT - (8 + height));
}

void Player::handleShooting(GameScene *scene)
{
    // First iterate through the entities in the game to find any that are the player's projectile type
    // and also specifically  the bullet type, not laser or multi-shot
    size_t projectiles = static_cast<size_t>(
        std::count_if(scene->entities.begin(), scene->entities.end(), [](Entity *e)
                      { 
                        if (e->type != EntityType::PLAYER_PROJECTILE)
                            return false;

                        PlayerProjectile *projectile = static_cast<PlayerProjectile *>(e);
                        return projectile->type == PlayerProjectileType::BULLET; }));

    // Spawn a bullet if all conditions are met
    if (is_shooting && (projectiles < max_shots || has_laser) && shooting_timer >= shoot_delay)
    {
        if (has_laser)
            scene->spawnEntity(new PlayerLaser({pos.x + static_cast<float>(width), getCenter().y - 1.0f}));
        else
            scene->spawnEntity(new PlayerBullet(
                {pos.x + static_cast<float>(width), pos.y + static_cast<float>(height) / 2}, {PROJECTILE_SPEED, 0}));
        shooting_timer = 0; // Reset the timer
    }

    // Increment the shoot timer
    shooting_timer++;

    frame_counter++;
    if (has_multishot)
    {
        if (frame_counter % multishot_delay == 0)
        {
            scene->spawnEntity(new PlayerMultishot(
                {pos.x + static_cast<float>(width), getCenter().y}, 1));
            scene->spawnEntity(new PlayerMultishot(
                {pos.x + static_cast<float>(width), getCenter().y}, -1));
        }
    }
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
        texture = PLAYER_TEXTURE_UP;
    else if (movement[PLAYER_DOWN])
        texture = PLAYER_TEXTURE_DOWN;

    if (is_dead)
    {
        explosion->draw({pos.x - 7, pos.y - 5});
    }
    else
    {
        DrawTexture(textures[texture], static_cast<int>(pos.x - 6), static_cast<int>(pos.y - 3), WHITE);
        if (shield > 0)
            DrawCircleLines(static_cast<int>(getCenter().x), static_cast<int>(getCenter().y), static_cast<float>(PLAYER_SPRITE_WIDTH + 5) / 2.0f, PLAYER_LASER_COLOR);
    }
}

void Player::handleCollision(GameScene *scene, Entity *entity)
{
    if (is_dead)
        return;

    if ((entity->type == EntityType::PLAYER_PROJECTILE) || (entity->type == EntityType::PLAYER))
        return;

    // The entity calls it's own checkCollision function in case it has it's own custom one (like the spinning laser)
    if (!(entity->checkCollision(*this)))
        return;

    // Check the entity type: if it's a powerup we need to activate it, otherwise if it's an enemy bullet we need to delete it
    if (entity->type == EntityType::POWER_UP)
    {
        handlePowerUp(static_cast<PowerUp *>(entity));
        return;
    }
    else if (entity->type == EntityType::ENEMY_PROJECTILE)
    {
        entity->delete_entity = true; // Mark the projectile as dead since it hit the player
        if (shield > 0)
        { // Let the shield protect the player from dying
            shield--;
            return;
        }
    }
    else
    {
        if (shield >= 4 && !entity->is_tile)
        { // Enemies will cause the shield to loose 4 bars of health
            shield -= 4;
            entity->delete_entity = true; // Delete the entity

            scene->spawnEntity(new Explosion(1, entity->getCenter())); // Spawn an exlosion as well
            return;                                                    // If the shield isn't strong enough, the player will die
        }
    }

    is_dead = true;
    lives--;
    clearMovementArray();

    // Tell the game scene that the player has died
    scene->pushStateEvent(GameSceneState::PLAYER_KILLED);

    // Also shake the camera a bit
    scene->setCameraShake(CameraShake(2, 120));
}

void Player::handleTilemapCollision(GameScene *scene)
{
    if (is_dead)
        return;

    if (!checkTilemapCollision(scene->level_manager.getTilemap(), scene->scroll_distance))
        return;

    is_dead = true;
    lives--;
    clearMovementArray();

    scene->pushStateEvent(GameSceneState::PLAYER_KILLED);
    scene->setCameraShake(CameraShake(2, 120));
}

void Player::handlePowerUp(PowerUp *power_up)
{
    switch (power_up->type)
    {
    case PowerUpType::SPEED:
        speed += 0.25f;
        if (speed >= 1.75f)
            speed = 1.75f; // Cap the player speed to prevent the player from loosing control
        break;
    case PowerUpType::MULTISHOT:
        has_multishot = true;
        break;
    case PowerUpType::LASER:
        has_laser = true;
        shoot_delay = 20;
        break;
    case PowerUpType::SHIELD:
        shield = PLAYER_SHIELD_MAX_HEALTH;
        break;
    case PowerUpType::EXTRA_SHOT:
        max_shots++;
        break;
    case PowerUpType::NONE:
        break;
    }

    // Don't forget to delete the power-up!
    power_up->delete_entity = true;
}