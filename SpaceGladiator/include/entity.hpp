#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <raylib.h>

class GameScene;

enum class EntityType {
    PLAYER,
    ENEMY,
    PLAYER_PROJECTILE,
    ENEMY_PROJECTILE
};

class Entity
{
public:
    // Create a new entity with a position, width, and height. Also pass a reference to the game scene to it.
    Entity(EntityType type, Vector2 pos, int width, int height, GameScene *scene);
    virtual ~Entity() = default;

    // Update loop for the entity
    virtual void update() = 0;

    // Draw the entity
    virtual void draw() = 0;

    // Used to check if this entity is colliding with another
    bool checkCollision(Entity &entity);

    Rectangle getRect();

    // Used to indicate if the entity should be deleted from the system
    bool deleteEntity = false;

protected:
    EntityType type;
    Vector2 pos;
    int width;
    int height;
    
    // Internal pointer to the world
    GameScene *scene;
};

#endif // ENTITY_HPP