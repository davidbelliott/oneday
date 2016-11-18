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
    abs({ 5, 0}),
    fists(),
    fragments(),
    thug_fist(""),
    abs_str(),
    abs_tense_str(),
    time_alive(sf::seconds(0)),
    total_time(sf::seconds(180)),
    time_since_spawn(sf::seconds(0)),
    spawn_beats(8),
    ab_height(6),
    beat(sf::seconds(60.0 / 150.0 / 2))
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
    if(event->type == Event::KEY_PRESSED)
    {
        sf::Keyboard::Key keycode = std::static_pointer_cast<EventKeyPressed>(event)->code;
        if(keycode == sf::Keyboard::A)
            abs.tense_ab = 0;
        else if(keycode == sf::Keyboard::S)
            abs.tense_ab = 1;
        else if(keycode == sf::Keyboard::D)
            abs.tense_ab = 2;
        else if(keycode == sf::Keyboard::F)
            abs.tense_ab = 3;
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
                if(abs.tense_ab == fists[i].y)
                {
                    for(int j = 0; j < 25; j++)
                    {
                        fragments.push_back({ static_cast<double>(abs.health),
                                              static_cast<double>(rand() % 6) + fists[i].y * ab_height,
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
                                              static_cast<double>(rand() % 6) + fists[i].y * ab_height,
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
    if(time_since_spawn.asSeconds() >= spawn_beats * beat.asSeconds())
    {
        int rand_offset = rand() % 3;
        if(!fists.empty() && rand_offset >= fists.back().y)
            rand_offset++;
        fists.push_back({ static_cast<double>(config::screen_w_chars - 1),
                          static_cast<double>(rand_offset % 4),
                          false,
                          sf::seconds(2),
                          rand() % (config::N_COLORS - config::RED) + config::RED,
                          false});
        time_since_spawn = sf::seconds(0);
        spawn_beats = static_cast<int>((total_time - time_alive) / (total_time) * 7.0) + 1;
    }
    time_alive += dt;
    
    time_since_spawn += dt;
}

void GameStateThugFight::draw(sf::RenderTarget* target)
{
    terminal->clr();
    for(int i = 0; i < 4; i++)
    {
        std::string ab_str_to_display = abs.tense_ab == i ? abs_tense_str : abs_str;
        if(abs.tense_ab == i)
            terminal->set_color(config::colors[config::ORANGE]);
        else
            terminal->set_color();
        terminal->output(abs.health - 5, i * ab_height, ab_str_to_display);
    }
    terminal->set_color();
    for(int i = 0; i < fists.size(); i++)
    {
        terminal->set_color(config::colors[fists[i].color_index]);
        terminal->output((int)fists[i].x, (int)fists[i].y * ab_height, thug_fist);
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
