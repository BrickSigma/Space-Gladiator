#include "start_scene.hpp"
#include <raylib.h>

StartScene::StartScene() {}

StartScene::~StartScene() {}

SceneState StartScene::update(RenderTexture *screen)
{
    SceneState next_state = SceneState::START_SCREEN;

    // Handle user input
    if (IsKeyPressed(KEY_ENTER)) {
        next_state = SceneState::GAME;
    }

    // Handle rendering here
    BeginTextureMode(*screen);
    ClearBackground(BLACK);
    DrawText("Space Gladiators", 40, 40, 12, WHITE);
    DrawText("Start", 100, 100, 8, RED);
    EndTextureMode();

    return next_state;
}