#include "Config.h"
#include "Terminal.h"
#include "Engine.h"
#include <iostream>

Terminal::Terminal(Engine* owner_engine_in)
:   state({ 0, sf::Color::White, sf::Color::Black, OUTPUT }),
    window(new sf::RenderWindow(sf::VideoMode(config::window_width, config::window_height), "One Day in the Life of Young Jamal")),
    buffer(new CharBuffer()),
    owner_engine(owner_engine_in),
    cur_user_string("")
{
}


Terminal::~Terminal()
{
    delete buffer;
}

void Terminal::output(int start_x, int start_y, std::string str, int spread)
{
    int x = start_x;
    int y = start_y;
	for (int i = 0; i < str.size(); i++)
	{
        if (x >= 0 && x < config::screen_w_chars && y >= 0 && y < config::screen_h_chars && str[i] != '\0' && str[i] != '\n')
        {
            int index = buffer->get_index(x, y);
            Char c = Char(str[i], spread);
            c.fg = state.foreground_color;
            c.bg = state.background_color;
            buffer->setChar(index, c);
        }
		if (str[i] == '\n')
		{
			x = start_x;
			y++;
		}
		else
		{
			x++;
		}
	}
}

void Terminal::input_mode()
{
    //set_color(config::colors[config::color_user_input]);
    disp(">", false);
    state.mode = INPUT;
}

void Terminal::output_mode()
{
    //set_color(config::colors[config::color_default_fg]);
    state.mode = OUTPUT;
}

void Terminal::disp(std::string str, bool newline)
{
	//std::cout << string << std::endl;
    buffer->scroll_value = buffer->scroll_value_max;
    int x = buffer->get_x(state.cursor_index);
    int y = buffer->get_y(state.cursor_index);
    if(newline)
        str += "\n";
    state.cursor_index = buffer->get_index(x, y);
	for (int i = 0; i < str.size(); i++)
	{
        if (x >= config::screen_w_chars)
        {
            x = 0;
            y++;
        }
		while (y >= config::screen_h_chars)
		{
			buffer->add_line();
			y--;
		}
		int index = buffer->get_index(x, y);
        if (index < buffer->contents.size() && str[i] != '\0' && str[i] != '\n')
        {
            Char c = Char(str[i]);
            c.fg = state.foreground_color;
            c.bg = state.background_color;
            buffer->setChar(index, c);
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
        if (x == config::screen_w_chars && state.mode == INPUT)
        {
            x = 0;
            y++;
        }
        while (y >= config::screen_h_chars && state.mode == INPUT)
        {
            buffer->add_line();
            y--;
        }
	}
    state.cursor_index = buffer->get_index(x, y);
}

void Terminal::clr()
{
    buffer->clear();
    state.cursor_index = 0;
}

void Terminal::backspace()
{
    //Index where the cursor rests after backspace
    int stop_index = state.cursor_index; 
    if(stop_index > 0)
    {
        stop_index -= 1;
        buffer->setChar(stop_index, '\0');
    }
    state.cursor_index = stop_index;
}

void Terminal::set_color(sf::Color color)
{
    state.foreground_color = color;
}

void Terminal::set_bg_color(sf::Color color)
{
    state.background_color = color;
}

void Terminal::display()
{
    window->clear(state.background_color);

    buffer->draw(window);

    if(state.mode == INPUT && buffer->get_y(state.cursor_index) < config::screen_h_chars)
    {
        sf::RectangleShape cursor_shape;
        cursor_shape.setSize(sf::Vector2f(config::char_width, config::char_height));
        cursor_shape.setFillColor(state.foreground_color);
        cursor_shape.setPosition(buffer->get_x(state.cursor_index) * config::char_width + config::padding, buffer->get_y(state.cursor_index) * config::char_height);
        window->draw(cursor_shape);
    }
    window->display();
}

void Terminal::get_input()
{
    sf::Event sf_event;
    while(window->pollEvent(sf_event))
    {
        event_ptr event = nullptr;
        if(sf_event.type == sf::Event::KeyPressed)
            event = std::make_shared<EventKeyPressed>(sf_event.key.code);
        else if(sf_event.type == sf::Event::TextEntered)
            event = std::make_shared<EventTextEntered>(static_cast<char>(sf_event.text.unicode));
        if(event)
        {
            owner_engine->notify(event);
            this->notify(event);
        }
    }
}

void Terminal::notify(event_ptr event)
{
    if(event->type == Event::TEXT_ENTERED)
    {
        if(state.mode == INPUT)
        {
            char c = std::static_pointer_cast<EventTextEntered>(event)->c;
            if(c == '\n' || c == '\r')
            {
                disp("");
                owner_engine->notify(std::make_shared<EventUserLine>(cur_user_string));
                cur_user_string = "";
                output_mode();
            }
            else if(c == '\b')
            {
                if(cur_user_string.length() > 0)
                {
                    backspace();
                    cur_user_string.pop_back();
                }
            }
            else
            {
                cur_user_string += c;
                std::string str = "";
                str += c;
                disp(str, false);
            }
        }
    }
}
