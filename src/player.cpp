#include "player.h"
#include <iostream>

void Player::Notify(int key, int action)
{
    if(action == GLFW_PRESS)
    {
        active_keys_.insert(key);
    }
    else if(action == GLFW_RELEASE)
    {
        active_keys_.erase(key);
    }
}

void Player::SetControlConfiguration(const PlayerControlConfiguration& configuration)
{
    control_configuration_ = configuration;
}

void Player::AssignSpaceShip(std::weak_ptr<SpaceShip> space_ship_ptr)
{
    space_ship_ptr_ = space_ship_ptr;
}

void Player::Process(const double time_delta, std::function<void(Rocket)> rocket_pusher)
{
    if(space_ship_ptr_.expired())
        return;

    auto space_ship = space_ship_ptr_.lock();

    const double acceleration{200.0};

    const Vector orientation{space_ship->GetOrientation()};
    const Vector velocity{space_ship->GetVelocity()};

    double rotation_rate_change{};
    double acc{};

    // for(const auto& key : active_keys_)
    // {
    if(active_keys_.find(control_configuration_.key_accelerate) != active_keys_.end()){
        acc = acceleration;
    }

    if(active_keys_.find(control_configuration_.key_decelerate) != active_keys_.end()){
        acc = -acceleration;
    }

    if(active_keys_.find(control_configuration_.key_turn_cw) != active_keys_.end()){
        rotation_rate_change += 3.0;
    }

    if(active_keys_.find(control_configuration_.key_turn_ccw) != active_keys_.end()){
        rotation_rate_change -= 3.0;
    }
        // switch (key)
        // {
        //     case GLFW_KEY_W: acc = acceleration; break;
        //     case GLFW_KEY_S: acc = -acceleration; break;
        //     case GLFW_KEY_A: rotation_rate_change += 3.0; break;
        //     case GLFW_KEY_D: rotation_rate_change -= 3.0; break;
        // }
    // }

    if(active_keys_.find(control_configuration_.key_shoot) != active_keys_.end())
    {
        active_keys_.erase(control_configuration_.key_shoot);
        auto rocket{space_ship->Shoot()};
        if(rocket.has_value())
            rocket_pusher(rocket.value());
    }

    // Velocity limit
    const auto angle_to_movement = velocity.GetAngleTo(std::copysign(1.0, acc) * orientation);
    if((std::abs(velocity.GetLength()) < 1.0) || ((velocity.GetLength() * std::cos(angle_to_movement)) < 200.0))
        space_ship->Accelerate(acc, time_delta);
    else
        space_ship->Accelerate(0.0, time_delta);

    space_ship->ApplyRotation(rotation_rate_change);

}

