#include <raylib.h>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "constants.hpp"
#include "scene_manager.hpp"
#include "start_scene.hpp"

// Global scene manager
SceneManager *scene_manager = nullptr;

// Main gameloop function
void UpdateDrawFrame(void);

int main(int argc, char *argv[])
{
    // Initialize the window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Gladiator");

    // Render texture used to draw to
    RenderTexture screen = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);

    // Setup the main scene manager
    StartScene *start = new StartScene();
    scene_manager = new SceneManager(&screen, static_cast<Scene *>(start), nullptr, nullptr);

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