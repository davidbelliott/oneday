#include "Config.h"
#include "Terminal.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <functional>

std::string word_wrap(std::string s, int width)
{
    int chars_since_newline = 0;
    int chars_since_whitespace = 0;


    for(int i = 0; i < s.length(); i++)
    {
        chars_since_newline++;
        chars_since_whitespace++;
        if(s[i] == '\n')
        {
            chars_since_newline = 0;
            chars_since_whitespace = 0;
        }
        else if(s[i] == ' ')
        {
            chars_since_whitespace = 0;
        }
        if(chars_since_newline >= width)
        {
            if(chars_since_whitespace < width)
                s[i - chars_since_whitespace] = '\n';
            else
                s.insert(s.begin() + i, '\n');
            chars_since_newline = 0;
            chars_since_whitespace = 0;
        }
    }

    return s;
 }


Terminal::Terminal()
:   canvas(nullptr),
    display(nullptr)
{
    canvas = caca_create_canvas(config::screen_w_chars, config::screen_h_chars);
    display = caca_create_display(canvas);
}


Terminal::~Terminal()
{
    caca_free_display(display);
    caca_free_canvas(canvas);
}

void Terminal::output(int start_x, int start_y, std::string str, int spread)
{
    caca_put_str(canvas, start_x, start_y, str.c_str());
    /*int x = start_x;
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
	}*/
}

/*void Terminal::input_mode()
{
    //set_color(config::colors[config::color_user_input]);
    disp(">", false);
    state.mode = INPUT;
}

void Terminal::output_mode()
{
    //set_color(config::colors[config::color_default_fg]);
    state.mode = OUTPUT;
}*/

void Terminal::disp(std::string str, bool newline)
{
    caca_put_str(canvas, 0, 0, str.c_str());
    /*buffer->scroll_value = buffer->scroll_value_max;
    int x = buffer->get_x(state.cursor_index);
    int y = buffer->get_y(state.cursor_index);
    if(newline)
        str += "\n";
    str = word_wrap(str, config::screen_w_chars);
    state.cursor_index = buffer->get_index(x, y);
	for (int i = 0; i < str.size(); i++)
	{
        if(str[i] == '\n')
        {
            x = 0;
            y++;
        }
        else
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
            if (index < buffer->contents.size() && str[i] != '\0')

            {
                Char c = Char(str[i]);
                c.fg = state.foreground_color;
                c.bg = state.background_color;
                buffer->setChar(index, c);
            }
            x++;
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
	}
    state.cursor_index = buffer->get_index(x, y);*/
}

void Terminal::clr()
{
    //buffer->clear();
    //state.cursor_index = 0;
}

void Terminal::backspace()
{
    //Index where the cursor rests after backspace
    /*int stop_index = state.cursor_index; 
    if(stop_index > 0)
    {
        stop_index -= 1;
        buffer->setChar(stop_index, '\0');
    }
    state.cursor_index = stop_index;*/
}

void Terminal::refresh_display()
{
    /*window->clear(state.background_color);

    buffer->draw(window);

    if(state.mode == INPUT && buffer->get_y(state.cursor_index) < config::screen_h_chars)
    {
        sf::RectangleShape cursor_shape;
        cursor_shape.setSize(sf::Vector2f(config::char_width, config::char_height));
        cursor_shape.setFillColor(state.foreground_color);
        cursor_shape.setPosition(buffer->get_x(state.cursor_index) * config::char_width + config::padding, buffer->get_y(state.cursor_index) * config::char_height);
        window->draw(cursor_shape);
    }
    window->display();*/
    caca_refresh_display(display);
}

/*void Terminal::get_input()
{
    sf::Event sf_event;
    while(window->pollEvent(sf_event))
    {
        event_ptr event = nullptr;
        if(sf_event.type == sf::Event::KeyPressed)
            event = std::make_shared<EventKeyPressed>(sf_event.key.code);
        else if(sf_event.type == sf::Event::KeyReleased)
            event = std::make_shared<EventKeyReleased>(sf_event.key.code);
        else if(sf_event.type == sf::Event::TextEntered)
            event = std::make_shared<EventTextEntered>(static_cast<char>(sf_event.text.unicode));
        if(event)
        {
            owner_engine->notify(event);
            this->notify(event);
        }
    }
}*/

std::string Terminal::get_input()
{
    std::string input;
    std::getline(std::cin, input, '\n');
    return input;
}

void Terminal::pause()
{
    caca_get_event(display, CACA_EVENT_KEY_PRESS, NULL, -1);
}

/*void Terminal::notify(event_ptr event)
{
    if(event->type == Event::TEXT_ENTERED)
    {
        if(state.mode == INPUT)
        {
            char c = std::static_pointer_cast<EventTextEntered>(event)->c;
            if(c == '\n' || c == '\r')
            {
                disp("");
                //owner_engine->notify(std::make_shared<EventUserLine>(cur_user_string));
                cur_user_string = "";
                //output_mode();
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
}*/
