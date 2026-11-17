#ifndef CONFIG_HPP
#define CONFIG_HPP

#define CANVAS_WIDTH (256)
#define CANVAS_HEIGHT (224)

#define WINDOW_SCALE (3)

constexpr int TILE_SIZE{8};       // The size of a tile
constexpr int TILEMAP_WIDTH{416}; // The width of the entire tilemap/level
constexpr int TILEMAP_HEIGHT{26}; // The height of the entire tilemap/level

constexpr int WINDOW_WIDTH = CANVAS_WIDTH * WINDOW_SCALE;
constexpr int WINDOW_HEIGHT = CANVAS_HEIGHT * WINDOW_SCALE;

#endif // CONFIG_HPP