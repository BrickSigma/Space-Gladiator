#include "level_manager/level.hpp"

#include <fstream>
#include <format>
#include <iostream>

#include "level_manager/spawner.hpp"
#include "scenes/game_scene.hpp"

#define TYPE_STR_MAX_LEN (256)

Level::Level(unsigned int no)
{
    std::ifstream data_file{std::format("assets/levels/level_{}_data.txt", no)};

    if (!data_file)
        throw "Could not open level data file";

    std::string buffer{};

    // Read the checkpoints first
    // Skip the first line
    std::getline(data_file, buffer);
    std::cout << buffer << "\n";

    checkpoints.push_back(0); // Add zero as the default checkpoint
    while (std::getline(data_file, buffer))
    {
        if (buffer.length() == 0)
            break; // Exit the loop if we see an empty line

        checkpoints.push_back(std::atoi(buffer.c_str()));
    }

    std::getline(data_file, buffer);
    std::cout << buffer << "\n";
    while (std::getline(data_file, buffer))
    {
        int x, y, flags;
        char type_str[256];
        sscanf(buffer.c_str(), "%d,%d,%255[^,],%d", &x, &y, type_str, &flags);
        spawners.push_back({x, y, spawnerStringToType(type_str), flags});
    }

    background = LoadTexture(TextFormat("assets/levels/level_%d_background.png", no));
}

Level::~Level() {
    UnloadTexture(background);
}

void Level::returnToCheckpoint(GameScene *scene)
{
    // Set the game scene's distance to match the checkpoint
    scene->scroll_distance = checkpoints[last_checkpoint] * TILE_SIZE;

    // Loop through the spawner data to figure out where to set the next_spawner value to
    for (size_t i = 0; i < spawners.size(); i++)
    {
        if ((spawners[i].x_tile_pos * TILE_SIZE) >= (scene->scroll_distance + SPACE_CANVAS_WIDTH))
        {
            next_spawner = i;
            break;
        }
    }
}

void Level::update(GameScene *scene)
{
    // Loop through the spawner data from the next_spawner index and keep loading entities until the location is greater than the current distance
    for (size_t i = next_spawner; i < spawners.size(); i++)
    {
        // Exit the loop if the spawner is out of reach
        if ((spawners[i].x_tile_pos * TILE_SIZE) > (scene->scroll_distance + SPACE_CANVAS_WIDTH))
        {
            // Set the next_spawner value to this index
            next_spawner = i;
            break;
        }

        SpawnerData data = spawners[i];
        spawnerDispatch(data, scene); // Call the spawner callback function

        // Incase we're at the last spawner, set the next_spawner to 1 above the level size to prevent an infinite loop
        if (i == spawners.size() - 1)
        {
            next_spawner = spawners.size();
        }
    }

    // The last step is to update the checkpoint for the player, however we can skip this if the player may have died
    // if (scene->player->is_dead)
    // return;  // Though it's not a bad idea for a good bug honestly... I'll keep the check disabled for now

    // Also update the last checkpoint based on the distance
    for (size_t i = checkpoints.size(); i > 0; i--)
    {
        if ((checkpoints[i - 1] * TILE_SIZE) <= scene->scroll_distance)
        {
            last_checkpoint = i - 1;
            break;
        }
    }
}