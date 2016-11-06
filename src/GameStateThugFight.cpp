#include "GameStateThugFight.h"
#include "Event.h"
#include "Terminal.h"
#include <fstream>
#include <algorithm>

std::string get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    std::string contents = "";
    if (in)
    {
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
    }
    std::replace(contents.begin(), contents.end(), '@', '\0');
    return(contents);
}

GameStateThugFight::GameStateThugFight()
    : GameState(nullptr),
    terminal(new Terminal()),
    fists()
{
    terminal->owner = this;
    std::ifstream thug_fist_file;
    thug_fist = get_file_contents("fist.txt");
}

GameStateThugFight::~GameStateThugFight()
{
    delete terminal;
}

void GameStateThugFight::init()
{
    running = true;
}

void GameStateThugFight::cleanup()
{
}

void GameStateThugFight::handle_event(Event* event)
{
    if(event->type == Event::KEY_PRESSED)
    {
        fists.push_back({ config::screen_w_chars - 14,
                          rand() % (config::screen_h_chars - 6), false,
                          sf::seconds(2),
                          rand() % (config::N_COLORS - config::RED) + config::RED,
                          false});
    }
    terminal->handle_event(event);
}

void GameStateThugFight::run(sf::Time dt)
{
    for(int i = 0; i < fists.size(); )
    {
        if(fists[i].punching)
        {
            if(fists[i].x <= 0)
            {
                for(int j = 0; j < 25; j++)
                {
                    fragments.push_back({ 0,
                                          rand() % 6 + fists[i].y,
                                          (double)rand() / (double)RAND_MAX * 50.0,
                                          (double)rand() / (double)RAND_MAX * 100.0 - 50.0,
                                          sf::seconds(0.5),
                                          fists[i].color_index,
                                          false});
                }
                fists[i].dead = true;
            }
            fists[i].x -= 1;
        }

        fists[i].remaining_time -= dt;
        if(fists[i].remaining_time.asSeconds() <= 0.0)
            fists[i].punching = true;
        if(fists[i].dead)
            fists.erase(fists.begin() + i);
        else
            i++;
    }
    for(int i = 0; i < fragments.size(); i++)
    {
        fragments[i].x += fragments[i].vx * dt.asSeconds();
        fragments[i].y += fragments[i].vy * dt.asSeconds();
    }
}

void GameStateThugFight::draw(sf::RenderTarget* target)
{
    terminal->clr();
    for(int i = 0; i < fists.size(); i++)
    {
        terminal->set_color(config::colors[fists[i].color_index]);
        terminal->output(thug_fist, (int)fists[i].x, (int)fists[i].y);
        terminal->set_color();
    }
    for(int i = 0; i < fragments.size(); i++)
    {
        terminal->set_color(config::colors[fragments[i].color_index]);
        terminal->output("*", (int)fragments[i].x, (int)fragments[i].y);
        terminal->set_color();
    }
    terminal->draw(target);
}
