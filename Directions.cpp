#include "Directions.h"

std::map<DirectionId, DirectionData> dir = {
    { NORTH, { "north", "n", "To the north" } },
    { EAST, { "east", "e", "To the east" } },
    { SOUTH, { "south", "s", "To the south" } },
    { WEST, { "west", "w", "To the west" } },
    { UP, {"up", "u", "Up" } },
    { DOWN, {"down", "d", "Down" } }
};
