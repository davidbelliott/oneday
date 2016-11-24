#include "CharBuffer.h"
#include "Config.h"
#include <iostream>
#include <cmath>

using namespace std;

CharBuffer::CharBuffer()
:   contents({}),
    scroll_value(0), scroll_value_max(0)
{
    clear();
}

CharBuffer::~CharBuffer()
{
}

int CharBuffer::get_index(int x, int y)
{
    return x + (y + scroll_value) * config::screen_w_chars;
}

int CharBuffer::get_x(int index)
{
    return index % config::screen_w_chars;
}

int CharBuffer::get_y(int index)
{
    return index / config::screen_w_chars - scroll_value;
}

void CharBuffer::setChar(int index, Char c)
{
    if(index < contents.size() && index >= 0)
    {
        contents[index] = c;
    }
    else
    {
        std::cerr << "Error: character index " << index << " out of range." << std::endl;
    }
}

void CharBuffer::add_line()
{
    contents.resize(contents.size() + config::screen_w_chars, {'\0'} );
    scroll_value_max++;
    scroll_value = scroll_value_max;
}

void CharBuffer::scroll(int delta)
{
    scroll_value = std::max(0, std::min(scroll_value_max, scroll_value + delta));
}

void CharBuffer::draw(sf::RenderTarget* target)//sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Text text;
    text.setFont(config::font);
    text.setCharacterSize(config::char_size);

    for(int i = 0; i < config::screen_h_chars; i++)
    {
        for(int j = 0; j < config::screen_w_chars; j++)
        {
            int index = (i + scroll_value) * config::screen_w_chars + j;
            if(contents[index].c != '\0' && contents[index].x + j >= 0 && contents[index].x + j < config::screen_w_chars && contents[index].y + i >= 0 && contents[index].y + i< config::screen_h_chars)
            {
                std::string str = "";
                str += contents[index].c;
                text.setString(str);
                text.setPosition(int(contents[index].x + j) * config::char_width + config::padding, int(contents[index].y + i) * config::char_height);
                text.setFillColor(contents[index].fg);
                target->draw(text);
            }
        }
    }
    update_visual();
}

void CharBuffer::update_visual()
{
    for(int i = 0; i < contents.size(); i++)
    {
        if(std::abs(contents[i].x) >= contents[i].snap_threshold)
            contents[i].x -= contents[i].x * std::min((float)1.0, contents[i].lerp_coefficient);
        else
            contents[i].x = 0.0;

        if(std::abs(contents[i].y) >= contents[i].snap_threshold)
            contents[i].y -= contents[i].y * std::min((float)1.0, contents[i].lerp_coefficient);
        else
            contents[i].y = 0;
    }
}

void CharBuffer::clear()
{
    contents.assign(config::screen_w_chars * config::screen_h_chars, {'\0'} );
    scroll_value = 0;
    scroll_value_max = 0;
}
