#include "GameStateThugFight.h"
#include "GameStateNotification.h"
#include "Event.h"
#include "Engine.h"
#include "World.h"
#include "Terminal.h"
#include "Parser.h"
#include "File.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <ncurses.h>

int round(double a)
{
    return static_cast<int>(a + 0.5);
}

void GameStateThugFight::load_beats()
{
    for(int i = 0; i < 4; i++)
    {
        beats[i].resize(total_beats, NONE);
        for(int j = 8; j < total_beats; j++)
        {
            beats[i][j] = (rand() % int(4.0 * double(total_beats - j) / double(total_beats) + 4) == 0 ? UNBROKEN : NONE);
        }
    }
}

GameStateThugFight::GameStateThugFight(Engine* engine_in)
    : GameState(engine_in)
{
}

GameStateThugFight::~GameStateThugFight()
{
}

void GameStateThugFight::init()
{
    paused = false;
    cur_beat = 0;
    total_beats = 150;
    load_beats();
    abs = {8, {sf::seconds(0.0), sf::seconds(0.0), sf::seconds(0.0), sf::seconds(0.0)}};
    fragments = {};
    ab_height = 6;
    elapsed_time = sf::seconds(0.0);
    beat = sf::seconds(60.0 / 165.5 );
    thug_fist = get_file_contents("fist.txt");
    abs_str = get_file_contents("abs.txt");
    abs_tense_str = get_file_contents("abs_tense.txt");

    int stdscr_w, stdscr_h;
    getmaxyx(stdscr, stdscr_h, stdscr_w);
    window = newwin(config::screen_h_chars + 2, config::screen_w_chars + 2,
            (stdscr_h - config::screen_h_chars) / 2, (stdscr_w - config::screen_w_chars) / 2);

    wclear(window);
    nodelay(window, true);
    scrollok(window, false);
    curs_set(0);
    noecho();
}


void GameStateThugFight::cleanup()
{
    //wborder(window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    //wrefresh(window);
    delwin(window);
    wrefresh(stdscr);
    redrawwin(stdscr);
    curs_set(1);
    echo();
}

void GameStateThugFight::notify(event_ptr event)
{
}

void GameStateThugFight::break_fist(int index, int beat)
{
    if(index >= 0 && index < 4 && beat >= 0 && beat < total_beats)
    {
        beats[index][beat] = BROKEN;
        for(int j = 0; j < 25; j++)
        {
            fragments.push_back({ static_cast<double>(abs.health),
                                  static_cast<double>(rand() % 6) + index * ab_height,
                                  static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * 50.0,
                                  static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * 100.0 - 50.0,
                                  sf::seconds(0.5),
                                  false});
        }
    }
}

void GameStateThugFight::try_to_break(int index)
{
    if(index >= 0 && index < 4)
    {
        if(beats[index][cur_beat] == UNBROKEN)
        {
            break_fist(index, cur_beat);
        }
    }
}

int GameStateThugFight::get_seconds_remaining()
{
    return std::max(0, round((total_beats - cur_beat) * beat.asSeconds()));
}

void GameStateThugFight::update(sf::Time dt)
{
    int ch = wgetch(window);
    if(!paused)
    {
        int tense_index = -1;
        if(ch == 'a')
            tense_index = 0;
        else if(ch == 's')
            tense_index = 1;
        else if(ch == 'd')
            tense_index = 2;
        else if(ch == 'f')
            tense_index = 3;
        else if(ch == 'q')
            lose();
        else if(ch == 'w')
            win();

        if(tense_index != -1)
        {
            abs.tense[tense_index] = sf::seconds(beat.asSeconds() / 4.0);
            try_to_break(tense_index);
        }
        for(int i = 0; i < 4; i++)
        {
            if((int)cur_beat >= 1 && beats[i][(int)cur_beat - 1] == UNBROKEN)
            {
                abs.health--;
                beats[i][(int)cur_beat - 1] = MISSED;
                if(abs.health == 0)
                {
                    lose();
                }
            }
        }
        for(int i = 0; i < fragments.size(); )
        {
            if(fragments[i].x < 0 || fragments[i].x > config::screen_w_chars
               || fragments[i].y < 0 || fragments[i].y > config::screen_h_chars)
                fragments[i].dead = true;
            fragments[i].x += fragments[i].vx * dt.asSeconds();
            fragments[i].y += fragments[i].vy * dt.asSeconds();
            fragments[i].vy += 0.5;
            fragments[i].remaining_time -= dt;
            if(fragments[i].dead)
                fragments.erase(fragments.begin() + i);
            else
                i++;
        }
        if(cur_beat > total_beats)
        {
            win();
        }
        
        for(int i = 0; i < 4; i++)
        {
            abs.tense[i] -= dt;
        }
        elapsed_time += dt;
        cur_beat = (elapsed_time.asSeconds() / beat.asSeconds());
    }
    else if(ch == KEY_ENTER)
    {
        paused = false;
    }
}

