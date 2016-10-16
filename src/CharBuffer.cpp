#include "CharBuffer.h"
#include "Config.h"
#include <iostream>

using namespace std;

CharBuffer::CharBuffer(Config* config_in)
  : contents(), scrollValue(0), config(config_in)
{
    clear();
}

CharBuffer::~CharBuffer()
{
}

size_t CharBuffer::get_index(int x, int y)
{
    return x + (y + scrollValue) * config->screen_w_chars;
}

int CharBuffer::get_x(size_t index)
{
    return index % config->screen_w_chars;
}

int CharBuffer::get_y(size_t index)
{
    return index / config->screen_w_chars - scrollValue;
}

void CharBuffer::setChar(size_t index, char c, sf::Color foreground_color, sf::Color background_color)
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
    contents.resize(contents.size() + config->screen_w_chars, {'\0', sf::Color::Transparent, sf::Color::Transparent} );
    scrollValue++;
}

void CharBuffer::scroll(int delta)
{
    scrollValue = static_cast<size_t>(std::max(0, static_cast<int>(scrollValue) + delta));
}

void CharBuffer::draw(sf::RenderTarget* target)//sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Text text;
    text.setFont(config->font);
    text.setCharacterSize(config->char_size);

    for(size_t i = 0; i < config->screen_h_chars; i++)
    {
        for(size_t j = 0; j < config->screen_w_chars; j++)
        {
            size_t index = (i + scrollValue) * config->screen_w_chars + j;
            std::string str = "";
            str += contents[index].c;
            text.setString(str);
            text.setPosition(j * config->char_width + config->padding, i * config->char_height);
            text.setFillColor(contents[index].foreground_color);
            target->draw(text);
        }
    }
}

void CharBuffer::clear()
{
    contents.assign(config->screen_w_chars * config->screen_h_chars, {'\0', sf::Color::Transparent, sf::Color::Transparent} );
    scrollValue = 0;
}
