#pragma once
#include "Char.h"
#include <SFML/Graphics.hpp>
#include <vector>

class CharBuffer// : public sf::Drawable
{
public:

    std::vector<Char> contents;

    CharBuffer();
    virtual ~CharBuffer();

    int get_index(int x, int y);
    int get_x(int index);
    int get_y(int index);

    void setChar(int index, Char c);
    void add_line();
    void scroll(int delta);

    void draw(sf::RenderTarget* target);//sf::RenderTarget& target, sf::RenderStates states) const override;
    void update_visual();
    void clear();

    int scroll_value;
    int scroll_value_max;
};