void GameStateThugFight::draw()
{
    werase(window);
    box(window, 0, 0);
    for(int i = 0; i < 4; i++)
    {
        std::string ab_str_to_display = abs.tense[i].asSeconds() > 0.0 ? abs_tense_str : abs_str;
        /*if(abs.tense_ab == i)
            engine->terminal->set_color(config::colors[config::ORANGE]);
        else
            engine->terminal->set_color();*/
        engine->terminal->output(abs.health - 4, i * ab_height + 1, ab_str_to_display, window);
        std::string key_str_to_display = "";
        if(i == 0)
            key_str_to_display = "A";
        else if(i == 1)
            key_str_to_display = "S";
        else if(i == 2)
            key_str_to_display = "D";
        else if(i == 3)
            key_str_to_display = "F";
        engine->terminal->output(0, i * ab_height + ab_height / 2, key_str_to_display, window);
    }
    //engine->terminal->set_color();
    for(int i = 0; i < 4; i++)
    {
        for(int j = std::max(0, (int)cur_beat - 2); j <= std::min(total_beats, (int)cur_beat + 8); j++)
        {
            //engine->terminal->set_color(config::colors[fists[i].color_index]);
            if(beats[i][j] == UNBROKEN)
                engine->terminal->output(get_fist_x(j) + 1, i * ab_height + 1, thug_fist, window); //engine->terminal->set_color();
        }
    }
    for(int i = 0; i < fragments.size(); i++)
    {
        //engine->terminal->set_color(config::colors[fragments[i].color_index]);
        engine->terminal->output((int)fragments[i].x + 1, (int)fragments[i].y + 1, "*", window);
        //engine->terminal->set_color();
    }
    //engine->terminal->set_color();
    mvwprintw(window, 0, 1, "Jamal vs. Thugs");
    engine->terminal->output(1, 1, "TIME REMAINING: " + std::to_string(get_seconds_remaining()), window);
    wrefresh(window);
}

int GameStateThugFight::get_fist_x(int index)
{
    if(index <= cur_beat)
    {
        return abs.health;
    }
    else
    {
        double percent_screen_advancement = 1.0 - (double(index) - cur_beat) / 8.0;
        double squared_offset = 1 - percent_screen_advancement * percent_screen_advancement * percent_screen_advancement;
        return abs.health + int(squared_offset * (double(config::screen_w_chars) - abs.health)) + 1;
    }
}

void GameStateThugFight::win()
{
    engine->pop_state();
    engine->terminal->disp("Cowed by your abdominal prowess, the thugs slink off.");
    //send_front(std::make_shared<CmdRemoveGameState>(this));
}

void GameStateThugFight::lose()
{
    engine->pop_state();
    /*send(std::make_shared<CmdAddGameState>(new GameStateMenu(engine, this,
                    "Your abdomen is hard and tender from the repeated blows. You give up the ghost.\nYou had "
                    + std::to_string(get_seconds_remaining())
                    + " seconds remaining.\nTry again? (y/n)",
                    {{"y", {std::make_shared<CmdAddGameState>(new GameStateThugFight(engine))}},
                    {"n", {std::make_shared<CmdQuit>()}}})));*/
}
