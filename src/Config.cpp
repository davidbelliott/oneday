#include "Config.h"
#include <iostream>

namespace config
{
    int screen_h_chars = 24;
    int screen_w_chars = 80;
    int char_height = 12;
    int char_width = 12;
    int padding = 2;
    int window_height = 0;
    int window_width = 0;
    int update_frequency = 60;
    BitmapFont font;
    sf::Color colors[N_COLORS];
    ColorIndex color_default_fg = BASE_6;
    ColorIndex color_default_bg = BASE_0;
    ColorIndex color_room_title = ORANGE;
    ColorIndex color_user_input = GREEN;
    ColorIndex color_objective = RED;

    void config_init()
    {
        if (!font.loadFromFile("bin/cp437_12x12.png", char_width, char_height))
        {
            std::cerr << "Error: unable to load font." << std::endl;
            exit(1);
        }

        window_height = char_height * screen_h_chars + 2 * padding;
        window_width = char_width * screen_w_chars + 2 * padding;

        /* Gotham colors
        colors[BASE_0] = sf::Color(12,16,20);
        colors[BASE_1] = sf::Color(17,21,28);
        colors[BASE_2] = sf::Color(9,31,46);
        colors[BASE_3] = sf::Color(10,55,73);
        colors[BASE_4] = sf::Color(36,83,97);
        colors[BASE_5] = sf::Color(89,156,171);
        colors[BASE_6] = sf::Color(153,209,206);
        colors[BASE_7] = sf::Color(211,235,233);
        colors[RED] = sf::Color(194,49,39);
        colors[ORANGE] = sf::Color(210,105,55);
        colors[YELLOW] = sf::Color(237,180,67);
        colors[MAGENTA] = sf::Color(136,140,166);
        colors[VIOLET] = sf::Color(78,81,102);
        colors[BLUE] = sf::Color(25,84,102);
        colors[CYAN] = sf::Color(51,133,158);
        colors[GREEN] = sf::Color(42,168,137);
        */
        colors[BASE_0] = sf::Color(0,0,0);
        colors[BASE_1] = sf::Color(17,21,28);
        colors[BASE_2] = sf::Color(9,31,46);
        colors[BASE_3] = sf::Color(10,55,73);
        colors[BASE_4] = sf::Color(36,83,97);
        colors[BASE_5] = sf::Color(89,156,171);
        colors[BASE_6] = sf::Color(153,209,206);
        colors[BASE_7] = sf::Color(211,235,233);
        colors[RED] = sf::Color(194,49,39);
        colors[ORANGE] = sf::Color(210,105,55);
        colors[YELLOW] = sf::Color(237,180,67);
        colors[MAGENTA] = sf::Color(136,140,166);
        colors[VIOLET] = sf::Color(78,81,102);
        colors[BLUE] = sf::Color(25,84,102);
        colors[CYAN] = sf::Color(51,133,158);
        colors[GREEN] = sf::Color(42,168,137);
    }
}
