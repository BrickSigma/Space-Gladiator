#include "entities/entity.hpp"
#include "level_manager/level.hpp"

#include <cmath>

Entity::Entity(EntityType type_, Vector2 pos_, int width_, int height_) : type(type_), pos(pos_), width(width_), height(height_) {}

Entity::Entity(EntityType type_) : type(type_) {}

Rectangle Entity::getRect() const
{
    return Rectangle{pos.x, pos.y, static_cast<float>(width), static_cast<float>(height)};
}

bool Entity::checkCollision(const Entity &entity) const
{
    return CheckCollisionRecs(getRect(), entity.getRect());
}

void Entity::handleCollision(GameScene *scene, Entity *entity)
{
    // Does nothing by default
    (void)scene;
    (void)entity;
}

bool Entity::checkTilemapCollision(const std::bitset<TILEMAP_WIDTH * TILEMAP_HEIGHT> &tilemap, int scroll_distance) const
{
    int entity_x_pos = static_cast<int>(floor(pos.x)) + scroll_distance;
    int entity_y_pos = static_cast<int>(floor(pos.y));

    int grid_x_pos = entity_x_pos / TILE_SIZE;
    int grid_y_pos = entity_y_pos / TILE_SIZE;

    int grid_pos = scroll_distance / TILE_SIZE;
    int scroll_offset = scroll_distance % TILE_SIZE;

    int entity_grid_width = (width + TILE_SIZE) / TILE_SIZE;
    int entity_grid_height = (height + TILE_SIZE) / TILE_SIZE;

    Rectangle entity_rect{getRect()};

    for (int h = grid_y_pos; h <= grid_y_pos + entity_grid_height; h++)
    {
        if (h >= LEVEL_GRID_HEIGHT)
            continue;

        for (int w = grid_x_pos; w <= grid_x_pos + entity_grid_width; w++)
        {
            if (w >= LEVEL_GRID_WIDTH)
                continue; // Prevent a potential segfault when we reach the end of the level

            if (tilemap[static_cast<size_t>(h * LEVEL_GRID_WIDTH + w)])
            {
                Rectangle tile{static_cast<float>((w - grid_pos) * TILE_SIZE - scroll_offset), static_cast<float>(h * TILE_SIZE), (float)TILE_SIZE, (float)TILE_SIZE};
                if (CheckCollisionRecs(entity_rect, tile))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

void Entity::handleTilemapCollision(GameScene *scene)
{
    // Does nothing by default
    (void)scene;
}

bool Entity::isOffScreen()
{
    Rectangle screen{0, 0, CANVAS_WIDTH, CANVAS_HEIGHT};
    return !CheckCollisionRecs(screen, getRect());
}