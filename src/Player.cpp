#include "Player.h"
#include "Terminal.h"

Player::Player(std::string name_in, std::string description_in)
    : Object(name_in, description_in),
    objective(""),
    clothing("")
{
}

Player::~Player()
{
}

void Player::set_objective(std::string objective_in, Terminal* t)
{
    objective = objective_in;
    if(t)
    {
        t->set_color(config::colors[config::color_objective]);
        t->disp("New objective: " + objective);
        t->set_color();
    }
}
