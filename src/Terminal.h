#pragma once

class Engine;

#include "Config.h"
#include <string>
#include <vector>

std::string word_wrap(std::string str, int width);

class Terminal
{
public:

    //caca_display_t* display;
    //caca_canvas_t* canvas;

    std::string cur_user_string;

	Terminal();
	virtual ~Terminal();

    /* Displays the terminal in its current state. */
    void refresh_display();

    /* Outputs the specified string at the specified x and y values. Leaves x and y at the position following
     * the last modified character. */
    void output(int x, int y, std::string string, int spread = 0);

    /* Gets a string of input from the user.*/
    std::string get_input();

    /* Pauses until the user presses a key. */
    void pause();

	/*Outputs the specified string at the current cursor location. Adds a newline if newline is true.*/
	void disp(std::string string, bool newline = true);

	/*Clears the screen.*/
	void clr();

    /*Removes last character (if one exists) and moves the cursor back one space (if possible).*/
    void backspace();

    /*Sets whether or not to display the cursor rectangle at the current cursor x and y.*/
    void set_disp_cursor(bool disp_cursor_in);

    /* Gets input from the window, packages as Events, and sends to the engine. */
    //void get_input();

    /* Inherited from Observer. Terminal calls this on itself. */
    //void notify(event_ptr event);


//private:

};
