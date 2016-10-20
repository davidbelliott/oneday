#pragma once
#include "Object.h"

class Player: public Object
{
    public:
        std::string objective;    //String telling the player what they have to do next
        std::string clothing;    //Name of object in inventory currently being worn
        
        Player(std::string name_in, std::string description_in);
        virtual ~Player();
};
