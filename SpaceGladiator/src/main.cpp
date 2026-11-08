#include <raylib.h>
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "constants.hpp"
#include "scene_manager.hpp"
#include "scenes/start_scene.hpp"
#include "scenes/game_scene.hpp"
#include "scenes/credits_scene.hpp"
#include "utils/texture_manager.hpp"

#include <iostream>
#include <ctime>

// Global scene manager
SceneManager *scene_manager = nullptr;

RenderTexture window;

// Main gameloop function
void UpdateDrawFrame(void);

int main(int argc, char *argv[])
{
    // Used to ignore compiler errors of unused variables
    (void)argc;
    (void)argv;

#if defined(DEBUG)
    TraceLog(LOG_INFO, "Running in debug mode!");
#endif

    // Initialize the window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Space Gladiator");

    SetWindowState(FLAG_WINDOW_RESIZABLE);

#ifndef DEBUG
    SetExitKey(0);
#endif

    // Seed Raylib's random number generator
    SetRandomSeed(static_cast<unsigned long>(time(NULL)));

    // Initialize the texture manager
    TextureManager::getInstance();

    // Create another render texture which is linked to the actual window and used for scaling when in fullscreen mode
    window = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Render texture used to draw to
    RenderTexture screen = LoadRenderTexture(CANVAS_WIDTH, CANVAS_HEIGHT);

    StartScene *start = nullptr;
    GameScene *game = nullptr;
    CreditsScene *credits = nullptr;
    try
    {
        // Setup the main scene manager
        start = new StartScene(&screen);
        game = new GameScene(&screen);
        credits = new CreditsScene(&screen);
        scene_manager = new SceneManager(
            &screen,
            static_cast<Scene *>(start),
            static_cast<Scene *>(game),
            static_cast<Scene *>(credits));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        exit(-1);
    }
    catch (...)
    {
        std::cerr << "Unknown error caught!\n";
        exit(-1);
    }

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
    UnloadRenderTexture(window);

    CloseWindow();
    return 0;
}

void UpdateDrawFrame(void)
{
    try
    {
        scene_manager->update();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << "\n";
        exit(-1);
    }
    catch (...)
    {
        std::cerr << "Unknown error caught!\n";
        exit(-1);
    }

    // Handle rendering to the window.
    BeginTextureMode(window);
    scene_manager->draw();
    EndTextureMode();

#ifndef PLATFORM_WEB
    // Toggle the full screen state if the F-key is pressed
    if (IsKeyPressed(KEY_F))
    {
        ToggleFullscreen();
    }
#endif

    float width = static_cast<float>(GetRenderWidth());
    float height = static_cast<float>(GetRenderHeight());

    constexpr float aspect_ratio = static_cast<float>(CANVAS_WIDTH) / static_cast<float>(CANVAS_HEIGHT);

    float window_aspect_ratio = width / height;

    float drawn_width = width;
    float drawn_height = height;
    float x = 0; // X position of screen
    float y = 0; // Y position of screen

    if (window_aspect_ratio > aspect_ratio)
    {
        // Window width is greater than height, so we need to scale canvas width drawn
        drawn_height = height;                                 // Height stays as it is
        drawn_width = (height * WINDOW_WIDTH) / WINDOW_HEIGHT; // Width is scaled properly
        x = width / 2 - drawn_width / 2;
    }
    else if (window_aspect_ratio < aspect_ratio)
    {
        drawn_width = width;
        drawn_height = (width * WINDOW_HEIGHT) / WINDOW_WIDTH;
        y = height / 2 - drawn_height / 2;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(window.texture, {0, 0, static_cast<float>(WINDOW_WIDTH), static_cast<float>(-WINDOW_HEIGHT)}, {x, y, drawn_width, drawn_height}, {0, 0}, 0, WHITE);
    EndDrawing();
}