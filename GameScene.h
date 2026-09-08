// =============================
// Scenes/Game/GameScene.h
// =============================
#pragma once
#include "Scene.h"
#include "GameContext.h"
#include <algorithm>

class GameScene final : public Scene
{
public:
    explicit GameScene(GameContext* context) : Scene(context) {}
    void Init() override;
    void Update() override;
    void Render() const override;
    void End() override;

    void SetGameOverInput(bool input) { isGameOverInput = input; }
    bool GetGameOverInput() const { return isGameOverInput; }

private:
    bool isGameOverInput{ false };

    // --- マウス設定（サイズ＆判定） ---
    float mouseScale = 1.0f;            // マウスカーソルの表示スケール
    float mouseCollisionRadius = 60.0f; // マウスの判定半径
};