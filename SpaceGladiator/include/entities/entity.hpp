#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <raylib.h>

class GameScene;

enum class EntityType
{
    PLAYER,            // The main player entity
    ENEMY,             // Used for any enemies
    PLAYER_PROJECTILE, // Used for any projectiles fired by the player
    ENEMY_PROJECTILE,  // Used for any projectiles fired by the enemy
    BLOCK              // Used for any kind of block like a wall or baricade (blocks all projectiles, and will destroy the player if hit)
};

class Entity
{
public:
    // Create a new entity with a position, width, and height.
    Entity(EntityType type, Vector2 pos, int width, int height);
    virtual ~Entity() = default;

    // Update loop for the entity
    virtual void update(GameScene *scene) = 0;

    // Draw the entity
    virtual void draw() = 0;

    // Used to check if this entity is colliding with another
    bool checkCollision(Entity &entity);

    Rectangle getRect();

    // Used to indicate if the entity should be deleted from the system
    bool delete_entity = false;

    EntityType type;
protected:
    Vector2 pos;
    int width;
    int height;
};

#endif // ENTITY_HPP