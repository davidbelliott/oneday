#include "GameStateText.h"
#include "Action.h"
#include "Event.h"
#include "Parser.h"
#include "Room.h"
#include "Terminal.h"
#include "World.h"
#include "Config.h"
#include "level_data.h"

GameStateText::GameStateText(Engine* engine_in, Config* config_in)
    : GameState(engine_in), cur_user_string("")
{
    world = generate_world();
    terminal = new Terminal(config_in);
    parser = new Parser();
    Action* change_room_action = parser->action_factory.create_action(parser->word_list.get_word("look"));
    world->change_room_action = change_room_action;
	world->set_current_room(world->get_current_room()->name.word, terminal);
    terminal->prompt_input();
}

GameStateText::~GameStateText()
{
    delete world;
    delete terminal;
    delete parser;
}

void GameStateText::init()
{
}

void GameStateText::cleanup()
{
}

void GameStateText::handle_event(Event* event)
{
    if(event->type == Event::SFML && event->sfml_event_data.sf_event.type == sf::Event::TextEntered)
    {
        if(event->sfml_event_data.sf_event.text.unicode < 128)
        {
            char c = static_cast<char>(event->sfml_event_data.sf_event.text.unicode);
            if(c == '\n' || c == '\r')
            {
                terminal->set_color();
                terminal->disp("");
                Action* action = parser->parse(cur_user_string, world, terminal);
                if(action)
                    action->run(world, terminal);
                cur_user_string = "";
                terminal->prompt_input();
            }
            else if(c == '\b')
            {
                if(cur_user_string.length() > 0)
                {
                    terminal->backspace();
                    cur_user_string.pop_back();
                }
            }
            else
            {
                cur_user_string += c;
                std::string str = "";
                str += c;
                terminal->disp(str, false);
            }
        }
    }
    if(!world->active)
        running = false;
}

void GameStateText::update(sf::Time dt)
{

}

void GameStateText::draw(sf::RenderTarget* target)
{
    terminal->draw(target);
}
