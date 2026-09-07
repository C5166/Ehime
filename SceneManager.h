#pragma once
#include "Scene.h"
#include "GameContext.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameOverScene.h"

enum class SceneID { Title, Game, GameOver};

class SceneManager
{
public:
    static SceneManager& GetInstance()
    {
        static SceneManager instance;
        return instance;
    }

    void Init();
    void Shutdown();
    void Run();
    void SetScene(Scene* newScene);
    Scene* GetScene(SceneID id);

    Scene* GetCurrentScene() const { return scene; }

    GameContext& GetGameState() { return gameContext; }

    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

private:
    SceneManager() = default;
    ~SceneManager() = default;

    GameContext gameContext;
    TitleScene  titleScene{ &gameContext };
    GameScene   gameScene{ &gameContext };
    GameOverScene gameOverScene{ &gameContext };

    Scene* scene = nullptr;
};
inline SceneManager& SM() { return SceneManager::GetInstance(); }