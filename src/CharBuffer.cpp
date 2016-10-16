#include "CharBuffer.h"
#include "Config.h"
#include <iostream>

using namespace std;

CharBuffer::CharBuffer()
  : contents(), scroll_value(0), scroll_value_max(0)
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

void CharBuffer::setChar(int index, char c, sf::Color foreground_color, sf::Color background_color)
{
    if(index < contents.size() && index >= 0)
    {
        contents[index] = { c, foreground_color, background_color };;
    }
    else
    {
        std::cerr << "Error: character index " << index << " out of range." << std::endl;
    }
}

void CharBuffer::add_line()
{
    contents.resize(contents.size() + config::screen_w_chars, {'\0', sf::Color::Transparent, sf::Color::Transparent} );
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
            std::string str = "";
            str += contents[index].c;
            text.setString(str);
            text.setPosition(j * config::char_width + config::padding, i * config::char_height);
            text.setFillColor(contents[index].foreground_color);
            target->draw(text);
        }
    }
}

void CharBuffer::clear()
{
    contents.assign(config::screen_w_chars * config::screen_h_chars, {'\0', sf::Color::Transparent, sf::Color::Transparent} );
    scroll_value = 0;
    scroll_value_max = 0;
}
