#include "Config.h"
#include "Terminal.h"
#include <iostream>

Terminal::Terminal(Config* config_in)
: config(config_in)
{
}


Terminal::~Terminal()
{
}

void Terminal::disp(std::string string)
{
	std::cout << string << std::endl;
}

void Terminal::clr()
{
}

void Terminal::pause()
{
}

std::string Terminal::get_input()
{
	std::cout << "\n>";
	std::string input;
	std::getline(std::cin, input, '\n');
	return input;
}
