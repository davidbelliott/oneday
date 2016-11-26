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
        player->clothing = "";
        world->add_child(player);
        world->set_player(player);
    }
	{
		Object* jamal_bedroom = new Object("jamal_bedroom");
        jamal_bedroom->pretty_name = "Jamal's bedroom";

        ComponentDescription* c_description = new ComponentDescription( "It's a wretched hovel.");
        jamal_bedroom->add_component(c_description);
        jamal_bedroom->add_component(new ComponentRoom({
                    {EAST, "jamal_corridor"},
                    {SOUTH, "jamal_bathroom"}
                    }));

        Object* posters = new Object("posters");
        posters->aliases = { "poster", "wall", "walls" };
        posters->add_component(new ComponentDescription("The walls are plastered with grimy old posters."));
        jamal_bedroom->add_child(posters);

		Object* window = new Object("window");
        window->add_component(new ComponentDescription("A single smeared window to the north suffuses the room in dim light.", "Looking through the window, you notice a gang of thugs gathered in front of your house."));
		jamal_bedroom->add_child(window);

        Object* paper = new Object("paper");
        paper->aliases = { "sheet" };
        paper->add_component(new ComponentDescription("A crumpled sheet of paper lies on the floor.", "There are words written on the paper."));
        paper->add_component(new ComponentText("Just waking up in the morning, gotta thank God\nI don't know but today seems kinda odd\nNo barking from the dog, no smog\nAnd momma cooked a breakfast with no hog."));
        paper->add_component(new ComponentTakeable());
        jamal_bedroom->add_child(paper);

		world->add_child(jamal_bedroom);
	}

	{
		Object* jamal_bathroom = new Object("jamal_bathroom");
        jamal_bathroom->pretty_name = "Jamal's bathroom";
		jamal_bathroom->add_component(new ComponentDescription("This is where you defecate daily. This cesuo is a reeking pigsty.\nOne door leads to the north."));
        jamal_bathroom->add_component(new ComponentRoom({{NORTH, "jamal_bedroom"}}));

		Object* hole = new Object("hole");
        hole->add_component(new ComponentDescription("A dark hole gapes in the floor, presumably where a toilet used to be."));
		hole->add_component(new ComponentPortal("sewer"));
        hole->pre_command = [=](Command* cmd)
        {
            bool execute = true;
            if(cmd->type == Command::GO)
            {
                if(/*world->get_player->clothing != "" && world->player->clothing == "hazmat"*/ true)
                {
                    text->send_front(std::make_shared<CmdDisp>("With the hazmat suit on, you tentatively step down into the hole and lower yourself into the murky water.\nIt rises gurgling to your neck.\nWith a desperate resignation, you plunge beneath the dark sewage."));
                    text->send_front(std::make_shared<CmdPause>());
                    execute = true;
                }
				else
				{
					text->send_front(std::make_shared<CmdDisp>("The hole is full of disgusting sewage water. You want to touch it with your bare skin? Kimochi warui~"));
                    execute = false;
				}
            }
            return execute;
        };
                
		jamal_bathroom->add_child(hole);

		world->add_child(jamal_bathroom);
	}

    {
        Object* jamal_corridor = new Object("jamal_corridor");
        jamal_corridor->pretty_name = "the Magick corridor";
        jamal_corridor->add_component(new ComponentDescription("This hallway is imbued with a strong Faerie Magick."));
        jamal_corridor->add_component(new ComponentRoom({
                    {NORTH, "jamal_house_block"},
                    {EAST, "jamal_staircase"},
                    {WEST, "jamal_bedroom"}
                    }));
        world->add_child(jamal_corridor);
    }

    {
        Object* staircase = new Object("jamal_staircase");
        staircase->pretty_name = "the staircase";
        staircase->add_component(new ComponentDescription("A rickety staircase descending into the basement."));
        staircase->add_component(new ComponentRoom({
                    {UP, "jamal_corridor"},
                    {DOWN, "library"}
                    }));
        world->add_child(staircase);
    }

    {
        Object* library = new Object("library");
        library->pretty_name = "Thomas Pynchon's library";
        library->add_component(new ComponentDescription("This subterranean den is where the father of realism does his stuff.\nHe is becoming a rap god."));
        ComponentRoom* c_room = new ComponentRoom({{NORTH, "jamal_staircase"}});
        library->add_component(c_room);

        Object* shelf = new Object("bookshelf");
        shelf->aliases = { "shelf" };
        ComponentDescription* c_desc = new ComponentDescription("A massive bookshelf covers the wall to the west.", "The shelf is full of books written by dead white men.");
        shelf->add_component(c_desc);
        library->add_child(shelf);

        Object* secret_switch = new Object("switch");
        secret_switch->active = false;
        secret_switch->add_component(new ComponentDescription("A secret switch is snug in the cuddly nook where a book used to be."));
        secret_switch->add_component(new ComponentHittable());
        secret_switch->post_command = [=](Command* cmd) {
            if(cmd->type == Command::HIT)
            {
                text->send_front(std::make_shared<CmdDisp>("Hitting the switch causes the bookshelf to slide to the side, revealing a doorway leading to the west."));
                c_desc->shallow_description = "A massive bookshelf is slid to one side of the west wall.";
                c_room->directions[WEST] = "henrik_lab";
            }
            return true;
        };
        shelf->add_child(secret_switch);

        Object* book = new Object("book");
        book->add_component(new ComponentDescription("One book protrudes farther than the rest."));
        book->add_component(new ComponentTakeable());
        book->post_command = [=](Command* cmd)
        {
            if(cmd->type == Command::TAKE)
            {
                text->send_front(std::make_shared<CmdDisp>("Taking the book reveals a secret switch."));
                c_desc->shallow_description += " One book is missing, leaving an empty slot.";
                c_desc->deep_description += " One book is missing, leaving an empty slot.";
                secret_switch->active = true;
                secret_switch->discovered = true;
            }
        };
        shelf->add_child(book);
        
        world->add_child(library);
    }




    {
        Object* temp_lane = new Object("temp_lane");
        temp_lane->pretty_name = "Temporary Lane";
        temp_lane->add_component(new ComponentRoom({{NORTH, "jamal_bedroom"}}));
        temp_lane->add_component(new ComponentDescription("A temporary lane."));
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
