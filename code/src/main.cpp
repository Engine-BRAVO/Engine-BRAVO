#include <iostream>
#include "MapLoader.h"
#include "CSVWriter.h"
#include "MapRenderer.h"

int main()
{
    std::string mapFile = "code/example_maps/jb-32.tmx";

    MapLoader mapLoader;
    if (!mapLoader.loadMap(mapFile))
    {
        std::cerr << "Failed to load map." << std::endl;
        return 1;
    }

    CSVWriter csvWriter;
    if (!csvWriter.writeCSV(mapLoader.getMap(), "output.csv"))
    {
        return 1;
    }

    MapRenderer mapRenderer;
    mapRenderer.run(mapLoader.getMap());

    return 0;
}