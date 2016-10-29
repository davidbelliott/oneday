#include "GameStateText.h"
#include "Action.h"
#include "Event.h"
#include "Parser.h"
#include "Room.h"
#include "Terminal.h"
#include "World.h"
#include "Config.h"
#include "level_data.h"
#include "Engine.h"

GameStateText::GameStateText(Engine* engine_in)
    : GameState(engine_in)
{
}

GameStateText::~GameStateText()
{
}

void GameStateText::init()
{
    engine->register_sink(this, Event::SFML);
    running = true;
    cur_user_string = "";
    world = generate_world();
    parser = new Parser();
    Action* change_room_action = parser->action_factory.create_action(parser->word_list.get_word("look"));
    world->change_room_action = change_room_action;
	world->set_current_room(world->get_current_room()->name.word, engine);
    start_input();
}

void GameStateText::cleanup()
{
    engine->unregister_sink(this, Event::SFML);
    delete world;
    delete parser;
    end_input();
    engine->terminal->clr();
}

void GameStateText::start_input()
{
    Terminal* terminal = engine->terminal;
    terminal->set_disp_cursor(true);
    terminal->set_color(config::colors[config::color_user_input]);
    terminal->disp(">", false);
}

void GameStateText::end_input()
{
    Terminal* terminal = engine->terminal;
    terminal->set_disp_cursor(false);
    terminal->set_color(config::colors[config::color_default_fg]);
}

void GameStateText::notify(Event* event)
{
    Terminal* terminal = engine->terminal;
    if(event->type == Event::SFML)
    {
        sf::Event* sf_event = &event->sfml_event_data.sf_event;
        if(sf_event->type == sf::Event::TextEntered)
        {
            if(sf_event->text.unicode < 128)
            {
                char c = static_cast<char>(sf_event->text.unicode);
                if(c == '\n' || c == '\r')
                {
                    terminal->set_color();
                    terminal->disp("");
                    end_input();

                    Action* action = parser->parse(cur_user_string, world, engine->terminal);
                    if(action)
                        action->run(world, engine);
                    if(!world->active)
                        running = false;

                    cur_user_string = "";
                    start_input();
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
        else if(sf_event->type == sf::Event::MouseWheelScrolled)
        {
            int scroll_delta = -sf_event->mouseWheelScroll.delta;
            terminal->buffer->scroll(scroll_delta);
        }
    }
}

void GameStateText::run(sf::Time dt)
{
}

void GameStateText::draw(sf::RenderTarget* target)
{
}
