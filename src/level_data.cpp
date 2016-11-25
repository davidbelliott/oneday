#include "level_data.h"
#include "Engine.h"
#include "Console.h"
#include "GameStateIntro.h"
#include "GameStateText.h"
#include "GameStateThugFight.h"
#include "Player.h"
#include "World.h"

void execute()
{
    Engine* engine = new Engine();
    GameStateText* text = new GameStateText(engine);

    // Generate the world
	World* world = engine->world;

	world->flags =
	{
		{ "alive", 1 },
		{ "health", 100 },
		{ "woke_up", 0 }
	};
	world->cur_room = "jamal_bedroom";
    {
        Player* player = new Player("Jamal", "a sturdy creature fond of drink and industry");
        world->player = player;
    }
	{
		Object* jamal_bedroom = new Object("jamal_bedroom");
        jamal_bedroom->pretty_name = "Jamal's bedroom";

        ComponentDescription* c_description = new ComponentDescription(
		    "The walls of this cluttered hovel are plastered with layers of grime and old posters.");
        jamal_bedroom->add_component(c_description);

        ComponentRoom* c_room = new ComponentRoom();
		c_room->directions[EAST] = "jamal_corridor";
		c_room->directions[SOUTH] = "temp_lane";
        jamal_bedroom->add_component(c_room);

		Object* window = new Object("window");
        window->add_component(new ComponentDescription("A single smeared window to the north suffuses the room in dim light."));
		//window->deep_description = "Looking through the window, you notice a gang of thugs gathered in front of your house.";
		jamal_bedroom->add_child(window);

        Object* paper = new Object("paper");
        paper->aliases = { "sheet", "sheeit" };
        paper->add_component(new ComponentDescription("A crumpled sheet of paper lies on the floor."));
        paper->add_component(new ComponentText("Just waking up in the morning, gotta thank God\nI don't know but today seems kinda odd\nNo barking from the dog, no smog\nAnd momma cooked a breakfast with no hog."));
        jamal_bedroom->add_child(paper);

		world->add_child(jamal_bedroom);
	}

	{
		Object* jamal_bathroom = new Object("jamal_bathroom");
        jamal_bathroom->pretty_name = "Jamal's bathroom";
		//jamal_bathroom->shallow_description = "This is where you defecate daily. This cesuo is a reeking pigsty.\nOne door leads to the north.";
		//jamal_bathroom->directions[NORTH] = "jamal_bedroom";

		/*Object* hole = new Object("hole", "A dark hole gapes in the floor, presumably where a toilet used to be.");
		hole->properties |= Object::GOABLE;
		hole->goable_data = "sewer";
		/*hole->pre_action = [](World* w, Receiver* r, Action* a, Object* o)
		{
			if (a->name.id == a->name.parent_list->GO)
			{
				if (w->player->clothing == "hazmat")
				{
					r->add_event(std::make_shared<CmdDisp>("With the hazmat suit on, you tentatively step down into the hole and lower yourself into the murky water.\nIt rises gurgling to your neck.\nWith a desperate resignation, you plunge beneath the dark sewage."));
                    r->add_event(std::make_shared<CmdPause>());
                    return true;
				}
				else
				{
					r->add_event(std::make_shared<CmdDisp>("The hole is full of disgusting sewage water. You want to touch it with your bare skin? Kimochi warui~"));
                    return false;
				}
			}
		};*/
		//jamal_bathroom->add_child(hole);

		world->add_child(jamal_bathroom);
	}

    {
        Object* temp_lane = new Object("temp_lane");
        temp_lane->pretty_name = "Temporary Lane";

        ComponentRoom* c_room = new ComponentRoom();
        c_room->directions[NORTH] = "jamal_bedroom";
        temp_lane->add_component(c_room);

        temp_lane->add_component(new ComponentDescription("A temporary lane."));
        //temp_lane->directions[NORTH] = "hood_avenue";
        temp_lane->pre_command = [=](Command* cmd)
        {
            if(cmd->type == Command::DESCRIBE && world->get_flag("thug_fight_outcome") == 0)
            {
                text->send_front(std::make_shared<CmdDisp>("Suddenly, a group of thugs rounds the corner. They raise fists to attack you!"));
                text->send_front(std::make_shared<CmdPause>());
                text->send_front(std::make_shared<CmdDisp>("Press any key to tense your abs and deflect their blows."));
                text->send_front(std::make_shared<CmdPause>());
                text->send_front(std::make_shared<CmdAddGameState>(new GameStateThugFight(engine)));
                auto fn = [=](GameState* g)
                {
                    if(world->get_flag("thugfight_outcome") == 1)  // Won the fight
                    {
                        g->send_front(std::make_shared<CmdDisp>("Cowed by your abdominal prowess, the thugs slink off."));
                    }
                    else
                    {
                        g->send_front(std::make_shared<CmdDisp>("Your abdomen is hard and tender from the repeated blows. You give up the ghost."));
                        g->send_front(std::make_shared<CmdPause>());
                        g->send_front(std::make_shared<CmdQuit>());
                    }
                };
                text->send_front(std::make_shared<CmdCustom>(fn));
            }
            return true;
        };
        world->add_child(temp_lane);
    }
    
    /*cmd_ptr describe = std::make_shared<CmdDescribe>();
    describe->add_object((Object*)engine->world->get_current_room());
    text->send(describe);*/
    //world->get_current_room()->describe(text);
    text->send(std::make_shared<CmdInput>());
    engine->push_state(text);

    sf::Clock clock;
    sf::Time dt;
    while(engine->running)
    {
        // Collect input from the user
        engine->get_input();

        // Let gamestates handle their pending events
        engine->execute_commands();

        // Update gamestates based on elapsed time
        dt = clock.restart();
        engine->update(dt);

        // Draw gamestates
        engine->draw();

        engine->prune();
        // Sleep for remaining time
        while(clock.getElapsedTime().asSeconds() < 1.0f / config::update_frequency)
        {
            sf::sleep(sf::milliseconds(1.0f));
        }
    }

    delete text;
	delete engine;
}
