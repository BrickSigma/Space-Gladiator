#include "scenes/game_scene.hpp"

#include "entities/asteroid.hpp"
#include "utils/constants.hpp"
#include "level_manager/level_manager.hpp"
#include "utils/texture_manager.hpp"

#include <algorithm>
#include <string>

GameScene::GameScene(RenderTexture *screen_) : Scene(screen_)
{
    canvas = LoadRenderTexture(SPACE_CANVAS_WIDTH, SPACE_CANVAS_HEIGHT);
    reset();
}

void GameScene::reset()
{
    scroll_distance = 0;
    score = 0;
    frame_counter = 0;
    level_manager.reset();
    player.reset(true);
    player_killed_timer = PLAYER_DEATH_TIME;
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
    player.reset(false);

    // Load the level from the last checkpoint
    level_manager.returnToCheckpoint(this);

    frame_counter = 0;

    // Reset the game state
    state = GameSceneState::LEVEL;
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

void GameScene::updateEntities()
{
    // Update the player
    player.update(this);

    // Handle the entities update method
    for (Entity *entity : entities)
    {
        entity->update(this);
    }
}

SceneState GameScene::update()
{
    SceneState next_state = SceneState::NO_CHANGE;

    if (state == GameSceneState::COMPLETED)
        return SceneState::CREDITS;
    else if (state == GameSceneState::GAME_OVER)
        return SceneState::START_SCREEN;

    // Handle pausing the game
    if (IsKeyPressed(KEY_P) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT))
    {
        if (state == GameSceneState::LEVEL)
            pushStateEvent(GameSceneState::PAUSED);
        else if (state == GameSceneState::PAUSED)
            pushStateEvent(GameSceneState::LEVEL);
    }

    if (state != GameSceneState::PAUSED)
    {
        updateEntities();
    }

    // Handle user input
    if (IsKeyPressed(KEY_BACKSPACE))
        return SceneState::START_SCREEN;
    else if (IsKeyPressed(KEY_ENTER))
        return SceneState::CREDITS;

    // Handle rendering here for all entities and the level
    BeginTextureMode(canvas);
    ClearBackground(BLACK);

    if (state == GameSceneState::TRANSITION)
    {
        // Handle drawing cool transition background here
        ClearBackground(PURPLE);
    }
    else
    {
        // Render the level tiles here when it's implemented
        level_manager.drawLevel(this);
    }

    // Render all other entities
    for (Entity *entity : entities)
    {
        if (entity == nullptr)
        {
            TraceLog(LOG_ERROR, "Entity is set to NULL! That should not happen!");
            continue;
        }
        entity->draw();
    }

    // Player rendered on top of all entities
    player.draw();

    // Render any overlays
    if (state == GameSceneState::PAUSED)
    {
        int text_width = MeasureText("Paused", 8);
        DrawText("Paused", CANVAS_WIDTH / 2 - text_width / 2, SPACE_CANVAS_HEIGHT / 2 - 4, 8, WHITE);
    }
    else if (state == GameSceneState::PLAYER_KILLED && player.lives < 0)
    {
        int text_width = MeasureText("Game Over!", 8);
        DrawText("Game Over!", CANVAS_WIDTH / 2 - text_width / 2, SPACE_CANVAS_HEIGHT / 2 - 4, 8, WHITE);
    }

    EndTextureMode();

    // Update the actual screen here
    BeginTextureMode(*screen);
    ClearBackground(BLACK);
    DrawTextureRec(canvas.texture, {0, 0, SPACE_CANVAS_WIDTH, -SPACE_CANVAS_HEIGHT}, camera.offset, WHITE);

    // Render the current distance for debugging
    std::string distance_str = std::to_string(scroll_distance);
    DrawText(distance_str.c_str(), 8, SPACE_CANVAS_HEIGHT + 4, 12, WHITE);

    EndTextureMode();

    // We can now try handle collision detection between the entities.

    // The first case is checking each enemy-type entity against the player
    for (Entity *entity : entities)
    {
        if (entity->type == EntityType::PLAYER_PROJECTILE)
            continue; // Skip player bullets as they shouldn't harm a player

        player.handleCollision(this, entity);
    }

    player.handleTilemapCollision(this);

    // The next case is handling all enemies against the player bullets
    for (Entity *entity_1 : entities)
    {
        // Check if the entity has collided with the tilemap first
        entity_1->handleTilemapCollision(this);

        // If the player did hit the tilemap, or was deleted by another entity, skip it
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
            entities.erase(entities.begin() + static_cast<long int>(i - 1));
            delete entity; // Free the allocated memory for the entity
        }
    }

    // Update any timers or state-specific counters
    if (state == GameSceneState::LEVEL || state == GameSceneState::PLAYER_KILLED)
    {
        if (!stop_scrolling)
        {
            if (frame_counter % DISTANCE_UPDATE_DURATION == 0)
                scroll_distance++;

            if (scroll_distance >= (LEVEL_GRID_WIDTH * TILE_SIZE - SPACE_CANVAS_WIDTH))
            {
                stop_scrolling = true; // Stop scrolling when we hit the end of the level
                scroll_distance = (LEVEL_GRID_WIDTH * TILE_SIZE - SPACE_CANVAS_WIDTH);
            }

            // Update the level so new entities can spawn in as we scroll
            level_manager.updateLevel(this);
        }

        if (state == GameSceneState::PLAYER_KILLED)
            if (frame_counter >= player_killed_timer)
                pushStateEvent(GameSceneState::LEVEL);
    }

    if (state == GameSceneState::TRANSITION)
        if (frame_counter >= transition_duration)
            pushStateEvent(GameSceneState::LEVEL);

    // Update the frame counter
    frame_counter++;

    if (stop_scrolling)
    {
        if (IsKeyPressed(KEY_T))
        {
            pushStateEvent(GameSceneState::TRANSITION);
        }
    }

    // We can now push any entities that where spawned into the entities list
    entities.reserve(entities.size() + new_entities.size());
    entities.insert(entities.end(), new_entities.begin(), new_entities.end());
    new_entities.clear(); // Don't forget to clear the queue!

    // Update the camera at the end
    camera.update();

    // Update the scene state
    handleStateChange();

    return next_state;
}

