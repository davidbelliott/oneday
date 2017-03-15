#include "Engine.h"
#include "Terminal.h"
#include "World.h"
#include "GameStateText.h"
#include "level_data.h"

#include <thread>
//#include <common-image.h>
//
void update_audio(Engine* engine)
{
    sf::Clock clock;
    clock.restart();
    while(engine->running)
    {
        sf::Time elapsed = clock.restart();
        engine->update_audio(elapsed);
        sf::sleep(sf::seconds(0.1));
    }
}

int main(int argc, char *argv[])
{
    Engine* engine = new Engine();
    World* world = generate_world(engine);
    GameStateText* text = new GameStateText(engine);

    text->world = world;
    engine->push_state(text);

    std::thread audio_thread(update_audio, engine);

    while(engine->running)
    {
        // Collect input from the user
        // Update gamestates based on elapsed time
        engine->update(1);

        // Draw gamestates
        engine->draw();

        engine->prune();
        // Sleep for remaining time
    }

    audio_thread.join();
    //engine->terminal->disp("Test");
    //engine->terminal->refresh_display();
    //engine->terminal->pause();

	delete engine;
    delete world;
    delete text;

    return 0;
}

