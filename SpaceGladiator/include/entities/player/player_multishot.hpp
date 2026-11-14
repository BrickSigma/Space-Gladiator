#ifndef PLAYER_MULTISHOT_HPP
#define PLAYER_MULTISHOT_HPP

#include "entities/player/player_projectile.hpp"

constexpr float MULTISHOT_SPEED{2.121320344f}; // Speed of diagonal multishoot bullets

class PlayerMultishot : public PlayerProjectile
{
public:
    PlayerMultishot(Vector2 pos_, int direction);
    ~PlayerMultishot() = default;

    void draw() override;

private:
    Texture texture;
    int direction{1};
};

#endif // PLAYER_MULTISHOT_HPP