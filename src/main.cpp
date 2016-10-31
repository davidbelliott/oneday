#include "Config.h"
#include "Engine.h"
#include "Terminal.h"
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

    GameStateText* game_state_text = new GameStateText(engine);
    GameStateIntro* game_state_intro = new GameStateIntro(engine);
    engine->push_state(game_state_text);
    //engine->push_state(game_state_intro);

    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(config::window_width, config::window_height), "One Day in the Life of Young Jamal");

    sf::Clock clock;
    sf::Time dt;
    while(engine->running)
    {
        engine->get_input(window);
        engine->handle_events();
        dt = clock.restart();
        engine->run(dt);
        engine->draw(window);
        while(clock.getElapsedTime().asSeconds() < 1.0f / config::update_frequency)
        {
            sf::sleep(sf::milliseconds(1.0f));
        }
    }

	delete engine;
    delete game_state_text;
    delete window;

	return 0;
}
