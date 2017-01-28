#include "GameStateSubway.h"
#include "GameStateThugFight.h"
#include "GameStateMenu.h"
#include "GameStateNotification.h"
#include "File.h"
#include "Audio.h"
#include "Engine.h"
#include "Terminal.h"
#include <iostream>

GameStateSubway::GameStateSubway(Engine* engine_in)
    : GameState(engine_in)
{
}

GameStateSubway::~GameStateSubway()
{
}

void GameStateSubway::load_beats()
{
    terrain.clear();
    Terrain this_terrain = NORMAL;
    for(int i = 0; i < total_beats; i++)
    {
        if(i > 4)
        {
            if(terrain[i - 1] == NORMAL || terrain[i - 2] == NORMAL)
                this_terrain = (Terrain)(rand() % 2);
            else
                this_terrain = NORMAL;
        }
        terrain.push_back(this_terrain);
    }
}

void GameStateSubway::init()
{
    cur_beat = 0;
    total_beats = 150;
    elapsed_time = sf::seconds(0.0);
    load_beats();
    beat = sf::seconds(60.0 / 165.5);
    player = { Player::NORMAL, 14.0, 0.0, get_file_contents("player"), get_file_contents("player_1"), 0 };
    engine->audio->play_music("res/platinum.ogg", sf::seconds(30.0));
    gravity = 90.0;
}

void GameStateSubway::cleanup()
{
    engine->audio->stop_music("res/platinum.ogg");
}

void GameStateSubway::notify(event_ptr event)
{
    if(!paused)
    {
        if(event->type == Event::KEY_PRESSED)
        {
            sf::Keyboard::Key keycode = std::static_pointer_cast<EventKeyPressed>(event)->code;

            if(keycode == sf::Keyboard::Up)
                jump();
            else if(keycode == sf::Keyboard::Down)
                duck();
            else if(keycode == sf::Keyboard::Q)
                lose();
            else if(keycode == sf::Keyboard::W)
                win();
        }
        if(event->type == Event::KEY_RELEASED)
        {
            sf::Keyboard::Key keycode = std::static_pointer_cast<EventKeyReleased>(event)->code;

            if(keycode == sf::Keyboard::Up && player.vy < 0.0)
                player.vy = 0.0;
        }
    }
    else if(event->type == Event::KEY_PRESSED && std::static_pointer_cast<EventKeyPressed>(event)->code == sf::Keyboard::Return)
        paused = false;
}

int GameStateSubway::get_seconds_remaining()
{
    return std::max(0, round((total_beats - cur_beat) * beat.asSeconds()));
}

int GameStateSubway::get_x(int x_beat)
{
    int offset = x_beat * 10;
    int cur_pos = int((cur_beat - 2) * 10);
    return offset - cur_pos;
}


void GameStateSubway::update(sf::Time dt)
{
    if(!paused)
    {
        if(cur_beat > total_beats)
        {
            win();
        }

        if(terrain[(int)cur_beat] == NORMAL
                && player.y >= config::screen_h_chars - 10
                && player.y <= config::screen_h_chars - 8
                && player.vy >= 0.0)
        {
            std::cout<<"normal"<<std::endl;
            player.vy = 0.0;
            player.y = config::screen_h_chars - 10;
            player.state = Player::NORMAL;
        }
        else if(player.y >= config::screen_h_chars)
            lose();
        else
        {
            player.vy += dt.asSeconds() * gravity;
            player.y += player.vy * dt.asSeconds();
            player.state = Player::FALL;
        }
        
        elapsed_time += dt;
        cur_beat = (elapsed_time.asSeconds() / beat.asSeconds());
    }
}

void GameStateSubway::draw()
{
    engine->terminal->clr();

    // Draw walls
    for(int x, y, i = std::max(0, (int)cur_beat - 2); i < std::min(total_beats, (int)cur_beat + 7); i++)
    {
        x = get_x(i);
        std::string bot = (terrain[i] == NORMAL ? "##########" : "");
        engine->terminal->output(x, config::screen_h_chars - 10, bot);
    }

    // Draw player
    engine->terminal->output(20, (int)player.y - 3, (int(player.n_draws++ / 5) % 2 == 0 ? player.sprite : player.sprite_1));

    engine->terminal->output(0, 0, "TIME REMAINING: " + std::to_string(get_seconds_remaining()));
}

void GameStateSubway::jump()
{
    if(terrain[(int)cur_beat] == NORMAL  && player.state == Player::NORMAL)
    {
        player.vy = -40.0;
    }
    else
        std::cout << player.state << std::endl;
}

void GameStateSubway::duck()
{
    player.state = Player::DUCK;
}

void GameStateSubway::win()
{
    send_front(std::make_shared<CmdRemoveGameState>(this));
    send_front(std::make_shared<CmdClear>());
    send_front(std::make_shared<CmdAddGameState>(new GameStateNotification(engine, "At last, the subway rumbles into the station, and you disembark safely.")));
}

void GameStateSubway::lose()
{
    send(std::make_shared<CmdRemoveGameState>(this));
    send(std::make_shared<CmdClear>());
    send(std::make_shared<CmdAddGameState>(new GameStateMenu(engine,
                    "Your frail body is crushed, and you expire.\nYou had "
                    + std::to_string(get_seconds_remaining())
                    + " seconds remaining.\nTry again? (y/n)",
                    {{"y", {std::make_shared<CmdAddGameState>(new GameStateSubway(engine))}},
                    {"n", {std::make_shared<CmdQuit>()}}})));
}

