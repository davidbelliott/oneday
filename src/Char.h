#pragma once
class Terminal;
#include <SFML/Graphics.hpp>

class Char
{
    public:
        char c;
        float x;    // How far the char is from its normal spot in the x direction
        float y;
        float lerp_coefficient;
        float snap_threshold;
        sf::Color fg;
        sf::Color bg;

        Char(char c_in, int spread = 0);
        void print(Terminal* t);
};

