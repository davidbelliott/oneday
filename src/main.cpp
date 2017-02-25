#include "Engine.h"
#include "Terminal.h"
#include "World.h"
#include "GameStateText.h"
#include "level_data.h"

#include <caca.h>
//#include <common-image.h>

int main(int argc, char *argv[])
{
    Engine* engine = new Engine();
    World* world = generate_world(engine);
    GameStateText* text = new GameStateText(engine);

    text->world = world;
    engine->push_state(text);

    unsigned int dt;
    while(engine->running)
    {
        // Collect input from the user
        //engine->get_input();

        // Update gamestates based on elapsed time
        dt = 1;
        engine->update(dt);

        // Draw gamestates
        engine->draw();

        engine->prune();
        // Sleep for remaining time
    }
    //engine->terminal->disp("Test");
    //engine->terminal->refresh_display();
    //engine->terminal->pause();

	delete engine;
    delete world;
    delete text;

    return 0;
}

