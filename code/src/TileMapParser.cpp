#include "TileMapParser.h"

TileMapParser::TileMapParser(const std::string& aFilePath) : mFilePath(aFilePath) {}

void TileMapParser::parse() {
    std::ifstream file(mFilePath);
    if (file.is_open()) {
        file >> mJsonData;
        file.close();
    } else {
        throw std::runtime_error("Unable to open file: " + mFilePath);
    }

    // Extract tilesets
    for (const auto& tileset : mJsonData["tilesets"]) {
        mTilesets.push_back(tileset);
    }

    // Extract layers
    for (const auto& layer : mJsonData["layers"]) {
        if (layer["type"] == "tilelayer") {
            int width = layer["width"];
            int height = layer["height"];
            std::vector<std::vector<int>> grid(height, std::vector<int>(width));

            const auto& data = layer["data"];
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    grid[y][x] = data[y * width + x];
                }
            }
            mTileMapData.mLayers.push_back(grid);
            mLayerNames.push_back(layer["name"]);
        }
    }

    // Store tile information in a map
    storeTileInfo();
}

std::pair<int, int> TileMapParser::getTilePosition(int gID) const {
    for (const auto& tileset : mTilesets) {
        int firstGID = tileset["firstgid"];
        int tileCount = tileset["tilecount"];
        int columns = tileset["columns"];

        if (gID >= firstGID && gID < firstGID + tileCount) {
            int localID = gID - firstGID;
            int tileWidth = tileset["tilewidth"];
            int tileHeight = tileset["tileheight"];
            int x = (localID % columns) * tileWidth;
            int y = (localID / columns) * tileHeight;
            return {x, y};
        }
    }
    throw std::runtime_error("gID not found in any tileset");
}

void TileMapParser::storeTileInfo() {
    std::unordered_set<int> usedGIDs;

    // Get all used gIDs from the layers
    for (const auto& layer : mTileMapData.mLayers) {
        for (const auto& row : layer) {
            for (int gID : row) {
                if (gID != 0) { // Assuming 0 is an empty tile
                    usedGIDs.insert(gID);
                }
            }
        }
    }

    // Store tile information for the used gIDs
    for (int gID : usedGIDs) {
        for (const auto& tileset : mTilesets) {
            int firstGID = tileset["firstgid"];
            int tileCount = tileset["tilecount"];
            std::string tilesetName = tileset["image"];

            if (gID >= firstGID && gID < firstGID + tileCount) {
                auto position = getTilePosition(gID);
                mTileMapData.mTileInfoMap[gID] = {tilesetName, position};
                break;
            }
        }
    }
}

void TileMapParser::printTileInfo(int gID) const {
    auto it = mTileMapData.mTileInfoMap.find(gID);
    if (it != mTileMapData.mTileInfoMap.end()) {
        const TileInfo& info = it->second;
        std::cout << "gID: " << gID << ", Tileset: " << info.mTilesetName << ", Coordinates: (" << info.mCoordinates.first << ", " << info.mCoordinates.second << ")" << std::endl;
    } else {
        std::cerr << "gID " << gID << " not found in tile info map" << std::endl;
    }
}

void TileMapParser::printTileInfoMap() const {
    for (const auto& pair : mTileMapData.mTileInfoMap) {
        int gID = pair.first;
        const TileInfo& info = pair.second;
        std::cout << "gID: " << gID << ", Tileset: " << info.mTilesetName << ", Coordinates: (" << info.mCoordinates.first << ", " << info.mCoordinates.second << ")" << std::endl;
    }
}

void TileMapParser::printLayers() const {
    for (size_t layerIndex = 0; layerIndex < mTileMapData.mLayers.size(); ++layerIndex) {
        std::cout << "Layer " << layerIndex << " (" << mLayerNames[layerIndex] << "):\n";
        for (const auto& row : mTileMapData.mLayers[layerIndex]) {
            for (int tile : row) {
                std::cout << tile << " ";
            }
            std::cout << "\n";
        }
    }
}

int TileMapParser::getGIDFromCoordinate(int layer, int x, int y) const {
    if (layer >= mTileMapData.mLayers.size()) {
        throw std::out_of_range("Layer index out of range");
    }
    if (y >= mTileMapData.mLayers[layer].size() || x >= mTileMapData.mLayers[layer][y].size()) {
        throw std::out_of_range("Grid coordinates out of range");
    }
    return mTileMapData.mLayers[layer][y][x];
}

std::pair<int, int> TileMapParser::getGridTilePosition(int layerIndex, int x, int y) const {
    if (layerIndex >= mTileMapData.mLayers.size()) {
        throw std::out_of_range("Layer index out of range");
    }
    if (y >= mTileMapData.mLayers[layerIndex].size() || x >= mTileMapData.mLayers[layerIndex][y].size()) {
        throw std::out_of_range("Grid coordinates out of range");
    }
    int gID = mTileMapData.mLayers[layerIndex][y][x];
    return getTilePosition(gID);
}

const TileMapData& TileMapParser::getTileMapData() const {
    return mTileMapData;
}