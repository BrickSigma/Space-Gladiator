#include "scenes/game_scene.hpp"

#include "utils/constants.hpp"
#include "level_manager/level_manager.hpp"
#include "utils/texture_manager.hpp"

#include <algorithm>
#include <string>
#include <sstream>
#include <format>

GameScene::GameScene(RenderTexture *screen_) : Scene(screen_)
{
    canvas = LoadRenderTexture(SPACE_CANVAS_WIDTH, SPACE_CANVAS_HEIGHT);

    TextureManager &texture_manager = TextureManager::getInstance();
    hud_background = texture_manager.hud;
    shield_health_bar = texture_manager.shield_health_bar;
    boss_health_bar = texture_manager.boss_health_bar;

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

    stop_scrolling = false;

    boss_health = 16;

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
    TextureManager &texture_manager = TextureManager::getInstance();

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

    // Render the level tiles here when it's implemented
    level_manager.drawLevel(this);

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
        texture_manager.renderText("PAUSED", {SPACE_CANVAS_WIDTH / 2.0f - 24.0F, SPACE_CANVAS_HEIGHT / 2.0f - 4.0f});
    }
    else if (state == GameSceneState::PLAYER_KILLED && player.lives < 0)
    {
        texture_manager.renderText("GAME OVER", {SPACE_CANVAS_WIDTH / 2.0f - 36.0F, SPACE_CANVAS_HEIGHT / 2.0f - 4.0f});
    }

    EndTextureMode();

    // Update the actual screen here
    BeginTextureMode(*screen);
    ClearBackground(BLACK);
    DrawTextureRec(canvas.texture, {0, 0, SPACE_CANVAS_WIDTH, -SPACE_CANVAS_HEIGHT}, camera.offset, WHITE);

    // Drawing the distance for debugging
    std::string distance_str = std::to_string(scroll_distance);
    DrawText(distance_str.c_str(), 8, 0, 8, WHITE);
    std::string fps = std::to_string(GetFPS());
    DrawText(fps.c_str(), 16 * 14, 0, 8, WHITE);

    // HUD goes here
    DrawTexture(hud_background, 0, SPACE_CANVAS_HEIGHT, WHITE);

    // Render the player score and health to the HUD as well
    renderScoreAndHealth();

    // Draw the sheild health bar for the player
    for (int i = 0; i < player.shield; i++)
    {
        DrawTexture(shield_health_bar, 56 + 4 * i, SPACE_CANVAS_HEIGHT, WHITE);
    }

    // Draw the boss health bar
    for (int i = 0; i < boss_health; i++)
    {
        DrawTexture(boss_health_bar, 56 + 4 * i, SPACE_CANVAS_HEIGHT + 8, WHITE);
    }

    EndTextureMode();

    // We can now try handle collision detection between the entities.

    // The first case is checking each enemy-type entity against the player
    for (Entity *entity : entities)
    {
        if (entity->type == EntityType::PLAYER_PROJECTILE || entity->type == EntityType::NONE)
            continue; // Skip player bullets or explosions as they shouldn't harm a player

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

    // Update the frame counter
    frame_counter++;

    if (stop_scrolling)
    {
        if (IsKeyPressed(KEY_T))
        {
            nextLevel();
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
        // We need to check if we're coming from the PAUSED, or PLAYER_KILLED states
        switch (state)
        {
        case GameSceneState::PAUSED:
            state = next_state;
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

    case GameSceneState::PLAYER_KILLED:
        state = next_state;
        frame_counter = 0;
        if (player.lives < 0)
            player_killed_timer = GAME_OVER_TIME;
        else
            player_killed_timer = PLAYER_DEATH_TIME;
        break;

    case GameSceneState::COMPLETED:
        state = next_state;
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

void GameScene::addScore(unsigned int score_)
{
    score += score_;

    if (player.lives >= 9)
        return; // It's highly unlikely a player can get more than 450,000 points, but just in case, let's make sure the lives stay in single digits

    // Add an extra life to the player when > 30,000 points are reached
    if (score >= (player_lives_incremented + 1) * 30000)
    {
        player.lives++;
        player_lives_incremented++;
    }
}

void GameScene::setBossHealth(int health)
{
    boss_health = health;
}

void GameScene::nextLevel()
{
    if (state == GameSceneState::PLAYER_KILLED)
        return; // Don't go to the next level if the player died just after beating the boss

    // Try tansition to the next level if there is one
    if (level_manager.nextLevel())
    {
        stop_scrolling = false;
        scroll_distance = -SPACE_CANVAS_WIDTH;
        frame_counter = 0;
        boss_health = 16; // Make sure to set the boss level again
        // Also allow the player to reach the very top and bottom of the screen
        player.min_y_pos = 8.0f;
        player.max_y_pos = 8.0f;
    }
    else
    {
        pushStateEvent(GameSceneState::COMPLETED);
    }
}

void GameScene::renderScoreAndHealth() const
{
    TextureManager &texture_manager = TextureManager::getInstance();

    // Render the player's health first
    float lives_index = player.lives >= 0 ? static_cast<float>(player.lives) : 0.0f;
    DrawTextureRec(texture_manager.numbers, {8.0f * lives_index, 0, 8, 8}, {24, SPACE_CANVAS_HEIGHT + 4.0f}, WHITE);

    std::ostringstream number{};
    number << std::format("{:07}", score);

    // 160, SPACE_CANVAS_HEIGHT + 4

    std::string text = number.str();

    for (size_t i = 0; i < text.length(); i++)
    {
        float index = static_cast<float>(text[i] - '0');
        DrawTextureRec(texture_manager.numbers, {8.0f * index, 0, 8, 8}, {160.0f + 8.0f * static_cast<float>(i), SPACE_CANVAS_HEIGHT + 4.0f}, WHITE);
    }
}