void GameScene::onEnter(SceneState previous_state)
{
    if (previous_state != SceneState::GAME)
    {
        // Reset the game if we're entering from the start screen
        reset();
    }
}

void GameScene::spawnEntity(Entity *entity)
{
    new_entities.push_back(entity);
}

void GameScene::pushStateEvent(GameSceneState new_state)
{
    state_event_queue.push_back(new_state);
}

static bool state_cmp(const GameSceneState &lhs, const GameSceneState &rhs)
{
    return static_cast<int>(lhs) < static_cast<int>(rhs);
}

void GameScene::handleStateChange()
{
    if (state_event_queue.size() == 0)
        return;

    // Sort the event queue first
    std::sort(state_event_queue.begin(), state_event_queue.end(), state_cmp);

    // We only care about the event at the very beginning of the queue, as it has the highest priority
    GameSceneState next_state = state_event_queue[0];

    // Now we can use a transition table to help move between the scenes
    switch (next_state)
    {
    case GameSceneState::PAUSED:
        // No need to check if the previous state was a level since it's done earlier on
        state = next_state;
        break;
    case GameSceneState::LEVEL:
    {
        // We need to check if we're coming from the PAUSED, TRANSITION, or PLAYER_KILLED states
        switch (state)
        {
        case GameSceneState::PAUSED:
            state = next_state;
            break;
        case GameSceneState::TRANSITION:
            state = next_state;
            frame_counter = 0;
            break;
        case GameSceneState::PLAYER_KILLED:
            stop_scrolling = false; // Make sure to enable scrolling again
            if (player.lives < 0)
            {
                // reset(); // Reset the entire game if the player completely died
                // Go back to the start screen
                state = GameSceneState::GAME_OVER;
            }
            else
            {
                resetFromCheckpoint(); // Reset the game from the player's last checkpoint
            }
            break;
        default:
            break;
        }
        break;
    }

    case GameSceneState::TRANSITION:
    {
        // Only move to the transition state if the player hasn't been killed
        if (state == GameSceneState::LEVEL)
        {
            // Try tansition to the next level if there is one
            if (level_manager.nextLevel())
            {
                stop_scrolling = false;
                scroll_distance = 0;
                frame_counter = 0;
                state = next_state;
            }
            else
            {
                state = GameSceneState::COMPLETED;
            }
        }
        break;
    }

    case GameSceneState::PLAYER_KILLED:
        state = next_state;
        frame_counter = 0;
        if (player.lives < 0)
            player_killed_timer = GAME_OVER_TIME;
        else
            player_killed_timer = PLAYER_DEATH_TIME;
        break;

    default:
        break;
    }

    // Once done, clear the event queue so we don't get an infinite loop of events
    state_event_queue.clear();
}

void GameScene::setCameraShake(CameraShake camera_shake)
{
    this->camera = camera_shake;
}