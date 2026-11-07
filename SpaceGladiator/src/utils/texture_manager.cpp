#include "utils/texture_manager.hpp"

static char *getImageAssetFile(const char *file) {
    const char *strings[] = {
        "assets/images/",
        file
    };
    return TextJoin(const_cast<char **>(strings), 2, "");
}

TextureManager &TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

TextureManager::TextureManager()
{
    TraceLog(LOG_INFO, "Creating texture manager instance");
    background = LoadTexture(getImageAssetFile("background.png"));

    // Player related textures
    player_normal = LoadTexture(getImageAssetFile("player-normal.png"));
    player_down = LoadTexture(getImageAssetFile("player-down.png"));
    player_up = LoadTexture(getImageAssetFile("player-up.png"));
    player_bullet = LoadTexture(getImageAssetFile("player-projectile.png"));

    // Asteroid textures
    asteroid_small = LoadTexture(getImageAssetFile("asteroid-small.png"));
    asteroid_medium = LoadTexture(getImageAssetFile("asteroid-medium.png"));
    asteroid_large = LoadTexture(getImageAssetFile("asteroid-large.png"));

    // Explosion frames
    explosion_1_frames = LoadTexture(getImageAssetFile("explosion-1.png"));
}

TextureManager::~TextureManager()
{
    TraceLog(LOG_INFO, "Destroying texture manager instance");

    UnloadTexture(background);

    // Player textures
    UnloadTexture(player_normal);
    UnloadTexture(player_down);
    UnloadTexture(player_up);
    UnloadTexture(player_bullet);

    // Asteroid textures
    UnloadTexture(asteroid_small);
    UnloadTexture(asteroid_medium);
    UnloadTexture(asteroid_large);

    // Explosion textures
    UnloadTexture(explosion_1_frames);
}