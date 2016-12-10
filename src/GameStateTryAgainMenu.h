#pragma once
#include "GameState.h"

class GameStateTryAgainMenu : public GameState
{
    public:
        GameStateTryAgainMenu(Engine* engine_in);
        virtual ~GameStateTryAgainMenu();

        virtual void init();
        virtual void cleanup();

        virtual void notify(event_ptr event);
};
