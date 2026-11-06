#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "level_manager/spawner.hpp"

#include <raylib.h>
#include <bitset>
#include <vector>

#define TILE_SIZE (16)
#define LEVEL_GRID_WIDTH (208)  // Width of the level, where a grid is a 16x16 tile
#define LEVEL_GRID_HEIGHT (13)  // Height of the level, where a grid is a 16x16 tile
#define VISIBLE_GRID_WIDTH (16) // The visible number of tiles we can see on screen

class GameScene;

class Level
{
public:
    // Load a level and it's data
    Level(unsigned int no);
    ~Level();

    // Reset the level to start at the beginning again
    void reset();

    // Return the level to the last checkpoint reached
    void returnToCheckpoint(GameScene *scene);

    /**
     * Update the level by loading in the next set of entities using the spawners
     * and updating the checkpoint; all dependent on the current `scroll_distance` of the game scene.
     */
    void update(GameScene *scene);

    // List of checkpoints in the level
    std::vector<int> checkpoints{};
    // Index of the last checkpoint the player reached
    std::size_t last_checkpoint = 0;

    // List of all entitiy spawners
    std::vector<SpawnerData> spawners{};
    // Index of next spawner to load when reached
    std::size_t next_spawner = 0;

    std::bitset<LEVEL_GRID_HEIGHT * LEVEL_GRID_WIDTH> tilemap;

    // Background texture for the full level
    Texture background;
};

#endif // LEVEL_HPP