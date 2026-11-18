#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "level_manager/spawner.hpp"

#include <raylib.h>
#include <bitset>
#include <vector>

#include "utils/constants.hpp"
#include "utils/audio_manager.hpp"

constexpr int LEVEL_GRID_WIDTH{TILEMAP_WIDTH};   // Width of the level, where a grid is a 16x16 tile
constexpr int LEVEL_GRID_HEIGHT{TILEMAP_HEIGHT}; // Height of the level, where a grid is a 16x16 tile
constexpr int VISIBLE_GRID_WIDTH{32};            // The visible number of tiles we can see on screen

class GameScene;

class Level
{
public:
    // Load a level and it's data
    Level(unsigned int level_no_);
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

    // Start the music for the level
    void startMusic() const;

    // Start the boss music for the level
    void startBossMusic() const;

    unsigned int level_no{1}; // The level number

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

    MusicFile music;
    MusicFile boss_music;
};

#endif // LEVEL_HPP