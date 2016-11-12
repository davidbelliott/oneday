#include "Config.h"
#include "Engine.h"
#include "Console.h"
#include "GameStateIntro.h"
#include "GameStateText.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <thread>

int main()
{
    config::config_init();

    Engine* engine = new Engine();
    Console* console = new Console(engine);
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(config::window_width, config::window_height), "One Day in the Life of Young Jamal");

    GameStateIntro* intro = new GameStateIntro(engine);
    GameStateText* text = new GameStateText(engine);
    engine->push_state(text);
    engine->push_state(intro);

    sf::Clock clock;
    sf::Time dt;
    while(engine->running)
    {
        // Collect input from the user
        console->get_input(window);

        // Let gamestates handle their pending events
        engine->execute_commands();

        // Update gamestates based on elapsed time
        dt = clock.restart();
        engine->update(dt);

        // Draw gamestates
        engine->draw(window);
        window->display();

        engine->prune();

        // Sleep for remaining time
        while(clock.getElapsedTime().asSeconds() < 1.0f / config::update_frequency)
        {
            sf::sleep(sf::milliseconds(1.0f));
        }
    }

    delete text;
    delete intro;

	delete engine;
    delete console;
    delete window;

	return 0;
}
