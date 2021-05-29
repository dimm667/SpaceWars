#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <list>
#include "game_object.h"
#include "graphic_engine/background.h"
#include <memory>
#include <algorithm>
#include <random>

class GameLevelBase
{
public:
    GameLevelBase();
    ~GameLevelBase();

    std::weak_ptr<SpaceShip> GetPlayerSpaceShip();
    std::weak_ptr<SpaceShip> GetSecondPlayerSpaceShip();
    std::list<std::weak_ptr<SpaceShip>>& GetEnemySpaceShips();
    std::list<std::shared_ptr<PhysicObjectWithType>>& GetObjectsList();
    Background& GetBackGround();

    void Reset();
    void PushRocket(const Rocket& rocket);
    bool IsAllEnemiesDestroyed() const;

protected:

    virtual void ConfigureScene() = 0;

    template<typename... _Args>
    void PlaceFirstPlayerSpaceShip(_Args&&... __args)
    {
        std::shared_ptr<SpaceShip> space_ship{new SpaceShip{std::forward<_Args>(__args)...}};
        objects_.emplace_back(space_ship);
        player_1_space_ship_ = space_ship;
    }

    template<typename... _Args>
    void PlaceSecondPlayerSpaceShip(_Args&&... __args)
    {
        std::shared_ptr<SpaceShip> space_ship{new SpaceShip{std::forward<_Args>(__args)...}};
        objects_.emplace_back(space_ship);
        player_2_space_ship_ = space_ship;
    }

    template<typename... _Args>
    void PlaceEnemySpaceShip(_Args&&... __args)
    {
        std::shared_ptr<SpaceShip> space_ship{new SpaceShip{std::forward<_Args>(__args)...}};
        objects_.emplace_back(space_ship);
        enemy_space_ships_.push_back(space_ship);
    }

    template<typename... _Args>
    void PlaceMeteor(_Args&&... __args)
    {
        objects_.emplace_back(new Meteor{std::forward<_Args>(__args)...});
    }

    void PlaceRandomMeteor()
    {
        PlaceMeteor(static_cast<MeteorType>(meteor_type_(gen)), GetRandomPosition(), GetRandomVelocity(), GetRandomRotation());
    }

    template<typename... _Args>
    void PlaceStar(_Args&&... __args)
    {
        objects_.emplace_back(new Star{std::forward<_Args>(__args)...});
    }

    Point GetRandomPosition()
    {
        return Point{dis_width_(gen), dis_heigh_(gen)};
    }

    Vector GetRandomVelocity()
    {
        // return Vector{};
        return Vector{velocity_(gen), velocity_(gen)};
    }

    double GetRandomRotation()
    {
        return rotation_(gen);
    }

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<> dis_width_{0.0, 1920.0};
    std::uniform_real_distribution<> dis_heigh_{0.0, 1080.0};
    std::uniform_real_distribution<> velocity_{-300.0, 300.0};
    std::uniform_real_distribution<> rotation_{-20.0, 20.0};
    std::uniform_int_distribution<> meteor_type_{0, 1};

    std::unique_ptr<Background> background_;
    std::weak_ptr<SpaceShip>  player_1_space_ship_{};
    std::weak_ptr<SpaceShip>  player_2_space_ship_{};
    std::list<std::weak_ptr<SpaceShip>> enemy_space_ships_{};
    std::list<std::shared_ptr<PhysicObjectWithType>> objects_{};

};

class GameLevelFor2 : public GameLevelBase
{
public:
    GameLevelFor2();

protected:
    void ConfigureScene() override;
};

class GameLevel1 : public GameLevelBase
{
public:
    GameLevel1();

protected:
    void ConfigureScene() override;
};

class GameLevel2 : public GameLevelBase
{
public:
    GameLevel2();

protected:
    void ConfigureScene() override;
};

class GameLevel3 : public GameLevelBase
{
public:
    GameLevel3();

protected:
    void ConfigureScene() override;
};

#endif // GAME_LEVEL_H
