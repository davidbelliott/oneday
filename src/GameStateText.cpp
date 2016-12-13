#include "GameStateText.h"
#include "Engine.h"
#include "Terminal.h"
#include "World.h"
#include "File.h"

GameStateText::GameStateText(Engine* engine_in)
    : GameState(engine_in),
      parser(new Parser()),
      line("")
{
}

GameStateText::~GameStateText()
{
    delete parser;
}

void GameStateText::init()
{
    std::string title_string(get_file_contents("jamal.txt"));
    running = true;
    world->set_current_room(world->cur_room);
    send(std::make_shared<CmdOutput>(0, 0, "One Day in the Life of"));
    std::shared_ptr<CmdOutput> output = std::make_shared<CmdOutput>(0, 1, title_string);
    output->spread = 100;
    send(output);
    send(std::make_shared<CmdDisp>("\n\n\n\n\n\n\n"));
    send(std::make_shared<CmdDisp>("You wake."));
    send(std::make_shared<CmdDisp>("No canine utterances grace your ears,\nand you can smell no fresh bacon cooking in the kitchen."));
    send(std::make_shared<CmdDisp>("You should look around."));
    send(std::make_shared<CmdInput>());
}

void GameStateText::cleanup()
{

}

void GameStateText::notify(event_ptr event)
{
    if(paused)
    {
        if(event->type == Event::KEY_PRESSED && std::static_pointer_cast<EventKeyPressed>(event)->code == sf::Keyboard::Return)
            unpause();
    }
    else if(event->type == Event::USER_LINE)
    {
        line = std::static_pointer_cast<EventUserLine>(event)->line;
        if(line == "")
            send(std::make_shared<CmdDisp>("-say something, pls"));
        else
        {
            std::vector<cmd_ptr> commands = parser->parse(line, this);
            for(int i = 0; i < commands.size(); i++)
            {
                send(commands[i]);
            }
        }
        send(std::make_shared<CmdInput>());
    }
}

void GameStateText::update(sf::Time dt)
{
    world->update(dt);
}
