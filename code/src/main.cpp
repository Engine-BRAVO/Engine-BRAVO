#include <iostream>
#include <fstream>
#include <vector>
#include "tinytmx.hpp"
#include <string>
#include <filesystem>

// Path to the .tmx map file
std::string mapFile = "code/example_maps/sample/sample.tmx";

// Function to read finite map tile data into a 2D array
std::vector<std::vector<int>> ReadLayerInto2DArray(const tinytmx::TileLayer* tileLayer, int width, int height) {
    
    std::vector<std::vector<int>> tileArray(height, std::vector<int>(width));

    // Retrieve the finite map data
    const tinytmx::DataChunkTile* dataTileFiniteMap = tileLayer->GetDataTileFiniteMap();
    if (!dataTileFiniteMap) {
        std::cerr << "Error: Finite map data not found." << std::endl;
        return tileArray;
    }

    // Fill the 2D array with tile IDs
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const tinytmx::MapTile& tile = dataTileFiniteMap->GetTile(x, y);  // Get tile at (x, y)
            int tileID = tile.gid;  // Get the GID of the tile
            tileArray[y][x] = tileID;
        }
    }

    return tileArray;
}

// Function to write the 2D array to a CSV file
void WriteArrayToCSV(const std::vector<std::vector<int>>& array, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }

    int height = array.size();
    int width = (height > 0) ? array[0].size() : 0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            file << array[y][x];
            if (x < width - 1) file << ",";  // Add a comma between values
        }
        file << "\n";  // Newline at the end of each row
    }

    file.close();
}

int main(int argc, char *argv[]) {
    // Initialize the map object
    tinytmx::Map map;

    // Parse the map file and check for errors
    map.ParseFile(mapFile);
    if (map.HasError()) {  // Check if there was an error during parsing
        std::cerr << "Error: Failed to load map file. Error: " << map.GetErrorText() << std::endl;
        return 1;
    }

    // Retrieve map dimensions
    int mapWidth = map.GetWidth();
    int mapHeight = map.GetHeight();
    std::cout << "Map Width: " << mapWidth << ", Height: " << mapHeight << std::endl;

    // Retrieve the first layer (you can loop through all layers if needed)
    const auto& layers = map.GetLayers();
    if (layers.empty()) {
        std::cerr << "No layers found in the map." << std::endl;
        return 1;
    }

    const tinytmx::Layer* layer = layers[0];  // Assume the first layer for this example

    // Cast to TileLayer
    const tinytmx::TileLayer* tileLayer = dynamic_cast<const tinytmx::TileLayer*>(layer);
    if (!tileLayer) {
        std::cerr << "Error: Layer is not a TileLayer." << std::endl;
        return 1;
    }

    std::cout << "Reading Layer: " << layer->GetName() << std::endl;

    // Read the tile layer into a 2D array
    std::vector<std::vector<int>> tileArray = ReadLayerInto2DArray(tileLayer, mapWidth, mapHeight);

    // Write the 2D array to a CSV file
    std::string outputFilename = "tile_data.csv";
    WriteArrayToCSV(tileArray, outputFilename);

    std::cout << "Tile data has been written to " << outputFilename << std::endl;

    return 0;
}
