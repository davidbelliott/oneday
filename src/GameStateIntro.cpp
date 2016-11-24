#include "GameStateIntro.h"
#include "Event.h"
#include "Engine.h"
#include "Config.h"
#include "Terminal.h"
#include "File.h"

GameStateIntro::GameStateIntro(Engine* engine_in)
    : GameState(engine_in),
    title_string(get_file_contents("jamal.txt"))
{
    running = true;
    send(std::make_shared<CmdOutput>(0, 0, "================================================================================"));
    send(std::make_shared<CmdOutput>(0, 1, "One Day in the Life of"));
    std::shared_ptr<CmdOutput> output = std::make_shared<CmdOutput>(0, 2, title_string);
    output->spread = 100;
    send(output);
    send(std::make_shared<CmdOutput>(0, 9, "================================================================================"));
    send(std::make_shared<CmdOutput>(0, 10, "Produced by moraine"));
    /*int x = 0;
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
    }*/
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
}

void GameStateIntro::draw(sf::RenderTarget* target)
{
    terminal->draw(target);
}
