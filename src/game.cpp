#include "game.h"

GameState StartScreenGameState::Process(std::set<int>& active_keys, GraphicEngine& graphic_engine)
{
    graphic_engine.Draw({&background_, &text_render_});
    title_text_ << "SPACE WARS";
    help_text_ << "Control: Player 1 [W, A, S, D, E], Player 2 [I, K, J, L, P], ESC(for exit)";
    start_text_ << "Press ENTER to start multiplayer or BACKSPASE to start singleplayer";
    if(active_keys.size() > 0)
    {
        if(active_keys.find(GLFW_KEY_ESCAPE) != active_keys.end())
        {
            return GameState::exit;
        }
        else if(active_keys.find(GLFW_KEY_ENTER) != active_keys.end())
        {
            active_keys.clear();
            return GameState::multi_play_level;
        }
        else if(active_keys.find(GLFW_KEY_BACKSPACE) != active_keys.end())
        {
            active_keys.clear();
            return GameState::single_play_level;
        }
    }
    return GameState::start_screen;
}

SinglePlayLevelGameState::SinglePlayLevelGameState(ObjectsDrawer& objects_drawer) : objects_drawer_{objects_drawer}
{
    game_levels_.emplace_back(new GameLevel1{});
    game_levels_.emplace_back(new GameLevel2{});
    game_levels_.emplace_back(new GameLevel3{});
    current_game_level_ = game_levels_.begin();
 }

GameState SinglePlayLevelGameState::Process(std::set<int>& active_keys, GraphicEngine& graphic_engine)
{
    const auto currentFrame = glfwGetTime();
    const auto deltaTime = currentFrame - lastFrame_;
    lastFrame_ = currentFrame;

    auto* game_level = current_game_level_->get();

    auto rocket_pusher = [game_level](const Rocket& rocket){game_level->PushRocket(rocket);};

    for(auto& bot : bots_) bot.Process(deltaTime, rocket_pusher);
    player_->Process(deltaTime, rocket_pusher);

    auto& game_objects = game_level->GetObjectsList();

    physic_emulator_.Emulate(game_objects, deltaTime);

    for(auto obj_it = game_objects.begin(); obj_it != game_objects.end();)
    {
        if((*obj_it)->ShouldBeDestroyed()) obj_it = game_objects.erase(obj_it);
        else ++obj_it;
    }

    if(!game_level->GetPlayerSpaceShip().expired())
    {
        auto space_ship = game_level->GetPlayerSpaceShip().lock();
        Point position = space_ship->GetPosition();
        player_space_ship_state_text_.UpdateCoordinates(position.x - 20.0, position.y + 30.0);
        player_space_ship_state_text_ << "H:" << space_ship->GetHealth() << "| R:" << space_ship->GetRockets() << "| F:" << space_ship->GetFuel();
    }

    graphic_engine.Draw({&game_level->GetBackGround(), &objects_drawer_, &text_render_});

    if(game_level->GetPlayerSpaceShip().expired())
    {
        battle_result_test_ << "YOU LOOSE!";
        next_text_ << "Press ENTER to restart";

        if(active_keys.find(GLFW_KEY_ENTER) != active_keys.end())
        {
            ResetLevel(current_game_level_->get());
            return GameState::single_play_level;
        }
    }
    else if(game_level->IsAllEnemiesDestroyed())
    {
        battle_result_test_ << "YOU WIN!";
        next_text_ << "Press ENTER for next level";

        if(active_keys.find(GLFW_KEY_ENTER) != active_keys.end())
        {
            active_keys.clear();
            ++current_game_level_;
            if(current_game_level_ != game_levels_.end())
            {
                ResetLevel(current_game_level_->get());
                return GameState::single_play_level;
            }
            else
            {
                active_keys.emplace(GLFW_KEY_ENTER);
                current_game_level_ = game_levels_.begin();
                return GameState::start_screen;
            }
        }
    }

    if(active_keys.find(GLFW_KEY_ESCAPE) != active_keys.end())
    {
        active_keys.erase(GLFW_KEY_ESCAPE);
        return GameState::start_screen;
    }

    return GameState::single_play_level;
}

void SinglePlayLevelGameState::OnStateEnter()
{
    lastFrame_ = glfwGetTime();
    current_game_level_ = game_levels_.begin();
    ResetLevel(current_game_level_->get());
}

void SinglePlayLevelGameState::OnStateExit()
{
    InputHandler::RemoveListener(*player_);
}

void SinglePlayLevelGameState::ResetLevel(GameLevelBase* game_level)
{
    bots_.clear();
    InputHandler::RemoveListener(*player_);
    game_level->Reset();

    objects_drawer_.UpdateObjectListReference(game_level->GetObjectsList());

    player_.reset(new Player{});
    player_->AssignSpaceShip(game_level->GetPlayerSpaceShip());

    auto player_space_shipt = game_level->GetPlayerSpaceShip();
    auto enemy_space_ships = game_level->GetEnemySpaceShips();
    for(auto& object : enemy_space_ships)
    {
        auto [object_type, specific_type] = object.lock()->GetType().Get();
        if( (object_type == GeneralType::space_ship) && (player_space_shipt.lock() != object.lock()))
        {
            bots_.emplace_back(object, player_space_shipt);
        }
    }

    InputHandler::RegisterListener(*player_);
}

MultiPlayLevelGameState::MultiPlayLevelGameState(ObjectsDrawer& objects_drawer) : objects_drawer_{objects_drawer}
{
     game_level_.reset(new GameLevelFor2{});
}

