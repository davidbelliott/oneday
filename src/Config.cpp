#include "Config.h"
#include <iostream>


Config::Config()
: font(),
    screen_h_chars(20),
    screen_w_chars(80),
    char_size(14),
    char_height(14),
    char_width(8),
    padding(2),
    window_height(0),
    window_width(0),
    update_frequency(60)
{
	if (!font.loadFromFile("bin/ter-u14n.bdf"))
	{
		std::cerr << "Error: unable to load font Incolsolata.otf." << std::endl;
		exit(1);
	}


	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(char_size);

    /* If width and height are not known
	text.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	char_height = static_cast<int>(text.getLocalBounds().height);
	char_width = static_cast<int>(text.getLocalBounds().width / 52.0f);
    */

	window_height = char_height * screen_h_chars + 2 * padding;
	window_width = char_width * screen_w_chars + 2 * padding;
}


Config::~Config()
{
}
