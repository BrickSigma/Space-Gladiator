#ifndef LEVELS_HPP
#define LEVELS_HPP

#include "levels/spawner.hpp"
#include <cstddef>

#define GRID_SIZE (16)
#define LEVEL_GRID_WIDTH (208) // Width of the level, where a grid is a 16x16 tile
#define LEVEL_GRID_HEIGHT (13) // Height of the level, where a grid is a 16x16 tile
#define MAX_CHECKPOINTS (4)

class GameScene;

// Used to indicate all the checkpoints in a level
typedef struct LevelCheckpoints
{
    unsigned long checkpoints[MAX_CHECKPOINTS];
} LevelCheckpoints;

// Class used to keep track of the level we're in and managing it
class LevelManager
{
public:
    // Initialize the level manager
    LevelManager();
    ~LevelManager();

    // Load a specified level, this also resets the level to the beginning
    void loadLevel(size_t level_no);

    // Return the level back to the last checkpoint saved
    void returnToCheckpoint(GameScene *scene);

    // Used to update the level at the end of the game loop.
    // This will load any new entities based on the distance in the game scene, and update the checkpoint marker
    void updateLevel(GameScene *scene);

    // The current level we're in
    size_t current_level = 0;
    // Last checkpoint reached (between 0 and 3)
    size_t last_checkpoint = 0;
    // Index of next spawner to check in the level data
    size_t next_spawner = 0;
    // The size of the level
    size_t current_level_size = 0;
};

#endif // LEVELS_HPP