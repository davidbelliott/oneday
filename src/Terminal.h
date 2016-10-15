#pragma once

#include "CharBuffer.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Config;

/* A Terminal is a wrapper for a sf::RenderWindow which handles input and textual output. */
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

	/*Pauses until the user presses ENTER. Blocking.*/
	void pause();

    /*Gets a string input from the user. Blocking.*/
    std::string get_input();

    /*Gets an event off the top of the window's event queue. Returns false if no event.*/
    bool get_event(sf::Event* event);

    /*Sets the color of all text outputted after this command.*/
    void set_color(sf::Color color = sf::Color::White);

    /*Sets whether or not to display the cursor rectangle at the current cursor x and y.*/
    void set_disp_cursor(bool disp_cursor_in);

//private:

    /*Print's the terminal's buffer to the render target.*/
    void draw();

	Config* config;
    sf::RenderWindow* window;
    State state;
    CharBuffer buffer;
    bool disp_cursor;
    bool dirty;
};

