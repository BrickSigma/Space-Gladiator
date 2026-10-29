#ifndef TEXTURES_MANAGER_HPP
#define TEXTURES_MANAGER_HPP

#include <raylib.h>

class TextureManager
{
private:
    TextureManager(const TextureManager &) = delete;
    TextureManager &operator=(const TextureManager &) = delete;

    TextureManager();
    ~TextureManager();

public:
    // Get an instance of the singleton
    static TextureManager &getInstance();

    // Textures which are part of the texture manager
    
    Texture player_normal;
    Texture player_up;
    Texture player_down;
};

#endif // TEXTURES_MANAGER_HPP