#include "bot.h"

Bot::Bot(std::weak_ptr<SpaceShip> own_space_ship, const std::weak_ptr<SpaceShip> player_space_ship) :
    own_space_ship_ptr_{own_space_ship}, player_space_ship_ptr_{player_space_ship}
{}

void Bot::Process(float delta_time, std::function<void(Rocket)> rocket_pusher)
{
    if(own_space_ship_ptr_.expired() || player_space_ship_ptr_.expired())
        return;

    auto own_space_ship = own_space_ship_ptr_.lock();
    auto player_space_ship = player_space_ship_ptr_.lock();

    // determine direction
    const auto& own_position{own_space_ship->GetPosition()};
    const auto& player_position{player_space_ship->GetPosition()};
    const auto dx{player_position.x - own_position.x};
    const auto dy{player_position.y - own_position.y};
    double heading_to_player{std::atan2(dy, dx)};
    const double heading_diff = wrap(own_space_ship->GetHeading() - heading_to_player, -PI, PI);

    // rotate_to_player
    if(std::abs(heading_diff) > 0.1 )
    {
        if(heading_diff < 0.0)
            own_space_ship->ApplyRotation(5.0);
        else
            own_space_ship->ApplyRotation(-5.0);
    }
    else
    {
        own_space_ship->ApplyRotation(0.0);
        time_accumulator_ += delta_time;
        if(time_accumulator_ > 1.0f)
        {
            auto rocket{own_space_ship->Shoot()};
            if(rocket.has_value())
                rocket_pusher(rocket.value());
            time_accumulator_ = 0.0f;
        }
    }

     if(std::abs(heading_diff) < 0.5 )
     {

        const Vector orientation{own_space_ship->GetOrientation()};
        const Vector velocity{own_space_ship->GetVelocity()};
        const double acc = std::abs(own_position.GetDistanceTo(player_position) > 100.0) ? 100.0 : -100.0;


        // Velocity limit
        const auto angle_to_movement = velocity.GetAngleTo(std::copysign(1.0, acc) * orientation);
        if((velocity.GetLength() < 1.0) || ((velocity.GetLength() * std::cos(angle_to_movement)) < 200.0))
            own_space_ship->Accelerate(acc, delta_time);
        else
            own_space_ship->Accelerate(0.0, delta_time);
    }
}
