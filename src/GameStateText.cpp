#include "GameStateText.h"
#include "Engine.h"
#include "Terminal.h"
#include "World.h"
#include "File.h"

GameStateText::GameStateText(Engine* engine_in)
    : GameState(engine_in),
      parser(new Parser())
{
    std::string title_string(get_file_contents("jamal.txt"));
    running = true;
    send(std::make_shared<CmdOutput>(0, 0, "One Day in the Life of"));
    std::shared_ptr<CmdOutput> output = std::make_shared<CmdOutput>(0, 1, title_string);
    output->spread = 100;
    send(output);
    send(std::make_shared<CmdDisp>("\n\n\n\n\n\n\n"));
    send(std::make_shared<CmdDisp>("You wake."));
    send(std::make_shared<CmdDisp>("No canine utterances grace your ears,\nand you can smell no fresh bacon cooking in the kitchen."));
}

GameStateText::~GameStateText()
{
    delete parser;
}
/*
void GameStateText::init()
{
    running = true;
    send(std::make_shared<CmdOutput>(0, 0, "================================================================================"));
    send(std::make_shared<CmdOutput>(0, 1, "One Day in the Life of"));
    std::shared_ptr<CmdOutput> output = std::make_shared<CmdOutput>(0, 2, title_string);
    output->spread = 100;
    send(output);
    send(std::make_shared<CmdOutput>(0, 9, "================================================================================"));
    send(std::make_shared<CmdOutput>(0, 10, "Produced by moraine"));
    parser = new Parser();
	engine->get_world()->set_current_room(world->get_current_room()->name.word, this);
    add_event(std::make_shared<CmdInput>());
}*/

void GameStateText::notify(event_ptr event)
{
    if(paused)
    {
        if(event->type == Event::KEY_PRESSED && std::static_pointer_cast<EventKeyPressed>(event)->code == sf::Keyboard::Return)
            unpause();
    }
    else if(event->type == Event::USER_LINE)
    {
        std::string line = std::static_pointer_cast<EventUserLine>(event)->line;
        if(line == "")
            send(std::make_shared<CmdDisp>("-say something, pls"));
        else
        {
            ParseOutcome outcome = UNKNOWN;
            InstructionPtr instruction = parser->parse(line, this, &outcome);
            if(instruction)
            {
                std::vector<cmd_ptr> commands = instruction->compile(this);
                for(int i = 0; i < commands.size(); i++)
                {
                    send(commands[i]);
                }
            }
            if(outcome == UNKNOWN)
            {
                send(std::make_shared<CmdDisp>("-what u talkin bout boi?"));
            }
            else if(outcome == UNKNOWN_VERB)
            {
                send(std::make_shared<CmdDisp>("-
        }
        send(std::make_shared<CmdInput>());
    }
    terminal->notify(event);
}
