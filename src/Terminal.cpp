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
    set_color(CYAN);
    std::cout << "#";
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
    set_color(CYAN);
	std::cout << ">";
	std::string input;
	std::getline(std::cin, input, '\n');
    set_color();
	return input;
}
