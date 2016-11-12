#include "Console.h"
#include "Engine.h"

Console::Console(Engine* engine_in)
    : engine(engine_in)
{
}

Console::~Console()
{
}

void Console::get_input(sf::Window* window)
{
    sf::Event sf_event;
    while(window->pollEvent(sf_event))
    {
        event_ptr output_event = nullptr;
        if(sf_event.type == sf::Event::KeyPressed)
            output_event = std::make_shared<EventKeyPressed>(sf_event.key.code);
        else if(sf_event.type == sf::Event::TextEntered)
            output_event = std::make_shared<EventTextEntered>(static_cast<char>(sf_event.text.unicode));
        if(output_event)
            handle_input(output_event);
    }
}

void Console::handle_input(event_ptr event)
{
    if(event->type == Event::TEXT_ENTERED)
    {
        if(input)
        {
            char c = std::static_pointer_cast<EventTextEntered>(event)->c;
            if(c == '\n' || c == '\r')
            {
                engine->game_states.back()->terminal->disp("");
                engine->notify_gamestates(std::make_shared<EventUserLine>(cur_user_string));
                cur_user_string = "";
            }
            else if(c == '\b')
            {
                if(cur_user_string.length() > 0)
                {
                    engine->game_states.back()->terminal->backspace();
                    cur_user_string.pop_back();
                }
            }
            else
            {
                cur_user_string += c;
                std::string str = "";
                str += c;
                engine->game_states.back()->terminal->disp(str, false);
            }
        }
    }

    engine->notify_gamestates(event);
}
