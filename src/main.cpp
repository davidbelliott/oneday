#include "Config.h"
#include "Engine.h"
#include "GameStateText.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "One Day in the Life of Young Jamal");

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

    sf::Text test;
    test.setFont(config->font);
    test.setString("This is a test");
    test.setPosition(0, 0);
    window.clear();
    window.draw(test);
    window.display();
    float bounds = test.getLocalBounds().width;





	Engine* engine = new Engine(&window);
    GameStateText* game_state_text = new GameStateText(engine, config);
    engine->game_states.push_back(game_state_text);

    while(engine->running)
    {
        engine->get_input();
        sf::Time dt = sf::seconds(1.0f / 60.0f);
        engine->update(dt);
        engine->draw();
    }

	delete config;
	delete engine;
    delete game_state_text;

	return 0;
}
