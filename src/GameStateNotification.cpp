#include "GameStateNotification.h"
#include "Terminal.h"

GameStateNotification::GameStateNotification(Engine* engine_in, std::string text_in)
    : GameState(engine_in),
      text(text_in)
{
}

GameStateNotification::~GameStateNotification()
{
}

void GameStateNotification::init()
{
    running = true;
    terminal->disp(text);
}

void GameStateNotification::cleanup()
{
}

void GameStateNotification::notify(event_ptr event)
{
    if(event->type == Event::KEY_PRESSED && std::static_pointer_cast<EventKeyPressed>(event)->code == sf::Keyboard::Return)
        running = false;
    terminal->notify(event);
}
