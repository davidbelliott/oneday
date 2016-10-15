#include "Config.h"
#include "Terminal.h"
#include <iostream>

Terminal::Terminal(Config* config_in)
: config(config_in), state({ 0, 0, sf::Color::White, sf::Color::Black }), buffer(config_in), disp_cursor(false)
{
}


Terminal::~Terminal()
{
}

void Terminal::output(std::string str, int& x, int& y)
{
	for (size_t i = 0; i < str.size(); i++)
	{
        if (x >= config->screen_w_chars)
        {
            x = 0;
            y++;
        }
		while (y >= config->screen_h_chars)
		{
			buffer.add_line();
			y--;
		}
		size_t index = buffer.get_index(x, y);
        if (index < buffer.contents.size())
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
		}
        if (x == config->screen_w_chars && disp_cursor)
        {
            x = 0;
            y++;
            while (y >= config->screen_h_chars)
            {
                buffer.add_line();
                y--;
            }
        }
	}
}

void Terminal::disp(std::string string, bool newline)
{
	//std::cout << string << std::endl;
    output(string + (newline ? "\n" : ""), state.cursor_x, state.cursor_y);
}

void Terminal::clr()
{
    buffer.clear();
    state.cursor_x = 0;
    state.cursor_y = 0;
}

void Terminal::backspace()
{
    //Index where the cursor rests after backspace
    size_t stop_index = buffer.get_index(state.cursor_x, state.cursor_y); 
    if(stop_index > 0)
    {
        stop_index -= 1;
        buffer.setChar(stop_index, '\0', sf::Color::Transparent, sf::Color::Transparent);
    }
    state.cursor_x = buffer.get_x(stop_index);//stop_index / config->screen_w_chars;
    state.cursor_y = buffer.get_y(stop_index);//stop_index % config->screen_w_chars;
}

void Terminal::pause()
{
}

void Terminal::prompt_input()
{
    set_color(sf::Color::Cyan);
    disp(">", false);
    disp_cursor = true;
}

void Terminal::set_color(sf::Color color)
{
    state.foreground_color = color;
}

void Terminal::draw(sf::RenderTarget* target)
{
    //target->draw(buffer);
    buffer.draw(target);

    if(disp_cursor)
    {
        sf::RectangleShape cursor_shape;
        cursor_shape.setSize(sf::Vector2f(config->char_width, config->char_height));
        cursor_shape.setFillColor(state.foreground_color);
        cursor_shape.setPosition(state.cursor_x * config->char_width, state.cursor_y * config->char_height);
        target->draw(cursor_shape);
    }
}
