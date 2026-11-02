#include "levels/levels.hpp"
#include "entities/asteroid.hpp"

#include "scenes/game_scene.hpp"

// Used to get the size of a level
#define GET_LEVEL_SIZE(level) (sizeof(level) / sizeof(SpawnerData))

// Setup for Level 1
// =================================================================

static const SpawnerData level_1[] = {
    {1, spawnAsteroid, 16, static_cast<int>(AsteroidSize::MEDIUM)},
    {12, spawnAsteroid, SPACE_CANVAS_HEIGHT - 64, static_cast<int>(AsteroidSize::SMALL)},
    {16, spawnAsteroid, 64, static_cast<int>(AsteroidSize::MEDIUM)},
    {24, spawnAsteroid, SPACE_CANVAS_HEIGHT - 128, static_cast<int>(AsteroidSize::MEDIUM)},
    {24, spawnAsteroid, 100, static_cast<int>(AsteroidSize::MEDIUM)},
    {35, spawnAsteroid, 16, static_cast<int>(AsteroidSize::LARGE)},
    {40, spawnAsteroid, SPACE_CANVAS_HEIGHT - 64, static_cast<int>(AsteroidSize::SMALL)},
    {42, spawnAsteroid, 64, static_cast<int>(AsteroidSize::SMALL)},
    {50, spawnAsteroid, SPACE_CANVAS_HEIGHT - 128, static_cast<int>(AsteroidSize::MEDIUM)},
    {50, spawnAsteroid, 100, static_cast<int>(AsteroidSize::LARGE)},
    {55, spawnAsteroid, 16, static_cast<int>(AsteroidSize::MEDIUM)},
    {59, spawnAsteroid, SPACE_CANVAS_HEIGHT - 64, static_cast<int>(AsteroidSize::SMALL)},
    {62, spawnAsteroid, 64, static_cast<int>(AsteroidSize::MEDIUM)},
    {65, spawnAsteroid, SPACE_CANVAS_HEIGHT - 128, static_cast<int>(AsteroidSize::MEDIUM)},
    {65, spawnAsteroid, 100, static_cast<int>(AsteroidSize::MEDIUM)}};

static const LevelCheckpoints level_1_checkpoints = {
    {0, 50, 100, 150}};

// =================================================================

// Put all the levels into a single space
// =================================================================

static const SpawnerData *levels[] = {
    level_1};

static const LevelCheckpoints level_checkpoints[] = {
    level_1_checkpoints};

static const size_t level_sizes[] = {
    GET_LEVEL_SIZE(level_1)};

static const size_t no_levels = sizeof(level_sizes) / sizeof(size_t);

// =================================================================

LevelManager::LevelManager() {}
LevelManager::~LevelManager() {}

void LevelManager::loadLevel(size_t level_no)
{
    if (level_no >= no_levels)
    {
        TraceLog(LOG_FATAL, "Invalid level was trying to be loaded: %ld", level_no);
        return;
    }
    current_level = level_no;
    last_checkpoint = 0;
    next_spawner = 0;
    current_level_size = level_sizes[level_no];
}

void LevelManager::returnToCheckpoint(GameScene *scene)
{
    // Set the game scene's distance to match the checkpoint
    scene->distance = level_checkpoints[current_level].checkpoints[last_checkpoint];

    // Loop through the spawner data to figure out where to set the next_spawner value to
    for (size_t i = 0; i < current_level_size; i++)
    {
        if (levels[current_level][i].location >= scene->distance)
        {
            next_spawner = i;
            break;
        }
    }
}

void LevelManager::updateLevel(GameScene *scene)
{
    // Loop through the spawner data from the next_spawner index and keep loading entities until the location is greater than the current distance
    for (size_t i = next_spawner; i < current_level_size; i++)
    {
        // Exit the loop if the spawner is out of reach
        if (levels[current_level][i].location > scene->distance)
        {
            // Set the next_spawner value to this index
            next_spawner = i;
            break;
        }

        SpawnerData data = levels[current_level][i];
        data.cb(data, scene); // Call the spawner callback function

        // Incase we're at the last spawner, set the next_spawner to 1 above the level size to prevent an infinite loop
        if (i == current_level_size - 1)
        {
            next_spawner = current_level_size;
        }
    }

    // The last step is to update the checkpoint for the player, however we can skip this if the player may have died
    // if (scene->player->is_dead)
    // return;  // Though it's not a bad idea for a good bug honestly... I'll keep the check disabled for now

    // Also update the last checkpoint based on the distance
    LevelCheckpoints checkpoints = level_checkpoints[current_level];
    for (size_t i = MAX_CHECKPOINTS; i > 0; i--)
    {
        if (checkpoints.checkpoints[i - 1] <= scene->distance)
        {
            last_checkpoint = i - 1;
            break;
        }
    }
}