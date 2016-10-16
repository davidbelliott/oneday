#pragma once

#include "CharBuffer.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

/* A Terminal is a wrapper for a sf::RenderWindow which handles input and textual output. */
class Terminal
{
public:

    struct State
    {
        int cursor_index;
        sf::Color foreground_color;
        sf::Color background_color;
    };

	Terminal();
	virtual ~Terminal();



    /* Outputs the specified string at the specified index. Leaves index at the position following
     * the last modified character. */
    void output(std::string string, int& index);

	/*Outputs the specified string at the current cursor location. Adds a newline if newline is true.*/
	void disp(std::string string, bool newline = true);

    void pause();

	/*Clears the screen.*/
	void clr();

    /*Removes last character (if one exists) and moves the cursor back one space (if possible).*/
    void backspace();

    /*Gets an event off the top of the window's event queue. Returns false if no event.*/
    bool get_event(sf::Event* event);

    /*Sets the color of all text outputted after this command.*/
    void set_color(sf::Color color = config::colors[config::color_default_fg]);

    /*Sets whether or not to display the cursor rectangle at the current cursor x and y.*/
    void set_disp_cursor(bool disp_cursor_in);

//private:

    /*Print's the terminal's buffer to the render target.*/
    void draw();

    sf::RenderWindow* window;
    State state;
    CharBuffer buffer;
    bool disp_cursor;
    bool dirty;
};

