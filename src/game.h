#ifndef GAME_H
#define GAME_H

#include <tuple>
#include <map>
#include <algorithm>

#include "graphic_engine/shader_program.h"
#include "graphic_engine/background.h"
#include "graphic_engine/object_model.h"
#include "graphic_engine/user_input_handler.h"
#include "graphic_engine/graphic_engine.h"
#include "graphic_engine/text_render.h"
#include "objects_graphic.h"
#include "game_physics.h"
#include "player.h"
#include "game_object.h"
#include "bot.h"
#include "game_level.h"

enum class GameState
{
    start_screen,
    single_play_level,
    multi_play_level,
    exit,
};

class IGameState
{
public:
    virtual GameState Process(std::set<int>& active_keys, GraphicEngine& graphic_engine) = 0;
    virtual void OnStateEnter() {}
    virtual void OnStateExit() {}
};

class NullGameState : public IGameState
{
public:
    GameState Process(std::set<int>& active_keys, GraphicEngine& graphic_engine) override
    {
        return GameState::exit;
    }
};

class StartScreenGameState : public IGameState
{
public:

    GameState Process(std::set<int>& active_keys, GraphicEngine& graphic_engine) override;

private:
    Background background_{"resources/textures/start_screen.jpg"};
    TextRender text_render_{};
    TextStream& title_text_{text_render_.GetRenderTextStream(620.0f, 800.0f, 2.0f, glm::vec3(0.0, 0.8f, 0.0f))};
    TextStream& help_text_{text_render_.GetRenderTextStream(600.0f, 100.0f, 0.5f, glm::vec3(0.0, 0.8f, 0.0f))};
    TextStream& start_text_{text_render_.GetRenderTextStream(590.0f, 50.0f, 0.5f, glm::vec3(0.0, 0.8f, 0.0f))};
};

class SinglePlayLevelGameState : public IGameState
{
public:
    SinglePlayLevelGameState(ObjectsDrawer& objects_drawer);

    GameState Process(std::set<int>& active_keys, GraphicEngine& graphic_engine) override;

    void OnStateEnter() override;

    void OnStateExit() override;

private:
    void ResetLevel(GameLevelBase* game_level);

    GamePhysicEmulator physic_emulator_{};
    TextRender text_render_{};

    TextStream& player_space_ship_state_text_{text_render_.GetRenderTextStream(25.0f, 25.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f))};
    TextStream& battle_result_test_{text_render_.GetRenderTextStream(800.0f, 700.0f, 1.5f, glm::vec3(0.5, 0.8f, 0.2f))};
    TextStream& next_text_{text_render_.GetRenderTextStream(840.0f, 50.0f, 0.5f, glm::vec3(0.0, 0.8f, 0.0f))};

    float lastFrame_{};
    std::unique_ptr<Player> player_{};
    std::list<std::unique_ptr<GameLevelBase>> game_levels_{};
    std::list<std::unique_ptr<GameLevelBase>>::iterator current_game_level_{};
    ObjectsDrawer& objects_drawer_;
    std::list<Bot> bots_{};
};

class MultiPlayLevelGameState : public IGameState
{
public:
    MultiPlayLevelGameState(ObjectsDrawer& objects_drawer);

    GameState Process(std::set<int>& active_keys, GraphicEngine& graphic_engine) override;

    void OnStateEnter() override;

    void OnStateExit() override;

private:
    void ResetLevel();

    GamePhysicEmulator physic_emulator_{};
    TextRender text_render_{};

    TextStream& player_1_space_ship_state_text_{text_render_.GetRenderTextStream(25.0f, 25.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f))};
    TextStream& player_2_space_ship_state_text_{text_render_.GetRenderTextStream(25.0f, 25.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f))};
    TextStream& battle_result_test_{text_render_.GetRenderTextStream(600.0f, 700.0f, 1.5f, glm::vec3(0.5, 0.8f, 0.2f))};
    TextStream& next_text_{text_render_.GetRenderTextStream(840.0f, 50.0f, 0.5f, glm::vec3(0.0, 0.8f, 0.0f))};

    float lastFrame_{};
    std::unique_ptr<Player> player_1{};
    std::unique_ptr<Player> player_2{};
    std::unique_ptr<GameLevelBase> game_level_{};
    ObjectsDrawer& objects_drawer_;

};

class Game : public IUserInputListener
{
public:
    void Notify(int key, int action) override;

    static Game& GetInstance();

    void Run(GraphicEngine& graphic_engine);

private:
    Game();

    std::map<GameState, std::unique_ptr<IGameState>> game_states_{};
    std::set<int> active_keys_{};
    GameState game_state_{GameState::start_screen};
    ObjectsDrawer objects_drawer_{};
};

#endif // GAME_H
