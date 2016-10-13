#pragma once

#include "CharBuffer.h"
#include "Color.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Config;

class Terminal
{
public:

    struct State
    {
        int cursor_x;
        int cursor_y;
        sf::Color foreground_color;
        sf::Color background_color;
    };

	Terminal(Config* config_in);
	virtual ~Terminal();

    /* Outputs the specified string at the specified x and y. Leaves x and y at the position following
     * the last modified character. */
    void output(std::string string, int& x, int& y);

	/*Outputs the specified string at the current cursor location. Adds a newline if newline is true.*/
	void disp(std::string string, bool newline = true);

	/*Clears the screen.*/
	void clr();

    /*Removes last character (if one exists) and moves the cursor back one space (if possible).*/
    void backspace();

	/*Pauses until the user presses ENTER.*/
	void pause();

    /*Sets the color of all text outputted after this command.*/
    void set_color(Color color = DEFAULT);

    /*Print's the terminal's buffer to the render target.*/
    void draw(sf::RenderTarget* target);

//private:
	Config* config;
    State state;
    CharBuffer buffer;
    sf::Window* window;
};

