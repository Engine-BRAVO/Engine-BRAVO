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

    if (mJsonData.empty()) {
        throw std::runtime_error("File is empty: " + mFilePath);
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
        else if (layer["type"] == "objectgroup") {
            parseObjectLayer(layer);
        }
    }

    // Store tile information in a map
    storeTileInfo();
}

void TileMapParser::parseObjectLayer(const nlohmann::json& layer) {
    for (const auto& object : layer["objects"]) {
        SpawnPoint spawnPoint;
        spawnPoint.x = object["x"];
        spawnPoint.y = object["y"];
        if (object.contains("properties")) {
            for (const auto& property : object["properties"]) {
                if (property["name"] == "isPlayerSpawn" && property["type"] == "bool") {
                    spawnPoint.isPlayerSpawn = property["value"];
                }
            }
        }
        mTileMapData.mSpawnPoints.push_back(spawnPoint);
    }
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

    // Collect all used gIDs from the layers
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
                TileInfo tileInfo = {tilesetName, position, {}};

                // Check for colliders
                int localID = gID - firstGID;
                if (tileset.contains("tiles")) {
                    for (const auto& tile : tileset["tiles"]) {
                        if (tile["id"] == localID && tile.contains("objectgroup")) {
                            for (const auto& object : tile["objectgroup"]["objects"]) {
                                try {
                                    ColliderData collider = {
                                        object.at("x").get<float>(),
                                        object.at("y").get<float>(),
                                        object.at("width").get<float>(),
                                        object.at("height").get<float>()
                                    };
                                    tileInfo.mColliders.push_back(collider);
                                    // Debug print to verify collider parsing
                                    std::cout << "Added collider to gID " << gID << ": x=" << collider.x << ", y=" << collider.y << ", width=" << collider.mWidth << ", height=" << collider.mHeight << std::endl;
                                } catch (const nlohmann::json::type_error& e) {
                                    std::cerr << "Error parsing collider for gID " << gID << ": " << e.what() << std::endl;
                                }
                            }
                        }
                    }
                }

                mTileMapData.mTileInfoMap[gID] = tileInfo;
                break;
            }
        }
    }
}

const TileMapData& TileMapParser::getTileMapData() const {
    return mTileMapData;
}