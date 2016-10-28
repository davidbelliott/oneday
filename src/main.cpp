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

    Terminal* terminal = new Terminal();
	Engine* engine = new Engine(terminal);

    GameStateText* game_state_text = new GameStateText(engine);
    engine->push_state(game_state_text);

    sf::Clock clock;
    sf::Time dt;
    while(engine->running)
    {
        engine->get_input();
        dt = clock.restart();
        engine->run(dt);
        engine->draw();
        while(clock.getElapsedTime().asSeconds() < 1.0f / config::update_frequency)
        {
            sf::sleep(sf::milliseconds(1.0f));
        }
    }

    delete terminal;
	delete engine;
    delete game_state_text;

	return 0;
}
