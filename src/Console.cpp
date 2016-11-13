#include "Console.h"
#include "GameState.h"
#include "Terminal.h"
#include "Engine.h"

Console::Console(Engine* engine_in)
    : engine(engine_in),
      input(true)
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
            engine->notify_gamestates(output_event);
    }
}
