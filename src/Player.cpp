#include "Player.h"
#include "Engine.h"

Player::Player(std::string name_in, std::string description_in)
    : Object(name_in),
    objective(""),
    clothing("")
{
    add_component(new ComponentInventory());
}

Player::~Player()
{
}
