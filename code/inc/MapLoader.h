#pragma once

#include <tmxlite/Map.hpp>
#include <string>

class MapLoader
{
public:
    bool loadMap(const std::string& mapFile);
    const tmx::Map& getMap() const;

private:
    tmx::Map map;
};