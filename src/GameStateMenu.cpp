#include "GameStateMenu.h"
#include "GameStateThugFight.h"
#include "Parser.h"
#include "Terminal.h"

GameStateMenu::GameStateMenu(Engine* engine_in, std::string prompt_in,
        std::map<std::string, std::vector<cmd_ptr>> options_in)
    : GameState(engine_in),
      prompt(prompt_in),
      options(options_in)
{

}

GameStateMenu::~GameStateMenu()
{

}

void GameStateMenu::init()
{
    send(std::make_shared<CmdDisp>(prompt));
    send(std::make_shared<CmdInput>());
}

void GameStateMenu::cleanup()
{

}

void GameStateMenu::notify(event_ptr event)
{
    if(event->type == Event::USER_LINE)
    {
        std::string statement = std::static_pointer_cast<EventUserLine>(event)->line;
        arg_list args = {};
        bool found_option = false;
        for(auto it = options.begin(); it != options.end() && !found_option; ++it)
        {
            if(matches(tokenize(statement, ' '), it->first, args))
            {
                found_option = true;
                for(int i = 0; i < it->second.size(); i++)
                    send(it->second[i]);
            }
        }
        if(!found_option)
        {
            send(std::make_shared<CmdDisp>("I don't understand that."));
            send(std::make_shared<CmdInput>());
        }
        else
        {
            send(std::make_shared<CmdRemoveGameState>(this));
        }
    }
    terminal->notify(event);
}
