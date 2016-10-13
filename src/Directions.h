#pragma once
#include <string>
#include <map>

enum DirectionId
{
    NORTH,
    EAST,
    SOUTH,
    WEST,
    UP,
    DOWN,
    DIRECTION_MAX
};

struct DirectionData
{
    std::string name;
    std::string alias;
    std::string dir_reference;  //String used to reference direction, e.g. "To the north"
};

extern std::map<DirectionId, DirectionData> dir;
