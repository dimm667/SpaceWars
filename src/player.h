#ifndef PLAYER_H
#define PLAYER_H

#include "game_object.h"
#include "graphic_engine/user_input_handler.h"
#include <memory>
#include <set>
#include <array>
#include <algorithm>

struct PlayerControlConfiguration
{
    int key_accelerate{};
    int key_decelerate{};
    int key_turn_cw{};
    int key_turn_ccw{};
    int key_shoot{};
};

constexpr PlayerControlConfiguration player_1_control_configuration{GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_E};
constexpr PlayerControlConfiguration player_2_control_configuration{GLFW_KEY_I, GLFW_KEY_K, GLFW_KEY_J, GLFW_KEY_L, GLFW_KEY_O};


class Player : public IUserInputListener
{
public:
    Player() = default;

    void SetControlConfiguration(const PlayerControlConfiguration& configuration);

    void Notify(int key, int action) override;

    void AssignSpaceShip(std::weak_ptr<SpaceShip> space_ship_ptr);

    void Process(const double time_delta, std::function<void(Rocket)> rocket_pusher);

private:
    PlayerControlConfiguration control_configuration_{player_1_control_configuration};
    std::set<int> active_keys_{};
    std::weak_ptr<SpaceShip> space_ship_ptr_;
};

#endif // PLAYER_H
