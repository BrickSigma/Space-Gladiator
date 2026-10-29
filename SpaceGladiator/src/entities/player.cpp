#include "entities/player.hpp"
#include "game_scene.hpp"

#include "constants.hpp"
#include "texture_manager.hpp"

Player::Player() : Entity(EntityType::PLAYER, {24, (CANVAS_HEIGHT / 2) - 6}, 26, 12)
{
    reset();

    // Load up the player's assets and other components needed
    TextureManager &texture_manager = TextureManager::getInstance();

    textures[0] = texture_manager.player_normal;
    textures[1] = texture_manager.player_down;
    textures[2] = texture_manager.player_up;
}

void Player::reset()
{
    // Anything to do with the player's state can be reset here
    speed = PLAYER_BASE_SPEED; // Reset the speed back to normal
}

Player::~Player()
{
    // Free any allocated objects
}

void Player::handleInput()
{
    movement[0] = false;
    movement[1] = false;
    movement[2] = false;
    movement[3] = false;

    if (IsKeyDown(KEY_UP))
        movement[PLAYER_UP] = true;
    if (IsKeyDown(KEY_DOWN))
        movement[PLAYER_DOWN] = true;
    if (IsKeyDown(KEY_LEFT))
        movement[PLAYER_LEFT] = true;
    if (IsKeyDown(KEY_RIGHT))
        movement[PLAYER_RIGHT] = true;
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
    if (pos.x >= CANVAS_WIDTH - (16 + width))
        pos.x = CANVAS_WIDTH - (16 + width);
    if (pos.y < 16)
        pos.y = 16;
    if (pos.y >= CANVAS_HEIGHT - (16 + height))
        pos.y = CANVAS_HEIGHT - (16 + height);
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
    DrawTexture(textures[texture], pos.x, pos.y, WHITE);
}

void Player::update(GameScene *scene)
{
    (void)scene;
    
    // Handle any user input
    handleInput();

    // Handle the physics of moving
    handlePhysics();
}