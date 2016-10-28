#pragma once

#include <SFML/Graphics.hpp>

namespace config
{
    extern sf::Font font;
    extern int screen_w_chars;
    extern int screen_h_chars;
    extern int char_size;
    extern int char_height;
    extern int char_width;
    extern int padding;
    extern int window_height;
    extern int window_width;
    extern int update_frequency;
    enum ColorIndex
    {
        BASE_0 = 0,
        BASE_1,
        BASE_2,
        BASE_3,
        BASE_4,
        BASE_5,
        BASE_6,
        BASE_7,
        RED,
        ORANGE,
        YELLOW,
        MAGENTA,
        VIOLET,
        BLUE,
        CYAN,
        GREEN,
        N_COLORS
    };
    extern ColorIndex color_default_fg;
    extern ColorIndex color_default_bg;
    extern ColorIndex color_room_title;
    extern ColorIndex color_user_input;
    extern ColorIndex color_objective;
    extern sf::Color colors[N_COLORS];

    void config_init();
}
