#include "utils/collision.hpp"

#include <cmath>

bool checkCollisionWithTilemap(const Entity &entity, const std::bitset<LEVEL_GRID_HEIGHT * LEVEL_GRID_WIDTH> &tilemap, int scroll_distance)
{
    int entity_x_pos = static_cast<int>(floor(entity.pos.x)) + scroll_distance;
    int entity_y_pos = static_cast<int>(floor(entity.pos.y));

    int grid_x_pos = entity_x_pos / TILE_SIZE;
    int grid_y_pos = entity_y_pos / TILE_SIZE;

    int grid_pos = scroll_distance / TILE_SIZE;
    int scroll_offset = scroll_distance % TILE_SIZE;

    int entity_grid_width = (entity.width + TILE_SIZE) / TILE_SIZE;
    int entity_grid_height = (entity.height + TILE_SIZE) / TILE_SIZE;

    Rectangle entity_rect{entity.getRect()};

    for (int h = grid_y_pos; h <= grid_y_pos + entity_grid_height; h++)
    {
        if (h >= LEVEL_GRID_HEIGHT)
            continue;

        for (int w = grid_x_pos; w <= grid_x_pos + entity_grid_width; w++)
        {
            if (w >= LEVEL_GRID_WIDTH)
                continue; // Prevent a potential segfault when we reach the end of the level

            if (tilemap[h * LEVEL_GRID_WIDTH + w])
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