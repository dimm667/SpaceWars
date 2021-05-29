#include "game_level.h"

void GameLevelBase::Reset()
{
    objects_.clear();
    enemy_space_ships_.clear();
    ConfigureScene();
}

void GameLevelBase::PushRocket(const Rocket& rocket)
{
    objects_.emplace_back(new Rocket(rocket));
}


GameLevelBase::GameLevelBase()
{
}

GameLevelBase::~GameLevelBase()
{
}

std::weak_ptr<SpaceShip> GameLevelBase::GetPlayerSpaceShip()
{
    return player_1_space_ship_;
}

std::weak_ptr<SpaceShip> GameLevelBase::GetSecondPlayerSpaceShip()
{
    return player_2_space_ship_;
}

std::list<std::weak_ptr<SpaceShip>>& GameLevelBase::GetEnemySpaceShips()
{
    return enemy_space_ships_;
}

Background& GameLevelBase::GetBackGround()
{
    return *background_;
}

std::list<std::shared_ptr<PhysicObjectWithType>>& GameLevelBase::GetObjectsList()
{
    return objects_;
}

bool GameLevelBase::IsAllEnemiesDestroyed() const
{
    return std::all_of(enemy_space_ships_.begin(),
                enemy_space_ships_.end(),
                [](const auto ptr){return ptr.expired();});
}

GameLevelFor2::GameLevelFor2()
{
    background_.reset(new Background{"resources/textures/space/bottom.png"});
}

void GameLevelFor2::ConfigureScene()
{
    PlaceFirstPlayerSpaceShip(SpaceShipType::small_battle_ship, Point{100.0, 100.0}, 0.0, 30.0, 30.0);
    PlaceSecondPlayerSpaceShip(SpaceShipType::enemy_space_ship, Point{1800.0, 900.0}, -PI, 30.0, 30.0);
    PlaceStar(Point{1920.0/2.0, 1080.0/2.0}, 10000000.0, 30.0);
    for (int n = 0; n < 40; ++n) { PlaceRandomMeteor(); }

}
GameLevel1::GameLevel1()
{
    background_.reset(new Background{"resources/textures/space/bottom.png"});
}

void GameLevel1::ConfigureScene()
{
    PlaceFirstPlayerSpaceShip(SpaceShipType::small_battle_ship, Point{100.0, 100.0}, 0.0, 30.0, 30.0);
    PlaceEnemySpaceShip(SpaceShipType::enemy_space_ship, Point{1800.0, 900.0}, -PI, 30.0, 30.0);

    for (int n = 0; n < 5; ++n) { PlaceRandomMeteor(); }
}

GameLevel2::GameLevel2()
{
    background_.reset(new Background{"resources/textures/space/right.png"});
}

void GameLevel2::ConfigureScene()
{
    PlaceFirstPlayerSpaceShip(SpaceShipType::small_battle_ship, Point{100.0, 100.0}, 0.0, 30.0, 30.0);
    PlaceEnemySpaceShip(SpaceShipType::enemy_space_ship, Point{1800.0, 900.0}, -PI, 30.0, 30.0);
    PlaceEnemySpaceShip(SpaceShipType::enemy_space_ship, Point{1800.0, 100.0}, -PI, 30.0, 30.0);

    for (int n = 0; n < 15; ++n) { PlaceRandomMeteor(); }
}

GameLevel3::GameLevel3()
{
    background_.reset(new Background{"resources/textures/space/left.png"});
}

void GameLevel3::ConfigureScene()
{
    PlaceFirstPlayerSpaceShip(SpaceShipType::small_battle_ship, Point{100.0, 100.0}, 0.0, 30.0, 30.0);
    PlaceEnemySpaceShip(SpaceShipType::enemy_space_ship, Point{1800.0, 900.0}, -PI, 30.0, 30.0);
    PlaceEnemySpaceShip(SpaceShipType::enemy_space_ship, Point{100.0, 900.0}, -PI_HALF, 30.0, 30.0);
    PlaceEnemySpaceShip(SpaceShipType::enemy_space_ship, Point{1800.0, 500.0}, -PI, 30.0, 30.0);
    for (int n = 0; n < 20; ++n) { PlaceRandomMeteor(); }
}

