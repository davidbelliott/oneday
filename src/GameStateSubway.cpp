#include "GameStateSubway.h"
#include "GameStateThugFight.h"
#include "GameStateMenu.h"
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
    Terrain last_terrain = NORMAL;
    Terrain this_terrain = NORMAL;
    for(int i = 0; i < total_beats; i++)
    {
        if(i < 5)
            this_terrain = NORMAL;
        else
        {
            if(last_terrain == NORMAL)
                this_terrain = (Terrain)(rand() % 3);
            else if(last_terrain == LOW_CEILING)
                this_terrain = (rand() % 2 == 0 ? NORMAL : LOW_CEILING);
            else if(last_terrain == NO_FLOOR)
            {
                if(i >= 2 && terrain[i - 2] == NO_FLOOR)
                    this_terrain = NORMAL;
                else
                    this_terrain = (rand() % 2 == 0 ? NORMAL : NO_FLOOR);
            }
        }
        terrain.push_back(this_terrain);
        last_terrain = this_terrain;
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
    music = new Music("res/subway.ogg");
    gravity = 90.0;
    send(std::make_shared<CmdPlayMusic>(music));
}

void GameStateSubway::cleanup()
{
    std::make_shared<CmdStopMusic>(music)->run(this);
}

void GameStateSubway::notify(event_ptr event)
{
    if(!paused)
    {
        if(event->type == Event::KEY_PRESSED)
        {
            sf::Keyboard::Key keycode = std::static_pointer_cast<EventKeyPressed>(event)->code;

            if(keycode == sf::Keyboard::A)
                jump();
            else if(keycode == sf::Keyboard::Down)
                duck();
            else if(keycode == sf::Keyboard::Q)
                lose();
            else if(keycode == sf::Keyboard::W)
                win();
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

        if((terrain[(int)cur_beat] == NORMAL
                   || terrain[(int)cur_beat] == LOW_CEILING)
                && player.y >= config::screen_h_chars - 10
                && player.y <= config::screen_h_chars - 8
                && player.vy >= 0.0)
        {
            std::cout<<"normal"<<std::endl;
            player.vy = 0.0;
            player.y = config::screen_h_chars - 10;
            player.state = Player::NORMAL;
        }
        else if(terrain[(int)cur_beat] == LOW_CEILING
                && player.y <= config::screen_h_chars - 8)
            lose();
        else if(player.y >= config::screen_h_chars)
            lose();
        else
        {
            player.state = Player::FALL;
            player.vy += dt.asSeconds() * gravity;
            player.y += player.vy * dt.asSeconds();
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
        std::string top;
        std::string bot;
        if(terrain[i] == NORMAL)
        {
            top = "";
            bot = "##########";
        }
        else if(terrain[i] == LOW_CEILING)
        {
            top = "##########";
            bot = "##########";
        }
        else if(terrain[i] = NO_FLOOR)
        {
            top = "";
            bot = "";
        }
        for(int j = 0; j < config::screen_h_chars; j++)
        {
            if(j < 10)
                engine->terminal->output(x, config::screen_h_chars - j - 1, bot);
            else if(j > 12)
                engine->terminal->output(x, config::screen_h_chars - j - 1, top);
        }
    }

    // Draw player
    engine->terminal->output(20, (int)player.y - 3, (int(player.n_draws++ / 5) % 2 == 0 ? player.sprite : player.sprite_1));
}

void GameStateSubway::jump()
{
    if((terrain[(int)cur_beat] == NORMAL || terrain[(int)cur_beat] == LOW_CEILING) && player.state == Player::NORMAL)
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

