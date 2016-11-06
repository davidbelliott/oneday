#include "Config.h"
#include "Terminal.h"
#include "Engine.h"
#include <iostream>

Terminal::Terminal(Receiver* owner_in)
:   state({ 0, config::colors[config::color_default_fg], config::colors[config::color_default_bg], OUTPUT }),
    buffer(new CharBuffer()),
    disp_cursor(false),
    dirty(true),
    cur_user_string(""),
    owner(owner_in)
{
}


Terminal::~Terminal()
{
    delete buffer;
}

void Terminal::output(std::string str, int start_x, int start_y)
{
    int x = start_x;
    int y = start_y;
	for (int i = 0; i < str.size(); i++)
	{
        if (x >= 0 && x < config::screen_w_chars && y >= 0 && y < config::screen_h_chars && str[i] != '\0')
        {
            int index = buffer->get_index(x, y);
            buffer->setChar(index, str[i], state.foreground_color, state.background_color);
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
    set_color(config::colors[config::color_user_input]);
    disp(">", false);
    state.mode = INPUT;
}

void Terminal::output_mode()
{
    set_color(config::colors[config::color_default_fg]);
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
        if (index < buffer->contents.size() && str[i] != '\0')
        {
            buffer->setChar(index, str[i], state.foreground_color, state.background_color);
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
        }
        while (y >= config::screen_h_chars && disp_cursor)
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
    dirty = true;
}

void Terminal::backspace()
{
    //Index where the cursor rests after backspace
    int stop_index = state.cursor_index; 
    if(stop_index > 0)
    {
        stop_index -= 1;
        buffer->setChar(stop_index, '\0', sf::Color::Transparent, sf::Color::Transparent);
    }
    state.cursor_index = stop_index;
    dirty = true;
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
    }
}

void Terminal::draw(sf::RenderTarget* target)
{
    target->clear(state.background_color);

    buffer->draw(target);

    if(state.mode == INPUT && buffer->get_y(state.cursor_index) < config::screen_h_chars)
    {
        sf::RectangleShape cursor_shape;
        cursor_shape.setSize(sf::Vector2f(config::char_width, config::char_height));
        cursor_shape.setFillColor(state.foreground_color);
        cursor_shape.setPosition(buffer->get_x(state.cursor_index) * config::char_width + config::padding, buffer->get_y(state.cursor_index) * config::char_height + config::padding);
        target->draw(cursor_shape);
    }
    dirty = false;
}

void Terminal::handle_event(Event* event)
{
    if(event->type == Event::CMD_DISP)
    {
        disp(static_cast<CmdDisp*>(event)->str);
    }
    else if(event->type == Event::CMD_OUTPUT)
    {
        CmdOutput* cmd = static_cast<CmdOutput*>(event);
        output(cmd->str, cmd->x, cmd->y);
    }
    else if(event->type == Event::CMD_CLEAR)
    {
        clr();
    }
    else if(event->type == Event::CMD_INPUT)
    {
        input_mode();
    }
    else if(event->type == Event::CMD_SETCOLOR)
    {
        set_color(static_cast<CmdSetColor*>(event)->color);
    }
    else if(event->type == Event::TEXT_ENTERED)
    {
        if(state.mode == INPUT)
        {
            char c = static_cast<EventTextEntered*>(event)->c;
            if(c == '\n' || c == '\r')
            {
                disp("");
                if(owner)
                    owner->add_event(new EventUserLine(cur_user_string));
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
    /*else if(ef_event->type == sf::Event::MouseWheelScrolled)
    {
        int scroll_delta = -sf_event->mouseWheelScroll.delta;
        buffer->scroll(scroll_delta);
    }*/
}
