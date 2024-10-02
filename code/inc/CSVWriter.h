#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp> 
#include <string>

class CSVWriter
{
public:
    bool writeCSV(const tmx::Map& map, const std::string& outputFile);

private:
    void writeTileLayer(const tmx::TileLayer& tileLayer, std::ofstream& csvFile, const tmx::Vector2u& mapSize);
};