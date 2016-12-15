#pragma once

class Engine;

#include "Observer.h"
#include "CharBuffer.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

std::string word_wrap(std::string str, int width);

class Terminal : public Observer
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

    Engine* owner_engine;
    sf::RenderWindow* window;
    State state;
    std::string cur_user_string;
    CharBuffer* buffer;

	Terminal(Engine* owner_engine_in);
	virtual ~Terminal();

    /* Displays the terminal in its current state. */
    void display();

    /* Outputs the specified string at the specified x and y values. Leaves x and y at the position following
     * the last modified character. */
    void output(int x, int y, std::string string, int spread = 0);

    /* Puts the terminal into input mode, displaying the carat and changing the color accordingly. */
    void input_mode();

    /* Puts the terminal into output mode, changing the color accordingly. */
    void output_mode();

	/*Outputs the specified string at the current cursor location. Adds a newline if newline is true.*/
	void disp(std::string string, bool newline = true);

	/*Clears the screen.*/
	void clr();

    /*Removes last character (if one exists) and moves the cursor back one space (if possible).*/
    void backspace();

    /*Sets the color of all text outputted after this command.*/
    void set_color(sf::Color color = config::colors[config::color_default_fg]);

    /*Sets the color of the background for the entire terminal.*/
    void set_bg_color(sf::Color color = config::colors[config::color_default_bg]);

    /*Sets whether or not to display the cursor rectangle at the current cursor x and y.*/
    void set_disp_cursor(bool disp_cursor_in);

    /* Gets input from the window, packages as Events, and sends to the engine. */
    void get_input();

    /* Inherited from Observer. Terminal calls this on itself. */
    void notify(event_ptr event);


//private:

};
