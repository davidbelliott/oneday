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
    fists(),
    fragments(),
    thug_fist(""),
    your_fist(""),
    time_alive(sf::seconds(0)),
    total_time(sf::seconds(180)),
    spawn_countdown(0)
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

void GameStateThugFight::handle_event(std::shared_ptr<Event> event)
{
    if(event->type == Event::KEY_PRESSED)
    {
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
                                          static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * 50.0,
                                          static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * 100.0 - 50.0,
                                          sf::seconds(0.5),
                                          fists[i].color_index,
                                          false});
                }
                fists[i].dead = true;
            }
            fists[i].x -= 10;
        }
        else
        {
            fists[i].x -= (dt.asSeconds() / 2.0 * 13.0);
        }


        if(fists[i].remaining_time.asSeconds() <= 0.25f && fists[i].color_index == config::BASE_6)
              fists[i].color_index = rand() % (config::N_COLORS - config::RED) + config::RED;
        if(fists[i].remaining_time.asSeconds() <= 0.0)
            fists[i].punching = true;

        fists[i].remaining_time -= dt;

        if(fists[i].dead)
            fists.erase(fists.begin() + i);
        else
            i++;
    }
    for(int i = 0; i < fragments.size(); )
    {
        if(fragments[i].x < 0 || fragments[i].x > config::screen_w_chars
           || fragments[i].y < 0 || fragments[i].y > config::screen_h_chars)
            fragments[i].dead = true;
        fragments[i].x += fragments[i].vx * dt.asSeconds();
        fragments[i].y += fragments[i].vy * dt.asSeconds();
        fragments[i].remaining_time -= dt;
        if(fragments[i].dead)
            fragments.erase(fragments.begin() + i);
        else
            i++;
    }
    if(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) >= (1.0 - time_alive.asSeconds() / total_time.asSeconds()))
    {
        fists.push_back({ static_cast<double>(config::screen_w_chars - 1),
                          static_cast<double>(rand() % (config::screen_h_chars - 6)),
                          false,
                          sf::seconds(2),
                          rand() % (config::N_COLORS - config::RED) + config::RED,
                          false});
        spawn_countdown = (total_time.asSeconds() - time_alive.asSeconds()) / 180.0 + 0.1;
    }
    time_alive += dt;
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
