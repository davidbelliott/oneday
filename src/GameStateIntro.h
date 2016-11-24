#pragma once

#include "GameState.h"

struct LerpingChar
{
    int target_x;
    int target_y;
    float cur_x;
    float cur_y;
    std::string c;
    float lerp_coefficient;
    float snap_threshold;

    LerpingChar(char c_in, int target_x_in, int target_y_in);
    void update(sf::Time dt);
    void print(Terminal* t);
};

class GameStateIntro: public GameState
{
    private:
        //GameState* state_to_push;
        float x;
        float y;
        std::string title_string;
        std::vector<LerpingChar> chars;
    public:
        
        GameStateIntro(Engine* engine_in);
        virtual ~GameStateIntro();

        virtual void update(sf::Time dt);
        virtual void draw(sf::RenderTarget* target);
};
