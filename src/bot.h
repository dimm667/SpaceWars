#ifndef BOT_H
#define BOT_H

#include "game_object.h"
#include <cmath>
#include <memory>

class Bot
{
public:
    Bot(std::weak_ptr<SpaceShip> own_space_ship, const std::weak_ptr<SpaceShip> player_space_ship);
    void Process(float delta_time, std::function<void(Rocket)> rocket_pusher);

private:
    std::weak_ptr<SpaceShip> own_space_ship_ptr_;
    const std::weak_ptr<SpaceShip> player_space_ship_ptr_;
    float time_accumulator_{};

};

#endif // BOT_H
