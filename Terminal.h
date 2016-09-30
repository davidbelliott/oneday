#pragma once

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

	/*Collects user input and returns the user string.*/
	std::string get_input();

//private:
	Config* config;
};

