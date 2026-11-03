#include "level_manager/level_manager.hpp"
#include "entities/asteroid.hpp"
#include "level_manager/level.hpp"

#include "scenes/game_scene.hpp"

LevelManager::LevelManager() {
    TraceLog(LOG_INFO, "Loading levels...");
    levels.push_back(Level(1));
    levels.push_back(Level(1));
}
LevelManager::~LevelManager() {}

void LevelManager::reset() {
    for (Level &level : levels) {
        level.reset();
    }

    current_level = 0;
}

bool LevelManager::nextLevel()
{
    if (current_level + 1 < levels.size())
    {
        return false; // We're at the last level
    }

    current_level++;
    return true;
}

void LevelManager::returnToCheckpoint(GameScene *scene)
{
    levels[current_level].returnToCheckpoint(scene);
}

void LevelManager::updateLevel(GameScene *scene)
{
    levels[current_level].update(scene);
}