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
	world->cur_room = "temp_lane";
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
        posters->add_component(new ComponentDescription("The walls are plastered with grimy old posters.", "The posters feature realistic depictions of mom's spaghetti."));
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
                if(((Player*)world->get_player())->clothing == "hazmat suit")
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
                    {WEST, "jamal_bedroom"},
                    {SOUTH, "jamal_kitchen"},
                    }));
        world->add_child(jamal_corridor);
    }

    {
        Object* kitchen = new Object("jamal_kitchen");
        kitchen->pretty_name = "the kitchen";
        kitchen->add_component(new ComponentDescription("This rickety cookery would make Martha Stewart turn over in her grave."));
        kitchen->add_component(new ComponentRoom({{NORTH, "jamal_corridor"}}));
        world->add_child(kitchen);

        Object* stove = new Object("stove");
        stove->add_component(new ComponentDescription("An electric stove leans against one wall."));
        kitchen->add_child(stove);

        Object* pan = new Object("pan");
        pan->add_component(new ComponentDescription("There's an aluminum pan on one burner."));
        pan->add_component(new ComponentTakeable());
        stove->add_child(pan);

        Object* pellets = new Object("pellets");
        pellets->add_component(new ComponentDescription("Several brown pellets are in the pan. They do not contain pork."));
        pellets->add_component(new ComponentTakeable());
        pan->add_child(pellets);
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
        library->add_component(new ComponentDescription("This subterranean den is where the father of realism does his stuff.\nHe is beginning to feel like a rap god."));
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
                c_room->directions[WEST] = "lab";
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
        Object* lab = new Object("lab");
        lab->pretty_name = "Thomas Pynchon's lab";
        lab->add_component(new ComponentDescription("This is the underground lab where Pynchon does his stuff."));
        lab->add_component(new ComponentRoom({{EAST, "library"}}));
        world->add_child(lab);

        Object* table = new Object("table");
        table->add_component(new ComponentDescription("A polished granite table supports several scientific instruments."));
        lab->add_child(table);

        Object* instruments = new Object("instruments");
        instruments->add_component(new ComponentDescription("You'd play the instruments if you knew how."));
        table->add_child(instruments);

        Object* lockers = new Object("lockers");
        lockers->add_component(new ComponentDescription("Three metal lockers line the wall."));
        lab->add_child(lockers);

        Object* hazmat = new Object("hazmat suit");
        hazmat->aliases = {"hazmat", "suit"};
        hazmat->add_component(new ComponentDescription("A hazmat suit is hanging inside one locker."));
        hazmat->add_component(new ComponentTakeable());
        hazmat->add_component(new ComponentWearable());
        lockers->add_child(hazmat);
    }


    {
        Object* sewer = new Object("sewer");
        sewer->add_component(new ComponentRoom({{WEST, "temp_lane"}}));
        sewer->add_component(new ComponentDescription("Filthy water flows around your ankles."));
        world->add_child(sewer);
    }

    {
        Object* temp_lane = new Object("temp_lane");
        temp_lane->pretty_name = "Temporary Lane";
        temp_lane->add_component(new ComponentRoom({{NORTH, "compton_street"}}));
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
                auto fn = [](GameState* g)
                {
                    if(g->engine->world->get_flag("thug_fight_outcome") == 1)  // Won the fight
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
                text->send_front(std::make_shared<CmdPause>());
                text->send_front(std::make_shared<CmdCustom>(fn));
            }
            return true;
        };
        world->add_child(temp_lane);
    }

    {
        Object* compton_street = new Object("compton_street");
        compton_street->pretty_name = "Compton Street";
        compton_street->add_component(new ComponentRoom({{NORTH, "lil_wayne_front"}, {EAST, "magdalene_lane"}, {SOUTH, "temp_lane"}, {WEST, "vacant_lot"}}));
        compton_street->add_component(new ComponentDescription("This strip of gritty asphalt comes straight outta the dark and unknown reaches of the city of Compton."));
        world->add_child(compton_street);

        Object* urban_youth = new Object("urban youth");
        urban_youth->aliases = {"youth", "man", "person", "guy", "dude", "urban", "stranger", "him"};
        urban_youth->add_component(new ComponentDescription("An urban youth stands in the corner, his hands tucked inside hoodie pockets."));
        urban_youth->add_component(new ComponentTalkable({">Why are you standing here?",
                    "-I'm here 4 da people, Jamal.",
                    ">W...what do you mean?",
                    "-The man has kept us down for 2 long, Jamal.",
                    "-You must report our grievances 2 the president.",
                    ">But how?",
                    "-Viper will host a rap contest in the Club. The winner goes 2 the White House.",
                    "-You must challenge him, and emerge the victor.",
                    "Yet this is truly a great responsibility, you think--",
                    "-Go forth with faith, Jamal."}));
        compton_street->add_child(urban_youth);
    }

    {
        Object* vacant_lot = new Object("vacant_lot");
        vacant_lot->pretty_name = "a Vacant Lot";
        vacant_lot->add_component(new ComponentRoom({{EAST, "compton_street"}}));
        vacant_lot->add_component(new ComponentDescription("This lot is empty."));
        world->add_child(vacant_lot);

        Object* grass = new Object("grass");
        grass->add_component(new ComponentDescription("The politics of bacteria, the soil's stringing of rings and chains in nets only God can tell the meshes of, have seen the grass thrive often to lengths of a foot and a half, yes amazing but true."));
        vacant_lot->add_child(grass);

        Object* magnet = new Object("magnet");
        magnet->add_component(new ComponentTakeable());
        magnet->add_component(new ComponentDescription("A magnet lies in the grass.", "Who can fathom the mysterious workings of the magnet?"));
        grass->add_child(magnet);
    }

    {
        Object* magdalene_lane = new Object("magdalene_lane");
        magdalene_lane->pretty_name = "Magdalene Lane";
        magdalene_lane->add_component(new ComponentRoom({{WEST, "compton_street"}, {NORTH, "garbage_alley"}, {EAST, "kolob_street"}}));
        magdalene_lane->add_component(new ComponentDescription("The red-light domain where everybody's soul is for sale."));
        world->add_child(magdalene_lane);
    }

    {
        Object* garbage_alley = new Object("garbage_alley");
        garbage_alley->pretty_name = "a dim alley overflowing with garbage.";
        garbage_alley->add_component(new ComponentRoom({{SOUTH, "magdalene_lane"}}));
        world->add_child(garbage_alley);

        Object* garbage_cans = new Object("garbage cans");
        garbage_cans->aliases = {"cans", "garbage", "rubbish", "bins", "can", "bin"};
        garbage_cans->add_component(new ComponentDescription("Several cans huddle by the wall in a pool of sodium-vapor light."));
        garbage_alley->add_child(garbage_cans);
    }

    {
        Object* kolob_street = new Object("kolob_street");
        kolob_street->pretty_name = "Kolob Street";
        kolob_street->add_component(new ComponentRoom({{SOUTH, "jamal_corridor"}, {WEST, "magdalene_lane"}}));
        world->add_child(kolob_street);
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
