#include "Config.h"
#include "Terminal.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <functional>
#include <ncurses.h>
#include "Parser.h"

std::string word_wrap(std::string s, int width)
{
    int chars_since_newline = 0;
    int chars_since_whitespace = 0;


    for(int i = 0; i < s.length(); i++)
    {
        chars_since_newline++;
        chars_since_whitespace++;
        if(s[i] == '\n')
        {
            chars_since_newline = 0;
            chars_since_whitespace = 0;
        }
        else if(s[i] == ' ')
        {
            chars_since_whitespace = 0;
        }
        if(chars_since_newline >= width)
        {
            if(chars_since_whitespace < width)
                s[i - chars_since_whitespace] = '\n';
            else
                s.insert(s.begin() + i, '\n');
            chars_since_newline = 0;
            chars_since_whitespace = 0;
        }
    }

    return s;
 }


Terminal::Terminal()
//:   //canvas(nullptr),
    //display(nullptr)
{
    initscr();
    cbreak();
    wresize(stdscr, config::screen_h_chars, config::screen_w_chars);
    scrollok(stdscr, true);
    curs_set(0);
    //canvas = caca_create_canvas(config::screen_w_chars, config::screen_h_chars);
    //display = caca_create_display(canvas);
}


Terminal::~Terminal()
{
    endwin();
    //caca_free_display(display);
    //caca_free_canvas(canvas);
}

void Terminal::output(int start_x, int start_y, std::string str, int spread)
{
    int x = start_x;
    int y = start_y;
    for(auto it = str.begin(); it != str.end(); ++it)
    {
        if(*it == '\n')
        {
            y++;
            x = start_x;
        }
        else
        {
            mvaddch(y, x, *it);
            x++;
        }
    }
}

void Terminal::disp(std::string str, bool newline)
{
    printw((str + (newline ? "\n" : "")).c_str());
}

void Terminal::ers()
{
    erase();
}

void Terminal::clr()
{
    clear();
}

void Terminal::refresh_display()
{
    refresh();
}

std::string Terminal::get_input()
{
    curs_set(1);
    printw(">");
    char str[80];
    getstr(str);
    curs_set(0);
    return std::string(str);
}

void Terminal::pause()
{
    getch();
}
