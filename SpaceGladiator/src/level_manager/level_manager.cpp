#include "level_manager/level_manager.hpp"
#include "level_manager/level.hpp"

#include "scenes/game_scene.hpp"

#include <cmath>

LevelManager::LevelManager()
{
    TraceLog(LOG_INFO, "Loading levels...");
    levels.push_back(new Level(1));
    levels.push_back(new Level(1));
}

LevelManager::~LevelManager()
{
    for (Level *level : levels)
    {
        delete level;
    }
}

void LevelManager::reset()
{
    for (Level *level : levels)
    {
        level->reset();
    }

    current_level = 0;
}

bool LevelManager::nextLevel()
{
    if (current_level + 1 >= levels.size())
    {
        return false; // We're at the last level
    }

    current_level++;
    return true;
}

void LevelManager::returnToCheckpoint(GameScene *scene)
{
    levels[current_level]->returnToCheckpoint(scene);
}

void LevelManager::updateLevel(GameScene *scene)
{
    levels[current_level]->update(scene);
}

void LevelManager::drawLevel(GameScene *scene)
{
    DrawTexture(levels[current_level]->background, -scene->scroll_distance, 0, WHITE);

    // Tile map space test
    /**

    int player_x_pos = static_cast<int>(floor(scene->player.pos.x)) + scene->scroll_distance;
    int player_y_pos = static_cast<int>(floor(scene->player.pos.y));

    int grid_x_pos = player_x_pos / TILE_SIZE;
    int grid_y_pos = player_y_pos / TILE_SIZE;

    int grid_pos = scene->scroll_distance/TILE_SIZE;
    int scroll_offset = scene->scroll_distance % TILE_SIZE;

    for (int h = grid_y_pos; h <= grid_y_pos + 1; h++)
    {
        if (h >= LEVEL_GRID_HEIGHT)
        continue;

        for (int w = grid_x_pos; w <= grid_x_pos + 1; w++)
        {
            if (w >= LEVEL_GRID_WIDTH)
            continue; // Prevent a potential segfault when we reach the end of the level
            if (levels[current_level]->tilemap[h * LEVEL_GRID_WIDTH + w])
            DrawRectangle((w - grid_pos) * TILE_SIZE - scroll_offset, h * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
        }
    }
    */
}

std::bitset<LEVEL_GRID_WIDTH * LEVEL_GRID_HEIGHT> &LevelManager::getTilemap() const
{
    return levels[current_level]->tilemap;
}