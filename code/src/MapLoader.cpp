#include "MapLoader.h"

bool MapLoader::loadMap(const std::string& mapFile)
{
    return map.load(mapFile);
}

const tmx::Map& MapLoader::getMap() const
{
    return map;
}