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
    // Update と Draw で引数として HP と Score (number) を受け取る
    void Update(int& hp, int& score);
    void Draw(int hp, int score) const;

private:
    void SpawnBalls(); // 的をまとめて生成するヘルパー関数

    const DxPlus::Sprite::SpriteBase* Herat1ID{ nullptr };
    const DxPlus::Sprite::SpriteBase* Herat2ID{ nullptr };
    const DxPlus::Sprite::SpriteBase* Herat3ID{ nullptr };

    std::vector<Object> balls;

    // 画面の幅・高さ（必要に応じてゲームの画面サイズに合わせて変更してください）
    
    const float screenX = 50.0f; // 範囲の左端 X 座標
    const float screenY = 100.0f; // 範囲の上端 Y 座標

    const float screenWidth = 1852 ;
    const float screenHeight = 860;

    int poti;

    int bubu;

    int good[3];

    int perfect;

};