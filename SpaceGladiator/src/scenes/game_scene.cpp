#include "scenes/game_scene.hpp"

#include "entities/asteroid.hpp"
#include "constants.hpp"
#include "level_manager/level_manager.hpp"
#include "texture_manager.hpp"

#include <string>

GameScene::GameScene(RenderTexture *screen) : Scene(screen)
{
    canvas = LoadRenderTexture(SPACE_CANVAS_WIDTH, SPACE_CANVAS_HEIGHT);
    reset();
}

void GameScene::reset()
{
    scroll_distance = 0;
    score = 0;
    level_manager.reset();
    resetFromCheckpoint();
}

void GameScene::resetFromCheckpoint()
{
    // When restarting from a checkpoint, clear out all the previous entities first
    // and reload the correct ones in
    for (Entity *entity : entities)
    {
        delete entity;
    }
    entities.clear();

    // Reset the player and add it to the entities list
    player = new Player();
    entities.push_back(player);

    // Load the level from the last checkpoint
    level_manager.returnToCheckpoint(this);
}

GameScene::~GameScene()
{
    // Delete all allocated entities
    for (Entity *entity : entities)
    {
        delete entity;
    }

    UnloadRenderTexture(canvas);
}

SceneState GameScene::update()
{
    SceneState next_state = SceneState::NO_CHANGE;

    if (IsKeyPressed(KEY_P))
    {
        is_paused = !is_paused;
    }

    if (is_paused)
    {
        goto game_scene_render; // Ohh, scary GOTO statement. This shouldn't be an issue though
    }

    // Handle user input
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        next_state = SceneState::START_SCREEN;
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        next_state = SceneState::CREDITS;
    }

    // Handle the entities update method
    for (Entity *entity : entities)
    {
        entity->update(this);
    }

game_scene_render:

    // TextureManager &texture_manager = TextureManager::getInstance();

    // Handle rendering here for all entities and the level
    BeginTextureMode(canvas);
    ClearBackground(BLACK);

    // Render the scrolling background
    // DrawTexture(texture_manager.background, -(scroll_distance % 256), 0, WHITE);
    // DrawTexture(texture_manager.background, SPACE_CANVAS_WIDTH - (scroll_distance % 256), 0, WHITE);

    // Render the level tiles here when it's implemented
    level_manager.drawLevel(this);

    // Render all the entities
    for (Entity *entity : entities)
    {
        if (entity == nullptr)
        {
            TraceLog(LOG_ERROR, "Entity is set to NULL! That should not happen!");
            continue;
        }
        entity->draw();
    }
    EndTextureMode();

    // Update the actual screen here
    BeginTextureMode(*screen);
    ClearBackground(WHITE);
    DrawTextureRec(canvas.texture, {0, 0, SPACE_CANVAS_WIDTH, -SPACE_CANVAS_HEIGHT}, {0, 0}, WHITE);

    // Render the current distance for debugging
    std::string distance_str = std::to_string(scroll_distance);
    DrawText(distance_str.c_str(), 8, SPACE_CANVAS_HEIGHT + 4, 12, BLACK);

    if (is_paused)
    {
        int text_width = MeasureText("Game Paused", 8);
        DrawText("Game Paused", CANVAS_WIDTH / 2 - text_width / 2, CANVAS_HEIGHT / 2 - 4, 8, WHITE);
    }

    EndTextureMode();

    // We can now try handle collision detection between the entities.
    for (Entity *entity_1 : entities)
    {
        if (entity_1->delete_entity)
            continue; // Skip entities marked for deletion

        for (Entity *entity_2 : entities)
        {
            if (entity_1->delete_entity)
                break; // Incase the entity was marked for deletion during this loop
            if (entity_2->delete_entity)
                continue; // Skip entities marked for deletion
            if (entity_1 == entity_2)
                continue; // Skip collision detection for the same entity

            entity_2->handleCollision(this, entity_1);
        }
    }

    // Once done with rendering, let's remove any entities marked for deletion
    for (size_t i = entities.size(); i > 0; i--)
    {
        Entity *entity = entities[i - 1];
        if (entity->delete_entity)
        {
            entities.erase(entities.begin() + (i - 1));
            delete entity; // Free the allocated memory for the entity
        }
    }

    // Update the distance as well
    if (!is_paused && !stop_scrolling)
    {
        if (frame_counter % DISTANCE_UPDATE_DURATION == 0)
            scroll_distance++;
        frame_counter++;

        if (scroll_distance >= (LEVEL_GRID_WIDTH * TILE_SIZE - SPACE_CANVAS_WIDTH))
        {
            stop_scrolling = true; // Stop scrolling when we hit the end of the level
        }
    }

    if (stop_scrolling)
    {
        if (IsKeyPressed(KEY_T))
        {
            transitionToNextLevel();
        }
    }

    // Update the level as well so new entities can spawn in
    level_manager.updateLevel(this);

    // We can now push any entities that where spawned into the entities list
    entities.reserve(entities.size() + new_entities.size());
    entities.insert(entities.end(), new_entities.begin(), new_entities.end());
    new_entities.clear(); // Don't forget to clear the queue!

    // If the player died and their death timer is 0, reset the level to the checkpoint
    if (player->is_dead && player->death_timer == 0)
    {
        resetFromCheckpoint();
    }

    return next_state;
}

void GameScene::onEnter(SceneState previous_state)
{
    if (previous_state == SceneState::START_SCREEN)
    {
        // Reset the game if we're entering from the start screen
        reset();
    }
}

void GameScene::spawnEntity(Entity *entity)
{
    new_entities.push_back(entity);
}

void GameScene::transitionToNextLevel()
{
    if (level_manager.nextLevel())
    {
        scroll_distance = -SPACE_CANVAS_WIDTH;
        stop_scrolling = false;
    }
}