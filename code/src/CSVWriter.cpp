#include "CSVWriter.h"
#include <fstream>
#include <iostream>

bool CSVWriter::writeCSV(const tmx::Map& map, const std::string& outputFile)
{
    std::ofstream csvFile(outputFile);
    if (!csvFile.is_open())
    {
        std::cerr << "Failed to open CSV file." << std::endl;
        return false;
    }

    const auto& layers = map.getLayers();
    if (!layers.empty())
    {
        const auto& layer = layers[0]; // Change index or use a name to select a specific layer
        if (layer->getType() == tmx::Layer::Type::Tile)
        {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto& mapSize = map.getTileCount();
            writeTileLayer(tileLayer, csvFile, mapSize);
        }
    }

    csvFile.close();
    return true;
}

void CSVWriter::writeTileLayer(const tmx::TileLayer& tileLayer, std::ofstream& csvFile, const tmx::Vector2u& mapSize)
{
    const auto& tiles = tileLayer.getTiles();
    for (unsigned y = 0; y < mapSize.y; ++y)
    {
        for (unsigned x = 0; x < mapSize.x; ++x)
        {
            csvFile << tiles[y * mapSize.x + x].ID << ",";
        }
        csvFile << std::endl; // New line for each row
    }
}
