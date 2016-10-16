#include "Config.h"
#include "Engine.h"
#include "Terminal.h"
#include "GameStateText.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{

	/*while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.display();
    }*/


	Config* config = new Config();
    Terminal* terminal = new Terminal(config);
	Engine* engine = new Engine(terminal);
    GameStateText* game_state_text = new GameStateText(engine, config);
    engine->game_states.push_back(game_state_text);

    sf::Clock clock;
    sf::Time dt;
    while(engine->running)
    {
        dt = clock.restart();
        std::cout << "Time:" << dt.asSeconds() << std::endl;
        engine->run(dt);
        engine->draw();
        while(clock.getElapsedTime().asSeconds() < 1.0f / config->update_frequency)
        {
            sf::sleep(sf::milliseconds(1.0f));
        }
    }

	delete config;
    delete terminal;
	delete engine;
    delete game_state_text;

	return 0;
}
