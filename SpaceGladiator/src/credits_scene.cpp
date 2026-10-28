#include "credits_scene.hpp"
#include <raylib.h>

CreditsScene::CreditsScene(RenderTexture *screen) : Scene(screen) {}

CreditsScene::~CreditsScene() {}

SceneState CreditsScene::update()
{
    SceneState next_state = SceneState::NO_CHANGE;

    // Handle user input
    if (IsKeyPressed(KEY_BACKSPACE)) {
        next_state = SceneState::GAME;
    } else if (IsKeyPressed(KEY_ENTER)) {
        next_state = SceneState::START_SCREEN;
    }

    // Handle rendering here
    BeginTextureMode(*screen);
    ClearBackground(BLACK);
    DrawText("Credits", 40, 40, 12, WHITE);
    EndTextureMode();

    return next_state;
}