#include "entities/bosses/snakes.hpp"

#include "utils/texture_manager.hpp"
#include "scenes/game_scene.hpp"
#include "entities/explosion.hpp"
#include "entities/bosses/snake_fire.hpp"

#include <cmath>
#include <iostream>
#include <numbers>

Snake::Snake(Vector2 base_, Vector2 head_origin_, Vector2 amplitude_, int period_offset, int fire_time_, int health_)
    : Entity(EntityType::ENEMY, {SPACE_CANVAS_WIDTH + 8.0f + amplitude_.x, base_.y}, 16, 16),
      base(base_), head_origin(head_origin_), amplitude(amplitude_),
      frame_counter(period_offset),
      fire_time(fire_time_)
{
    // Make sure the fire time isn't 0
    if (fire_time <= 0)
        throw std::runtime_error("Snake fire time cannot be less than or equal to 0!");

    TextureManager &texture_manager = TextureManager::getInstance();
    snake_head = texture_manager.snake_head;
    snake_spine = texture_manager.snake_spine;

    // Set the y position such that the snake doesn't "snap" into place once spawned
    pos.y = getYHead(frame_counter);

    health = health_;

    is_tile = true;
}

void Snake::update(GameScene *scene)
{
    if (is_dead)
        return;

    if (spawning)
    {
        pos.x--;
        if (pos.x <= getXHead(frame_counter))
            spawning = false;
    }
    else
    {
        frame_counter++;
        pos = {getXHead(frame_counter), getYHead(frame_counter)};
    }

    // Recalculate each of the spine positions
    for (int i = 0; i < SPINE_LEN; i++)
    {
        float frac = static_cast<float>(i) / static_cast<float>(SPINE_LEN);
        if (spawning)
        {
            spine[i].x = base.x + frac * (pos.x - base.x);
            spine[i].y = base.y + frac * (pos.y - base.y);
        }
        else
        {
            int offset = spawning ? 0 : -20;
            offset = static_cast<int>(static_cast<float>(offset) * (1 - frac));
            spine[i].x = base.x + frac * (getXHead(frame_counter + offset) - base.x);
            spine[i].y = base.y + frac * (getYHead(frame_counter + offset) - base.y);
        }
    }

    // Fire a fire ball
    if ((frame_counter % fire_time == 0) && !spawning)
    {
        // Get the angle towards the player
        Vector2 player_pos = scene->player.getCenter();
        Vector2 shooter_center = getCenter();
        float dx = player_pos.x - shooter_center.x;
        float dy = player_pos.y - shooter_center.y;
        float angle = atan2f(dy, dx);
        scene->spawnEntity(new SnakeFire({pos.x - 15.0f, pos.y - 3.0f}, angle));
    }
}

void Snake::draw()
{
    if (is_dead)
        return;
    for (int i = 0; i < SPINE_LEN; i++)
    {
        int x = static_cast<int>(spine[i].x - 4);
        int y = static_cast<int>(spine[i].y - 8);
        DrawTexture(snake_spine, x, y, WHITE);
    }

    if (mouth_open)
        DrawTextureRec(snake_head, {16, 0, 16, 16}, {pos.x - 8, pos.y - 8}, WHITE);
    else
        DrawTextureRec(snake_head, {0, 0, 16, 16}, {pos.x - 8, pos.y - 8}, WHITE);
}

void Snake::handleCollision(GameScene *scene, Entity *entity)
{
    if (is_dead || spawning)
        return;

    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (!checkCollision(*entity))
        return;

    entity->delete_entity = true;

    if (!checkHeadCollision(*entity))
        return; // If the bullet didn't collide with the snake head, don't reduce it's health

    health--;
    scene->setCameraShake(CameraShake(2, 30));

    if (health <= 0)
    {
        is_dead = true;
        scene->addScore(score);
        scene->setCameraShake(CameraShake(3, 90));

        // Spawn some explosions for each part of the snake
        for (int i = 0; i < SPINE_LEN; i++)
            scene->spawnEntity(new Explosion(1, spine[i]));

        scene->spawnEntity(new Explosion(1, pos));
    }
}

bool Snake::checkCollision(const Entity &entity) const
{
    if (is_dead)
        return false;

    // First check if the entity has collided with the head
    if (checkHeadCollision(entity))
        return true;

    for (int i = 0; i < SPINE_LEN; i++)
    {
        if (checkSpineCollision(entity, spine[i]))
            return true;
    }

    return false;
}

bool Snake::checkHeadCollision(const Entity &entity) const
{
    Rectangle head{pos.x - 8.0f, pos.y - 8.0f, 16, 16};
    return CheckCollisionRecs(entity.getRect(), head);
}

bool Snake::checkSpineCollision(const Entity &entity, Vector2 spine_pos) const
{
    Rectangle spine_rect{spine_pos.x - 4.0f, spine_pos.y - 8.0f, 8, 16};
    return CheckCollisionRecs(entity.getRect(), spine_rect);
}

float Snake::getXHead(int frame)
{
    float t = (static_cast<float>(frame) / period) * 2 * std::numbers::pi_v<float>;
    return head_origin.x + amplitude.x * cosf(t);
}

float Snake::getYHead(int frame)
{
    float t = (static_cast<float>(frame) / period) * 2 * std::numbers::pi_v<float>;
    return head_origin.y + amplitude.y * sinf(t);
}

Snakes::Snakes() : Entity(EntityType::ENEMY, {0, 0}, 0, 0)
{
    snakes[0] = new Snake({SPACE_CANVAS_WIDTH, SPACE_CANVAS_HEIGHT / 5.0f}, {SPACE_CANVAS_WIDTH - 72, SPACE_CANVAS_HEIGHT / 5.0f}, {8, 14}, 43, 180, 5);               // Top snake
    snakes[1] = new Snake({SPACE_CANVAS_WIDTH, SPACE_CANVAS_HEIGHT / 2.0f}, {SPACE_CANVAS_WIDTH - 80, SPACE_CANVAS_HEIGHT / 2.0f}, {12, 20}, 0, 120, 6);               // Middle snake
    snakes[2] = new Snake({SPACE_CANVAS_WIDTH, SPACE_CANVAS_HEIGHT * 4.0f / 5.0f}, {SPACE_CANVAS_WIDTH - 64, SPACE_CANVAS_HEIGHT * 4.0f / 5.0f}, {8, 14}, 96, 160, 5); // Bottom snake
}

Snakes::~Snakes()
{
    delete snakes[0];
    delete snakes[1];
    delete snakes[2];
}

void Snakes::update(GameScene *scene)
{
    if (is_dead) {
        frame_counter++;
        if (frame_counter >= death_time)
        {
            delete_entity = true;
            scene->nextLevel();
        }

        return;
    }
    for (size_t i = 0; i < 3; i++)
        snakes[i]->update(scene);
}

void Snakes::draw()
{
    for (size_t i = 0; i < 3; i++)
        snakes[i]->draw();
}

void Snakes::handleCollision(GameScene *scene, Entity *entity)
{
    int combined_health{0};
    for (size_t i = 0; i < 3; i++)
    {
        snakes[i]->handleCollision(scene, entity);
        combined_health += snakes[i]->health;
    }

    scene->setBossHealth(combined_health);

    if (combined_health <= 0)
        is_dead = true;
}

bool Snakes::checkCollision(const Entity &entity) const
{
    for (size_t i = 0; i < 3; i++)
        if (snakes[i]->checkCollision(entity))
            return true;

    return false;
}