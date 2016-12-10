#include "GameStateThugFight.h"
#include "GameStateMenu.h"
#include "GameStateNotification.h"
#include "Event.h"
#include "Engine.h"
#include "World.h"
#include "Terminal.h"
#include "Parser.h"
#include "File.h"
#include <fstream>
#include <algorithm>

int round(double a)
{
    return static_cast<int>((a + 0.5)/2);
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
    abs = { 5, 0};
    fists = {};
    fragments = {};
    time_alive = sf::seconds(0);
    total_time = sf::seconds(120);
    time_since_spawn = sf::seconds(100);
    beats_to_wait = 1;
    spawn_beats = 8;
    ab_height = 6;
    beat = sf::seconds(60.0 / 165.5 );
    music = new Music("res/100kilos.ogg");
    thug_fist = get_file_contents("fist.txt");
    abs_str = get_file_contents("abs.txt");
    abs_tense_str = get_file_contents("abs_tense.txt");
    send(std::make_shared<CmdPlayMusic>(music));
}

void GameStateThugFight::cleanup()
{
    std::make_shared<CmdStopMusic>(music)->run(this);
}

void GameStateThugFight::notify(event_ptr event)
{
    if(!paused)
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
            else if(keycode == sf::Keyboard::Q)
                lose();
        }
    }
    else if(event->type == Event::KEY_PRESSED && std::static_pointer_cast<EventKeyPressed>(event)->code == sf::Keyboard::Return)
    {
        paused = false;
    }
    terminal->notify(event);
}

void GameStateThugFight::update(sf::Time dt)
{
    if(!paused)
    {
        for(int i = 0; i < fists.size(); )
        {
            if(fists[i].punching)
            {
                if(fists[i].x < abs.health + 1)
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
                                                  false});
                        }
                        fists[i].dead = true;
                    }
                    else if(fists[i].x < abs.health)
                    {
                        abs.health--;
                        if(abs.health == 0)
                        {
                            lose();
                        }
                        fists[i].dead = true;
                    }
                }
                fists[i].x -= 2;
            }
            else
            {
                fists[i].x -= (dt.asSeconds() / 2.0 * 13.0);
            }


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
        if(time_alive >= total_time)
        {
            if(fists.size() == 0)
            {
                win();
            }
        }
        else if(time_since_spawn.asSeconds() >= beats_to_wait * beat.asSeconds())
        {
            int rand_offset = rand() % 3;
            if(!fists.empty() && rand_offset >= fists.back().y)
                rand_offset++;
            fists.push_back({ static_cast<double>(config::screen_w_chars - 1),
                              static_cast<double>(rand_offset % 4),
                              false,
                              sf::seconds(2),
                              false});
            time_since_spawn = sf::seconds(0);
            spawn_beats = round(8.0 / (0.025 * time_alive.asSeconds() + 1.0) + 1.2);
            beats_to_wait = rand() % spawn_beats + 1;
        }

        time_alive += dt;
        time_since_spawn += dt;
    }
}

void GameStateThugFight::draw(sf::RenderTarget* target)
{
    terminal->clr();
    for(int i = 0; i < 4; i++)
    {
        std::string ab_str_to_display = abs.tense_ab == i ? abs_tense_str : abs_str;
        /*if(abs.tense_ab == i)
            terminal->set_color(config::colors[config::ORANGE]);
        else
            terminal->set_color();*/
        terminal->output(abs.health - 5, i * ab_height, ab_str_to_display);
    }
    //terminal->set_color();
    for(int i = 0; i < fists.size(); i++)
    {
        //terminal->set_color(config::colors[fists[i].color_index]);
        terminal->output((int)fists[i].x, (int)fists[i].y * ab_height, thug_fist); //terminal->set_color();
    }
    for(int i = 0; i < fragments.size(); i++)
    {
        //terminal->set_color(config::colors[fragments[i].color_index]);
        terminal->output((int)fragments[i].x, (int)fragments[i].y, "*");
        //terminal->set_color();
    }
    //terminal->set_color();
    terminal->output(0, 0, "TIME REMAINING: " + std::to_string(std::max(0, int(total_time.asSeconds() - time_alive.asSeconds()))));
    terminal->draw(target);
}

void GameStateThugFight::win()
{
    send_front(std::make_shared<CmdRemoveGameState>(this));
    send_front(std::make_shared<CmdAddGameState>(new GameStateNotification(engine, "Cowed by your abdominal prowess, the thugs slink off.")));
}

void GameStateThugFight::lose()
{
    send(std::make_shared<CmdRemoveGameState>(this));
    send(std::make_shared<CmdAddGameState>(new GameStateMenu(engine,
                    "Your abdomen is hard and tender from the repeated blows. You give up the ghost.\nYou had "
                    + std::to_string(std::max(0, int(total_time.asSeconds() - time_alive.asSeconds())))
                    + " seconds remaining.\nTry again? (y/n)",
                    {{"y", {std::make_shared<CmdAddGameState>(new GameStateThugFight(engine))}},
                    {"n", {std::make_shared<CmdQuit>()}}})));
}
