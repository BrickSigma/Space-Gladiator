#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <raylib.h>
#include <bitset>

#include "utils/constants.hpp"

class GameScene;

enum class EntityType
{
    PLAYER,            // The main player entity
    ENEMY,             // Used for any enemies
    PLAYER_PROJECTILE, // Used for any projectiles fired by the player
    ENEMY_PROJECTILE,  // Used for any projectiles fired by the enemy
    BLOCK,             // Used for any kind of block like a wall or baricade (blocks all projectiles, and will destroy the player if hit)
    NONE               // A blank entity, such as an explision which doesn't need any collision mechanics or physics
};

class Entity
{
public:
    // Create a new entity with a position, width, and height.
    Entity(EntityType type_, Vector2 pos_, int width_, int height_);
    // Create a new entity with all attributes set to 0
    Entity(EntityType type_);

    virtual ~Entity() = default;

    // Update loop for the entity
    virtual void update(GameScene *scene) = 0;

    // Draw the entity
    virtual void draw() = 0;

    // Used to check if this entity is colliding with another
    virtual bool checkCollision(const Entity &entity) const;

    // Used to handle collisions between entities
    virtual void handleCollision(GameScene *scene, Entity *other);

    // Check collision between the tileset
    virtual bool checkTilemapCollision(const std::bitset<TILEMAP_WIDTH * TILEMAP_HEIGHT> &tilemap, int scroll_distance) const;

    // Handle collisions between the tileset and entity
    virtual void handleTilemapCollision(GameScene *scene);

    // Check if the entity is off screen and returns true if it is.
    bool isOffScreen();

    Rectangle getRect() const;

    // Used to indicate if the entity should be deleted from the system
    bool delete_entity{false};

    EntityType type;
    Vector2 pos{0, 0};
    int width{0};
    int height{0};
    bool is_dead{false}; // Indicates whether the entity is dead or not
};

#endif // ENTITY_HPP