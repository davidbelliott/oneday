#pragma once

#include "Color.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Config;

class Terminal
{
public:
	Terminal(Config* config_in);
	virtual ~Terminal();

	/*Outputs the specified string at the current cursor location, wrapping if wrap is true.*/
	void disp(std::string string);

	/*Clears the screen.*/
	void clr();

	/*Pauses until the user presses ENTER.*/
	void pause();

    /*Sets the color of all text outputted after this command.*/
    void set_color(Color color = WHITE);

	/*Collects user input and returns the user string.*/
	std::string get_input();

//private:
	Config* config;
    Color cur_color;
};

