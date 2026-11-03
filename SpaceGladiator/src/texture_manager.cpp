#include "texture_manager.hpp"

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
    player_normal = LoadTexture(getImageAssetFile("player-normal.png"));
    player_down = LoadTexture(getImageAssetFile("player-down.png"));
    player_up = LoadTexture(getImageAssetFile("player-up.png"));
    player_bullet = LoadTexture(getImageAssetFile("player-projectile.png"));
}

TextureManager::~TextureManager()
{
    TraceLog(LOG_INFO, "Destroying texture manager instance");
    UnloadTexture(background);
    UnloadTexture(player_normal);
    UnloadTexture(player_down);
    UnloadTexture(player_up);
    UnloadTexture(player_bullet);
}