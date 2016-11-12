#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Event.h"
class Engine;

class Console
{
    public:
        bool input;
        Engine* engine;
        std::string cur_user_string;
        
        Console(Engine* engine_in);
        virtual ~Console();

        void get_input(sf::Window* window);
        void handle_input(event_ptr event);
};
