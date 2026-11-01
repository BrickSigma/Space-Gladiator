#include "scenes/game_scene.hpp"
#include <raylib.h>

#include "entities/asteroid.hpp"
#include "constants.hpp"

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

    // Some test asteroids
    Asteroid *asteroid = new Asteroid(AsteroidSize::LARGE, {CANVAS_WIDTH, CANVAS_HEIGHT / 2 - 64}, {-1, 0});
    entities.push_back(asteroid);
    asteroid = new Asteroid(AsteroidSize::MEDIUM, {CANVAS_WIDTH, CANVAS_HEIGHT / 2 - 16}, {-1, 0});
    entities.push_back(asteroid);
    asteroid = new Asteroid(AsteroidSize::SMALL, {CANVAS_WIDTH, CANVAS_HEIGHT / 2 + 64}, {-1, 0});
    entities.push_back(asteroid);
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
        if (entity == nullptr)
        {
            TraceLog(LOG_ERROR, "Entity is set to NULL! That should not happen!");
            continue;
        }
        entity->draw();
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
                break;  // Incase the entity was marked for deletion during this loop
            if (entity_2->delete_entity)
                continue; // Skip entities marked for deletion
            if (entity_1 == entity_2)
                continue; // Skip collision detection for the same entity

            entity_2->handleCollision(entity_1);
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

    // We can now push any entities that where spawned into the entities list
    entities.reserve(entities.size() + new_entities.size());
    entities.insert(entities.end(), new_entities.begin(), new_entities.end());
    new_entities.clear(); // Don't forget to clear the queue!

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