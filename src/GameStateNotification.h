#pragma once
#include "GameState.h"

class GameStateNotification : public GameState
{
    public:
        std::string text;

        GameStateNotification(Engine* engine_in, std::string text_in);
        virtual ~GameStateNotification();

        virtual void init();
        virtual void cleanup();
        virtual void notify(event_ptr event);
};
