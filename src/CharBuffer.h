#pragma once

class Config;

#include <SFML/Graphics.hpp>
#include <vector>

class CharBuffer// : public sf::Drawable
{
public:

    struct Char {
        char c;
        sf::Color foreground_color;
        sf::Color background_color;
    };

    std::vector<Char> contents;

    CharBuffer(Config* config_in);
    virtual ~CharBuffer();

    void setChar(size_t index, char c, sf::Color foreground_color, sf::Color background_color);
    void nextLine();
    void scroll(int delta);

    void draw();//sf::RenderTarget& target, sf::RenderStates states) const override;
    void clear();

private:
	Config* config;
    size_t scrollValue;
};
