#include "GameStateIntro.h"
#include "Event.h"
#include "Engine.h"
#include "Config.h"
#include "Terminal.h"
#include "File.h"

LerpingChar::LerpingChar(char c_in, int target_x_in, int target_y_in)
    : target_x(target_x_in),
      target_y(target_y_in),
      cur_x(0),
      cur_y(0),
      c(std::string("") + c_in),
      lerp_coefficient(4.0),
      snap_threshold(0.5)
{
    cur_x = target_x + rand() % 100 - 50;
    cur_y = target_y + rand() % 100 - 50;
    /*if(cur_x > 49)
        cur_x += config::screen_w_chars;
    else
        cur_x -= config::screen_w_chars;
    if(cur_y > 49)
        cur_y += config::screen_h_chars;
    else
        cur_y -= config::screen_h_chars;*/
}

void LerpingChar::update(sf::Time dt)
{
    if(abs(cur_x - target_x) >= snap_threshold)
        cur_x += ((float)target_x - cur_x) * std::min((float)1.0, lerp_coefficient * dt.asSeconds());
    else
        cur_x = target_x;

    if(abs(cur_y - target_y) >= snap_threshold)
        cur_y += ((float)target_y - cur_y) * std::min((float)1.0, lerp_coefficient * dt.asSeconds());
    else
        cur_y = target_y;
}

void LerpingChar::print(Terminal* t)
{
    t->output(int(cur_x), int(cur_y), c);
}

GameStateIntro::GameStateIntro(Engine* engine_in)
    : GameState(engine_in),
    title_string(get_file_contents("jamal.txt"))
{
    running = true;
    int x = 0;
    int y = 0;
    for(int i = 0; i < title_string.size(); i++)
    {
        if(title_string[i] == '\n')
        {
            x = 0;
            y++;
        }
        else if(title_string[i] == ' ')
        {
            x++;
        }
        else
        {
            chars.push_back({ title_string[i], x, y });
            x++;
        }
    }
    /*send(std::make_shared<CmdOutput>(0, 0, title_string));
    send(std::make_shared<CmdPause>());
    send(std::make_shared<CmdClear>());
    send(std::make_shared<CmdDisp>("-You wake."));
    send(std::make_shared<CmdPause>());
    send(std::make_shared<CmdDisp>("-No canine utterances grace your ears,\n and you can smell no fresh bacon cooking in the kitchen."));
    */
    send(std::make_shared<CmdPause>());
}

GameStateIntro::~GameStateIntro()
{
}

void GameStateIntro::update(sf::Time dt)
{
    if(!paused && commands.empty())
        running = false;




    for(int i = 0; i < chars.size(); i++)
    {
        chars[i].update(dt);
    }

}

void GameStateIntro::draw(sf::RenderTarget* target)
{
    terminal->clr();
    for(int i = 0; i < chars.size(); i++)
    {
        chars[i].print(terminal);
    }
    terminal->draw(target);
}
