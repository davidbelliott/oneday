#include "CharBuffer.h"
#include "Config.h"
#include <iostream>

using namespace std;

CharBuffer::CharBuffer(Config* config_in)
  : contents(), scrollValue(0), config(config_in)
{
    contents.resize(config->screen_w_chars * config->screen_h_chars, {'\0', sf::Color::Transparent, sf::Color::Transparent});
}

CharBuffer::~CharBuffer()
{
}

void CharBuffer::setChar(size_t index, char c, sf::Color foreground_color, sf::Color background_color)
{
    if(index < config->screen_w_chars * config->screen_h_chars && index >= 0)
    {
        contents[index] = { c, foreground_color, background_color };;
    }
    else
    {
        std::cerr << "Error: character index " << index << " out of range." << std::endl;
    }
}

void CharBuffer::nextLine()
{
}

void CharBuffer::scroll(int delta)
{
    scrollValue = static_cast<size_t>(std::max(0, static_cast<int>(scrollValue) + delta));
}

void CharBuffer::draw()//sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Text text;
    text.setFont(config->font);

    for(size_t i = 0; i < config->screen_h_chars; i++)
    {
        for(size_t j = 0; j < config->screen_w_chars; j++)
        {
            std::string str = "";
            str += contents[i * config->screen_w_chars + j].c;
            text.setString(str);
            text.setPosition(j * config->char_width, i * config->char_height);
            //text.setColor(contents[i * j].foreground_color);
            //target.draw(text);
            if(str != "\n")
                std::cout << contents[i * config->screen_w_chars + j].c;
        }
        std::cout << std::endl;
    }
}

void CharBuffer::clear()
{
    contents.resize(config->screen_w_chars * config->screen_h_chars, {'\0', sf::Color::Transparent, sf::Color::Transparent} );
}
