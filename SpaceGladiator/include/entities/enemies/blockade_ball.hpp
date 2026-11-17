#ifndef BLOCKADE_BALL_HPP
#define BLOCKADE_BALL_HPP

#include "entities/entity.hpp"

class BlockadeBall : public Entity
{
public:
    BlockadeBall(Vector2 pos_);
    ~BlockadeBall() = default;

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;

private:
    Texture texture;
    int frame_counter{0};
    const unsigned int score{50};
};

#endif  // BLOCKADE_BALL_HPP