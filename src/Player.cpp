#include "Player.h"
#include "Engine.h"
#include "CmdDisp.h"

Player::Player(std::string name_in, std::string description_in)
    : Object(name_in, description_in),
    objective(""),
    clothing("")
{
}

Player::~Player()
{
}

void Player::set_objective(std::string objective_in, Engine* e)
{
    objective = objective_in;
    if(e)
    {
        e->push_event(new CmdDisp("New objective: " + objective));
    }
}
