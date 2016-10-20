#include "Player.h"

Player::Player(std::string name_in, std::string description_in)
    : Object(name_in, description_in),
    objective(""),
    clothing("")
{
}

Player::~Player()
{
}
