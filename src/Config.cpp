#include "Config.h"
#include <iostream>


Config::Config()
: font(), screen_h_chars(20), screen_w_chars(80), char_size(20), char_height(0), char_width(0), lr_margin(2), window_height(0), window_width(0)
{
	if (!font.loadFromFile("bin/Inconsolata.otf"))
	{
		std::cerr << "Error: unable to load font Incolsolata.otf." << std::endl;
		exit(1);
	}

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(char_size);
	text.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	char_height = static_cast<int>(text.getLocalBounds().height);
	char_width = static_cast<int>(text.getLocalBounds().width / 52.0f);
	window_height = char_height * screen_h_chars;
	window_width = char_width * screen_w_chars + 2 * lr_margin;
}


Config::~Config()
{
}
