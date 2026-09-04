#pragma once
#include "DxPlus/DxPlus.h"
#include "GameObject.h"
#include <vector>

class Game_03
{
public:
    Game_03() = default;
    void Init();
    void Reset();
    void Update();
    void Draw() const;

private:
    void SpawnBalls(); // 的をまとめて生成するヘルパー関数

    const DxPlus::Sprite::SpriteBase* explanationID{ nullptr };

    std::vector<Object> balls;

    // 画面の幅・高さ（必要に応じてゲームの画面サイズに合わせて変更してください）
    const float screenWidth = 1280.0f;
    const float screenHeight = 720.0f;

    int number{ 0 };  // スコア
    int HP{ 3 };      // 仮HP（ハート3個分）
};