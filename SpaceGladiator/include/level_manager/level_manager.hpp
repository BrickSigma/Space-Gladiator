#ifndef LEVELS_HPP
#define LEVELS_HPP

#include <cstddef>
#include <vector>

#include "level_manager/level.hpp"

class GameScene;

/**
 * Level manager class:
 * 
 * Used to load all the levels into memory and handle their event loops,
 * drawing the transitions between levels, and making the tiling system accessible.
 */
class LevelManager
{
public:
    // Initialize the level manager
    LevelManager();
    ~LevelManager();

    // Used to reset the level manager and all the levels in it
    void reset();

    // Move to the next level. If we're at the last level, this will return `false`
    bool nextLevel();

    // Return the level back to the last checkpoint saved
    void returnToCheckpoint(GameScene *scene);

    // Used to update the level at the end of the game loop.
    // This will load any new entities based on the distance in the game scene, and update the checkpoint marker
    void updateLevel(GameScene *scene);

    // List of all the levels loaded into memory
    std::vector<Level> levels{};
    // The current level we're in
    size_t current_level = 0;
};

#endif // LEVELS_HPP