#include "game_scene.hpp"
#include <raylib.h>

GameScene::GameScene() {}

GameScene::~GameScene() {}

SceneState GameScene::update(RenderTexture *screen)
{
    SceneState next_state = SceneState::GAME;

    // Handle user input
    if (IsKeyPressed(KEY_BACKSPACE)) {
        next_state = SceneState::START_SCREEN;
    } else if (IsKeyPressed(KEY_ENTER)) {
        next_state = SceneState::CREDITS;
    }

    // Handle rendering here
    BeginTextureMode(*screen);
    ClearBackground(BLACK);
    DrawText("Main Game", 40, 40, 12, WHITE);
    EndTextureMode();

    return next_state;
}