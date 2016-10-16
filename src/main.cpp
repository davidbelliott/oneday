#include "Config.h"
#include "Engine.h"
#include "Terminal.h"
#include "GameStateIntro.h"
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

    config::config_init();

    Terminal* terminal = new Terminal();
	Engine* engine = new Engine(terminal);



/*    for(int i = 0; i < config::N_COLORS; i++)
    {
        terminal->set_color(config::colors[config::ColorIndex(i)]);
        terminal->disp("The quick brown fox jumped over the lazy dog");
    }
    terminal->draw();
    terminal->pause();*/

    GameStateText* game_state_text = new GameStateText(engine);
    GameStateIntro* game_state_intro = new GameStateIntro(engine, game_state_text);
    engine->push_state(game_state_intro);

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
