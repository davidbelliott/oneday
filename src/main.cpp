#include "Config.h"
#include "Engine.h"
#include "Parser.h"
#include "World.h"
#include "Terminal.h"
#include "level_data.h"
#include <SFML/Graphics.hpp>
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
	Terminal* terminal = new Terminal(config);

    int x = 0;
    int y = 0;
    terminal->output("Testing", x, y);

	Parser* parser = new Parser();
	World* world = generate_world();
	{
		Action* change_room_action = parser->action_factory.create_action(parser->word_list.get_word("look"));
		world->change_room_action = change_room_action;
	}
	Engine* engine = new Engine(world, terminal, parser);
	engine->run();

	delete config;
	delete world;
	delete terminal;
	delete parser;
	delete engine;

	return 0;
}
