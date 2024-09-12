#include <iostream>
#include "tinytmx.hpp"
#include <string>
#include <fstream>

// Path to the .tmx map file
std::string mapFile = "/home/ronan/Documents/GitHub/Engine-BRAVO/code/example_maps/dungeon/dungeon_test.tmx";

int main(int argc, char * argv[]) {
    // Check if the file exists
    std::ifstream file(mapFile);
    if (!file) {
        std::cerr << "Error: File not found." << std::endl;
        return 1;
    }
    file.close();

    tinytmx::Map map;
    try {
        map.ParseFile(mapFile);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing file: " << e.what() << std::endl;
        return 1;
    }

    // Check if map properties are correctly loaded
    if (map.GetWidth() == 0 || map.GetHeight() == 0) {
        std::cerr << "Warning: Map width or height is zero. There might be an issue with the map file or parsing." << std::endl;
    }

    std::cout << "Map Info:" << std::endl;
    std::cout << "Width: " << map.GetWidth() << std::endl;
    std::cout << "Height: " << map.GetHeight() << std::endl;
    std::cout << "Tile Width: " << map.GetTileWidth() << std::endl;
    std::cout << "Tile Height: " << map.GetTileHeight() << std::endl;

    // Print layer names
    auto layers = map.GetLayers();
    if (layers.empty()) {
        std::cout << "No layers found." << std::endl;
    } else {
        for (const auto& layer : layers) {
            std::cout << "Layer: " << layer->GetName() << std::endl;
        }
    }

    // Print tileset names
    auto tilesets = map.GetTilesets();
    if (tilesets.empty()) {
        std::cout << "No tilesets found." << std::endl;
    } else {
        for (const auto& tileset : tilesets) {
            std::cout << "Tileset: " << tileset->GetName() << std::endl;
        }
    }

    return 0;
}
