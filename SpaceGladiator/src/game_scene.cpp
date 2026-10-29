#include "game_scene.hpp"
#include <raylib.h>

GameScene::GameScene(RenderTexture *screen) : Scene(screen)
{
    reset();
}

void GameScene::reset()
{
    distance = 0;
    score = 0;
    level = 1;
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

    // Reset the player and it to the entities list
    player = new Player();
    entities.push_back(player);
}

GameScene::~GameScene()
{
    // Delete all allocated entities
    for (Entity *entity : entities)
    {
        delete entity;
    }
}

SceneState GameScene::update()
{
    SceneState next_state = SceneState::NO_CHANGE;

    // Handle the entities update method
    for (Entity *entity : entities)
    {
        entity->update(this);
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

    // Handle rendering here
    BeginTextureMode(*screen);
    ClearBackground(BLACK);

    // Render all the entities
    for (Entity *entity : entities)
    {
        if (entity == nullptr) {
            TraceLog(LOG_ERROR, "Entity is set to NULL! That should not happen!");
            continue;
        }
        entity->draw();
    }
    EndTextureMode();

    return next_state;
}

void GameScene::onEnter(SceneState previous_state) {
    if (previous_state == SceneState::START_SCREEN) {
        // Reset the game if we're entering from the start screen
        reset();
    }
}

void GameScene::spawnEntity(Entity * entity) {
    new_entities.push_back(entity);
}