#include "GameStateThugFight.h"
#include "Event.h"
#include "Engine.h"
#include "World.h"
#include "Terminal.h"
#include "Parser.h"
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

GameStateThugFight::GameStateThugFight(Engine* engine_in)
    : GameState(engine_in),
    abs({ sf::seconds(0.0), 5, 5, false }),
    fists(),
    fragments(),
    thug_fist(""),
    abs_str(),
    abs_tense_str(),
    time_alive(sf::seconds(0)),
    total_time(sf::seconds(180)),
    spawn_countdown(0)
{
    std::ifstream thug_fist_file;
    thug_fist = get_file_contents("fist.txt");
    abs_str = get_file_contents("abs.txt");
    abs_tense_str = get_file_contents("abs_tense.txt");
}

GameStateThugFight::~GameStateThugFight()
{
}

void GameStateThugFight::notify(event_ptr event)
{
    if(event->type == Event::KEY_PRESSED && abs.power > 0)
    {
        abs.tense = true;
        abs.remaining_time = sf::seconds(0.2);
        abs.power -= 1;
    }
}

void GameStateThugFight::update(sf::Time dt)
{
    for(int i = 0; i < fists.size(); )
    {
        if(fists[i].punching)
        {
            if(fists[i].x < abs.health)
            {
                if(abs.tense)
                {
                    for(int j = 0; j < 25; j++)
                    {
                        fragments.push_back({ static_cast<double>(abs.health),
                                              static_cast<double>(rand() % 6) + fists[i].y,
                                              static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * 50.0,
                                              static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * 100.0 - 50.0,
                                              sf::seconds(0.5),
                                              fists[i].color_index,
                                              false});
                    }
                }
                else
                {

                    for(int j = 0; j < 50; j++)
                    {
                        fragments.push_back({ static_cast<double>(abs.health),
                                              static_cast<double>(rand() % 6) + fists[i].y,
                                              static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * 100.0 - 50.0,
                                              static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * 100.0 - 50.0,
                                              sf::seconds(0.5),
                                              config::RED,
                                              false});
                    }
                    abs.health--;
                    if(abs.health == 0)
                    {
                        engine->world->set_flag("thug_fight_outcome", -1);
                        running = false;
                    }
                }
                fists[i].dead = true;
            }
            fists[i].x -= 2;
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
    if(static_cast<double>(rand()) / static_cast<double>(RAND_MAX) >= 1.0 - 0.02 * (total_time - time_alive).asSeconds() / 180.0)
    {
        fists.push_back({ static_cast<double>(config::screen_w_chars - 1),
                          static_cast<double>(rand() % (config::screen_h_chars - 6)),
                          false,
                          sf::seconds(2),
                          rand() % (config::N_COLORS - config::RED) + config::RED,
                          false});
        spawn_countdown = (total_time.asSeconds() - time_alive.asSeconds()) / 180.0 + 0.1;
    }
    if(abs.tense && abs.remaining_time.asSeconds() <= 0.0)
        abs.tense = false;
    time_alive += dt;
    abs.power = std::min(5.0, abs.power + dt.asSeconds());
    abs.remaining_time -= dt;
}

void GameStateThugFight::draw(sf::RenderTarget* target)
{
    terminal->clr();
    std::vector<std::string> ab_strings = Parser::tokenize(abs.tense ? abs_tense_str : abs_str, '\n');
    for(int i = 0; i < ab_strings.size(); i++)
    {
        if(abs.tense)
            terminal->set_color(config::colors[config::ORANGE]);
        else if(static_cast<double>(config::screen_h_chars - i) / static_cast<double>(config::screen_h_chars)
                    <= abs.power / 5.0)
            terminal->set_color(config::colors[config::GREEN]);
        else
            terminal->set_color();
        terminal->output(abs.health - 5, i, ab_strings[i]);
    }
    terminal->set_color();
    for(int i = 0; i < fists.size(); i++)
    {
        terminal->set_color(config::colors[fists[i].color_index]);
        terminal->output((int)fists[i].x, (int)fists[i].y, thug_fist);
        terminal->set_color();
    }
    for(int i = 0; i < fragments.size(); i++)
    {
        terminal->set_color(config::colors[fragments[i].color_index]);
        terminal->output((int)fragments[i].x, (int)fragments[i].y, "*");
        terminal->set_color();
    }
    terminal->set_color();
    terminal->output(0, 0, "AB HEALTH:");
    terminal->output(13, 0, "AB STAMINA: ");
    terminal->draw(target);
}
