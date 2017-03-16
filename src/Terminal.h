#pragma once

class Engine;

#include "Config.h"
#include <string>
#include <vector>
#include <ncurses.h>

std::string word_wrap(std::string str, int width);

class Terminal
{
public:

    std::string cur_user_string;

	Terminal();
	virtual ~Terminal();

    /* Displays the terminal in its current state. */
    void refresh_display();

    /* Outputs the specified string at the specified x and y values. Leaves x and y at the position following
     * the last modified character. */
    void output(int x, int y, std::string string, WINDOW* window);

    /* Gets a string of input from the user.*/
    std::string get_input();

    /* Pauses until the user presses a key. */
    void pause();

	/*Outputs the specified string at the current cursor location. Adds a newline if newline is true.*/
	void disp(std::string string, bool newline = true);

	/*Clears the screen completely.*/
	void clr();

    /*Covers the screen in spaces.*/
    void ers();
};
