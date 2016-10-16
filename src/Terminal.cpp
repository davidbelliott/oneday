#include "Config.h"
#include "Terminal.h"
#include <iostream>

Terminal::Terminal(Config* config_in)
:   config(config_in),
    window(new sf::RenderWindow(sf::VideoMode(config->window_width, config->window_height), "One Day in the Life of Young Jamal")),
    state({ 0, 0, sf::Color(153, 209, 206), sf::Color(17, 21, 28) }),
    buffer(config_in),
    disp_cursor(false),
    dirty(true)
{
}


Terminal::~Terminal()
{
    delete window;
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
        dirty = true;
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
    dirty = true;
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
    dirty = true;
}

void Terminal::pause()
{
    set_disp_cursor(true);
    draw();
    set_disp_cursor(false);
    sf::Event window_event;
    bool key_pressed = false;
    while(!key_pressed)
    {
        while(window->pollEvent(window_event))
        {
            if(window_event.type == sf::Event::KeyPressed)
                key_pressed = true;
            else if(window_event.type == sf::Event::Resized ||
                    window_event.type == sf::Event::GainedFocus)
                draw();
        }
    }
}

std::string Terminal::get_input()
{
    set_color(sf::Color::Cyan);
    disp(">", false);
    set_disp_cursor(true);
    draw();

    std::string cur_user_string = "";
    sf::Event window_event;
    while(true)
    {
        while(window->pollEvent(window_event))
        {
            if(window_event.type == sf::Event::TextEntered)
            {
                if(window_event.text.unicode < 128)
                {
                    char c = static_cast<char>(window_event.text.unicode);
                    if(c == '\n' || c == '\r')
                    {
                        set_color();
                        disp("");
                        //draw();
                        return cur_user_string;
                    }
                    else if(c == '\b')
                    {
                        if(cur_user_string.length() > 0)
                        {
                            backspace();
                            draw();
                            cur_user_string.pop_back();
                        }
                    }
                    else
                    {
                        cur_user_string += c;
                        std::string str = "";
                        str += c;
                        disp(str, false);
                        draw();
                    }
                }
            }
            else if(window_event.type == sf::Event::Resized ||
                    window_event.type == sf::Event::GainedFocus)
                draw();
        }
    }
    set_disp_cursor(false);
}

void Terminal::set_color(sf::Color color)
{
    state.foreground_color = color;
}

void Terminal::set_disp_cursor(bool disp_cursor_in)
{
    if(disp_cursor != disp_cursor_in)
    {
        disp_cursor = disp_cursor_in;
        dirty = true;
    }
}

void Terminal::draw()
{
        window->clear(state.background_color);

        buffer.draw(window);

        if(disp_cursor)
        {
            sf::RectangleShape cursor_shape;
            cursor_shape.setSize(sf::Vector2f(config->char_width, config->char_height));
            cursor_shape.setFillColor(state.foreground_color);
            cursor_shape.setPosition(state.cursor_x * config->char_width + config->padding, state.cursor_y * config->char_height + config->padding);
            window->draw(cursor_shape);
        }
        dirty = false;

        window->display();
}
