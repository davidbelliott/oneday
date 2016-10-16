#pragma once

#include <SFML/Graphics.hpp>

class Config
{
public:
	sf::Font font;
	int screen_w_chars;
	int screen_h_chars;
	int char_size;
	int char_height;
	int char_width;
	int padding;
	int window_height;
	int window_width;
    int update_frequency;

    sf::Color base0;
    sf::Color base1;
    sf::Color base2;


	Config();
	virtual ~Config();
};

