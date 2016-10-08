#include "Config.h"
#include "Terminal.h"
#include <iostream>

Terminal::Terminal(Config* config_in)
: config(config_in), cur_color(WHITE)
{
    set_color();
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
    set_color(BLUE);
    disp("Press any key to continue...");
    getchar();
    set_color();
}

void Terminal::set_color(Color color)
{
    cur_color = color;
    if(color == DEFAULT)
        std::cout << "\033[0m";
    else
        std::cout << "\033[3" << cur_color << "m";
}

std::string Terminal::get_input()
{
	std::cout << "\n>";
	std::string input;
	std::getline(std::cin, input, '\n');
	return input;
}
