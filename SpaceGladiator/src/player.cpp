#include "player.hpp"
#include "game_scene.hpp"

#include "constants.hpp"

Player::Player(GameScene *scene) : Entity({24, (CANVAS_HEIGHT / 2) - 6}, 26, 12, scene)
{
    reset();

    // Load up the player's assets and other components needed
}

void Player::reset()
{
    // Anything to do with the player's state can be reset here
    speed = PLAYER_BASE_SPEED; // Reset the speed back to normal
}

Player::~Player()
{
    // Free any allocated assets
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
}

void Player::draw()
{
    DrawRectangle(pos.x, pos.y, width, height, RED);
}

void Player::update()
{
    // Handle any user input
    handleInput();

    // Handle the physics of moving
    handlePhysics();
}