GameState MultiPlayLevelGameState::Process(std::set<int>& active_keys, GraphicEngine& graphic_engine)
{
    const auto currentFrame = glfwGetTime();
    const auto deltaTime = currentFrame - lastFrame_;
    lastFrame_ = currentFrame;

    auto* game_level = game_level_.get();

    auto rocket_pusher = [game_level](const Rocket& rocket){game_level->PushRocket(rocket);};
    player_1->Process(deltaTime, rocket_pusher);
    player_2->Process(deltaTime, rocket_pusher);

    auto& game_objects = game_level->GetObjectsList();

    physic_emulator_.Emulate(game_objects, deltaTime);

    for(auto obj_it = game_objects.begin(); obj_it != game_objects.end();)
    {
        if((*obj_it)->ShouldBeDestroyed()) obj_it = game_objects.erase(obj_it);
        else ++obj_it;
    }

    if(!game_level->GetPlayerSpaceShip().expired())
    {
        auto space_ship = game_level->GetPlayerSpaceShip().lock();
        Point position = space_ship->GetPosition();
        player_1_space_ship_state_text_.UpdateCoordinates(position.x - 20.0, position.y + 30.0);
        player_1_space_ship_state_text_ << "H:" << space_ship->GetHealth() << "| R:" << space_ship->GetRockets() << "| F:" << space_ship->GetFuel();
    }

    if(!game_level->GetSecondPlayerSpaceShip().expired())
    {
        auto space_ship = game_level->GetSecondPlayerSpaceShip().lock();
        Point position = space_ship->GetPosition();
        player_2_space_ship_state_text_.UpdateCoordinates(position.x - 20.0, position.y + 30.0);
        player_2_space_ship_state_text_ << "H:" << space_ship->GetHealth() << "| R:" << space_ship->GetRockets() << "| F:" << space_ship->GetFuel();
    }

    graphic_engine.Draw({&game_level->GetBackGround(), &objects_drawer_, &text_render_});

    if(game_level->GetPlayerSpaceShip().expired() || game_level->GetSecondPlayerSpaceShip().expired())
    {
        if(game_level->GetPlayerSpaceShip().expired() && game_level->GetSecondPlayerSpaceShip().expired())
        {
            battle_result_test_ << "BOTH PLAYER LOOSE!";
        }
        else if(game_level->GetSecondPlayerSpaceShip().expired())
        {
            battle_result_test_ << "FIRST PLAYER WIN!";
        }
        else
        {
            battle_result_test_ << "SECOND PLAYER WIN!";
        }

        if(active_keys.find(GLFW_KEY_ENTER) != active_keys.end())
        {
            ResetLevel();
            return GameState::multi_play_level;
        }
    }

    if(active_keys.find(GLFW_KEY_ESCAPE) != active_keys.end())
    {
        active_keys.erase(GLFW_KEY_ESCAPE);
        return GameState::start_screen;
    }

    return GameState::multi_play_level;
}

void MultiPlayLevelGameState::OnStateEnter()
{
    lastFrame_ = glfwGetTime();
    ResetLevel();
}

void MultiPlayLevelGameState::OnStateExit()
{
    InputHandler::RemoveListener(*player_1);
    InputHandler::RemoveListener(*player_2);
}


void MultiPlayLevelGameState::ResetLevel()
{
    InputHandler::RemoveListener(*player_1);
    InputHandler::RemoveListener(*player_2);
    game_level_->Reset();

    objects_drawer_.UpdateObjectListReference(game_level_->GetObjectsList());

    player_1.reset(new Player{});
    player_1->AssignSpaceShip(game_level_->GetPlayerSpaceShip());
    player_1->SetControlConfiguration(player_1_control_configuration);

    player_2.reset(new Player{});
    player_2->AssignSpaceShip(game_level_->GetSecondPlayerSpaceShip());
    player_2->SetControlConfiguration(player_2_control_configuration);


    InputHandler::RegisterListener(*player_1);
    InputHandler::RegisterListener(*player_2);

}

void Game::Notify(int key, int action)
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

Game& Game::GetInstance()
{
    static Game instance;
    return instance;
}

void Game::Run(GraphicEngine& graphic_engine)
{

    while(game_state_!= GameState::exit)
    {
        auto previous_state = game_state_;
        game_state_ = game_states_[game_state_]->Process(active_keys_, graphic_engine);
        if(game_state_ != previous_state)
        {
            game_states_[previous_state]->OnStateExit();
            game_states_[game_state_]->OnStateEnter();
        }
    }
}

Game::Game()
{
    InputHandler::RegisterListener(*this);
    game_states_[GameState::start_screen].reset(new StartScreenGameState{});
    game_states_[GameState::single_play_level].reset(new SinglePlayLevelGameState{objects_drawer_});
    game_states_[GameState::multi_play_level].reset(new MultiPlayLevelGameState{objects_drawer_});
    game_states_[GameState::exit].reset(new NullGameState{});

    objects_drawer_.AddModel({GeneralType::space_ship, SpaceShipType::small_battle_ship}, "resources/models/InterstellarRunner.vox");
    objects_drawer_.AddModel({GeneralType::space_ship, SpaceShipType::enemy_space_ship}, "resources/models/UltravioletIntruder.vox");
    objects_drawer_.AddModel({GeneralType::rocket, RocketType::base_rocket}, "resources/models/rocket.vox");
    objects_drawer_.AddModel({GeneralType::star, StarType::simple_star}, "resources/models/star.vox");
    objects_drawer_.AddModel({GeneralType::meteor, MeteorType::regular_1}, "resources/models/meteor_1.vox");
    objects_drawer_.AddModel({GeneralType::meteor, MeteorType::regular_2}, "resources/models/meteor_2.vox");

}

