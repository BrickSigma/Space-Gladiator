#include <raylib.h>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "constants.hpp"
#include "scene_manager.hpp"
#include "start_scene.hpp"
#include "game_scene.hpp"
#include "credits_scene.hpp"
#include "texture_manager.hpp"

// Global scene manager
SceneManager *scene_manager = nullptr;

// Main gameloop function
void UpdateDrawFrame(void);

int main(int argc, char *argv[])
{
    // Used to ignore compiler errors of unused variables
    (void)argc;
    (void)argv;

    // Initialize the window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Gladiator");

    // Initialize the texture manager
    TextureManager::getInstance();

    // Render texture used to draw to
    RenderTexture screen = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);

    // Setup the main scene manager
    StartScene *start = new StartScene(&screen);
    GameScene *game = new GameScene(&screen);
    CreditsScene *credits = new CreditsScene(&screen);
    scene_manager = new SceneManager(
        &screen,
        static_cast<Scene *>(start),
        static_cast<Scene *>(game),
        static_cast<Scene *>(credits));

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
#endif

    // Free the scene manager
    delete scene_manager;
    // Free all of the other scenes
    delete start;
    delete game;
    delete credits;

    // Free the render texture
    UnloadRenderTexture(screen);

    CloseWindow();
    return 0;
}

void UpdateDrawFrame(void)
{
    scene_manager->update();

    BeginDrawing();
    DrawTexturePro(scene_manager->screen->texture, {0, 0, CANVAS_WIDTH, -CANVAS_HEIGHT}, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, {0, 0}, 0, WHITE);
    EndDrawing();
}