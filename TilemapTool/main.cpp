#include <raylib.h>

#include <fstream>
#include <iostream>

#define TILEMAP_WIDTH (208)
#define TILEMAP_HEIGHT (13)

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Not enough command line arguments were passed!\n";
        std::cerr << "Usage:\n\tTilemapTool [level_no]\n";
        return -1;
    }

    Image tilemap_image = LoadImage(TextFormat("tilemaps/level-%s-tilemap.png", argv[1]));

    std::ofstream file(TextFormat("assets/levels/level-%s-tilemap.txt", argv[1]));

    for (int h = 0; h < TILEMAP_HEIGHT; h++)
    {
        for (int w = 0; w < TILEMAP_WIDTH; w++)
        {
            Color color = GetImageColor(tilemap_image, w, h);
            if ((color.r || color.g || color.b) != 0)
            {
                file << 1;
            }
            else
            {
                file << 0;
            }
        }
        file << "\n";
    }

    UnloadImage(tilemap_image);
}