#include "Config.h"
#include "Terminal.h"
#include "EventSource.h"
#include <iostream>

Terminal::Terminal()
:   window(new sf::RenderWindow(sf::VideoMode(config::window_width, config::window_height), "One Day in the Life of Young Jamal")),
    state({ 0, config::colors[config::color_default_fg], config::colors[config::color_default_bg] }),
    buffer(),
    disp_cursor(false),
    dirty(true)
{
}


Terminal::~Terminal()
{
    delete window;
}

void Terminal::pause()
{
}

void Terminal::output(std::string str, int& index)
{
    int x = buffer.get_x(index);
    int y = buffer.get_y(index);

	for (int i = 0; i < str.size(); i++)
	{
        if (x >= config::screen_w_chars)
        {
            x = 0;
            y++;
        }
		while (y >= config::screen_h_chars)
		{
			buffer.add_line();
			y--;
		}
		int index = buffer.get_index(x, y);
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
        if (x == config::screen_w_chars && disp_cursor)
        {
            x = 0;
            y++;
            while (y >= config::screen_h_chars)
            {
                buffer.add_line();
                y--;
            }
        }
        dirty = true;
	}

    index = buffer.get_index(x, y);
}

void Terminal::disp(std::string string, bool newline)
{
	//std::cout << string << std::endl;
    buffer.scroll_value = buffer.scroll_value_max;
    output(string + (newline ? "\n" : ""), state.cursor_index);
}

void Terminal::clr()
{
    buffer.clear();
    state.cursor_index = 0;
    dirty = true;
}

void Terminal::backspace()
{
    //Index where the cursor rests after backspace
    int stop_index = state.cursor_index; 
    if(stop_index > 0)
    {
        stop_index -= 1;
        buffer.setChar(stop_index, '\0', sf::Color::Transparent, sf::Color::Transparent);
    }
    state.cursor_index = stop_index;
    dirty = true;
}

void Terminal::get_input(EventSource* source)
{
    sf::Event sf_event;
    while(window->pollEvent(sf_event))
    {
        Event* event = new Event(); // The wrapper to put sf_event in before sending to engine
        event->type = Event::SFML;
        event->sfml_event_data.sf_event = sf_event;
        source->send_event(event);
    }
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

        if(disp_cursor && buffer.get_y(state.cursor_index) < config::screen_h_chars)
        {
            sf::RectangleShape cursor_shape;
            cursor_shape.setSize(sf::Vector2f(config::char_width, config::char_height));
            cursor_shape.setFillColor(state.foreground_color);
            cursor_shape.setPosition(buffer.get_x(state.cursor_index) * config::char_width + config::padding, buffer.get_y(state.cursor_index) * config::char_height + config::padding);
            window->draw(cursor_shape);
        }
        dirty = false;

        window->display();
}
