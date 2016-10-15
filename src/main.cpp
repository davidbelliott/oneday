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

    while(engine->running)
    {
        sf::Time dt = sf::seconds(1.0f / 60.0f);
        engine->run(dt);
        engine->draw();
    }

	delete config;
    delete terminal;
	delete engine;
    delete game_state_text;

	return 0;
}
