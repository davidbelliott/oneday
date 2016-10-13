#include "Config.h"
#include "Terminal.h"
#include <iostream>

Terminal::Terminal(Config* config_in)
: config(config_in), state({ 0, 0, sf::Color::White, sf::Color::Black }), buffer(config_in)
{
}


Terminal::~Terminal()
{
}

void Terminal::output(std::string str, int& x, int& y)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		size_t index = x + y * config->screen_w_chars;
        if (index < config->screen_w_chars * config->screen_h_chars)
        {
            buffer.setChar(index, str[i], state.foreground_color, state.background_color);
        }
		if (str[i] == '\n')
		{
			x = 0;
			y++;
		}
		else
		{
			x++;
			if (x >= config->screen_w_chars)
			{
				x = 0;
				y++;
			}
		}
		/*while (y >= config->screen_h_chars)
		{
			buffer.nextLine();
			y--;
		}*/
	}
}

void Terminal::disp(std::string string)
{
	//std::cout << string << std::endl;
    output(string, state.cursor_x, state.cursor_y);
}

void Terminal::clr()
{
    buffer.clear();
    state.cursor_x = 0;
    state.cursor_y = 0;
}

void Terminal::pause()
{
    set_color(CYAN);
    std::cout << "#";
    getchar();
    set_color();
}

void Terminal::set_color(Color color)
{
}

std::string Terminal::get_input()
{
    set_color(CYAN);
	std::cout << ">";
	std::string input;
	std::getline(std::cin, input, '\n');
    set_color();
	return input;
}

void Terminal::draw(sf::RenderTarget* target)
{
    //target->draw(buffer);
    buffer.draw(target);
}
