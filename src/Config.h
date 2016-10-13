#pragma once

//#include <SFML/Graphics.hpp>

class Config
{
public:
	//sf::Font font;
	int screen_w_chars;
	int screen_h_chars;
	int char_size;
	int char_height;
	int char_width;
	int lr_margin;
	int window_height;
	int window_width;

	Config();
	virtual ~Config();
};

