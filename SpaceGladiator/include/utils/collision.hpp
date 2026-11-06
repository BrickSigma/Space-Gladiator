#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <bitset>

#include "entities/entity.hpp"
#include "level_manager/level.hpp"

// Check if an entity is colliding with the tilemap
bool checkCollisionWithTilemap(const Entity &entity, const std::bitset<LEVEL_GRID_HEIGHT * LEVEL_GRID_WIDTH> &tilemap, int scroll_distance);

#endif // COLLISION_HPP