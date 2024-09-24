#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

// Path to the .tmx map file
std::string mapFile = "code/example_maps/dungeon/dungeon_test.tmx";


int main()
{
    tmx::Map map;
    if (map.load(mapFile))
    {
        std::ofstream csvFile("output.csv");
        if (!csvFile.is_open())
        {
            std::cerr << "Failed to open CSV file." << std::endl;
            return 1;
        }

        // Find the specific layer by name or index
        const auto& layers = map.getLayers();
        if (!layers.empty())
        {
            const auto& layer = layers[0]; // Change index or use a name to select a specific layer
            if (layer->getType() == tmx::Layer::Type::Tile)
            {
                const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
                const auto& tiles = tileLayer.getTiles();
                const auto& mapSize = map.getTileCount();

                // Write tile IDs to CSV file, respecting the map's dimensions
                for (unsigned y = 0; y < mapSize.y; ++y)
                {
                    for (unsigned x = 0; x < mapSize.x; ++x)
                    {
                        csvFile << tiles[y * mapSize.x + x].ID << ",";
                    }
                    csvFile << std::endl; // New line for each row
                }
            }
        }

        csvFile.close();
        std::cout << "Tile IDs written to output.csv" << std::endl;
    }
    else
    {
        std::cerr << "Failed to load map." << std::endl;
    }

    return 0;
}