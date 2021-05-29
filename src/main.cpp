#include <iostream>
#include "graphic_engine/graphic_engine.h"
#include "game.h"


int main()
{
    auto& graphic_engine = GraphicEngine::GetInstance();
    auto& game = Game::GetInstance();

    game.Run(graphic_engine);
}
