#pragma once

#include "CharBuffer.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Terminal
{
public:

    enum Mode
    {
        INPUT,
        OUTPUT
    };

    struct State
    {
        int cursor_index;
        sf::Color foreground_color;
        sf::Color background_color;
        Mode mode;
    };

	Terminal();
	virtual ~Terminal();

    /*Print's the terminal's buffer to the render target.*/
    void draw();

private:

    /* Outputs the specified string at the specified index. Leaves index at the position following
     * the last modified character. */
    void output(std::string string, int& index);

    /* Puts the terminal into input mode, displaying the carat and changing the color accordingly. */
    void input_mode();

    /* Puts the terminal into output mode, changing the color accordingly. */
    void output_mode();

	/*Outputs the specified string at the current cursor location. Adds a newline if newline is true.*/
	void disp(std::string string, bool newline = true);

public:

    void pause();

    void unpause();

	/*Clears the screen.*/
	void clr();

    /*Removes last character (if one exists) and moves the cursor back one space (if possible).*/
    void backspace();

    /*Sets the color of all text outputted after this command.*/
    void set_color(sf::Color color = config::colors[config::color_default_fg]);

    /*Sets whether or not to display the cursor rectangle at the current cursor x and y.*/
    void set_disp_cursor(bool disp_cursor_in);

//private:


    sf::RenderWindow* window;
    State state;
    CharBuffer* buffer;
    bool disp_cursor;
    bool dirty;
    std::string cur_user_string;
};
