// =============================
// Core/GameContext.h
// =============================
#pragma once
#include "DxPlus/DxPlus.h"
#include <memory>
#include <vector>
#include "Game_03.h"
#include "Game_02.h"
#include "Game_00.h"

// ゲーム全体で共有する"文脈（コンテキスト）"。
// プレイヤー・敵・マップ・アイテムなどを束ね、
// シーン（Title / Game / Result）から参照できるようにする。

enum GameNamber
{
    Game_3,
    Game_2,
    Game_0
};;

class GameContext
{
public:
    GameContext() = default;
    ~GameContext() = default;

    // 一度だけ呼ぶ初期化（リソースロードは ResourceManager 側で行う）
    void Init();

    // ステージ再開・リトライ用の軽量リセット
    void Reset();

    // プレイの更新と描画（必要なシーンで呼ぶ）
    void Update();
    void Draw() const;

    // HPやScoreを参照・取得したい場合に利用できるゲッター
    int GetHP() const { return playerHP; }
    int GetScore() const { return totalScore; }
    int GetRemainingTime() const { return static_cast<int>(timer); }

private:
    // --- タイマー描画位置の設定 ---
    const DxPlus::Vec2 TIMER_POS = { 960.0f, 50.0f }; // タイマーの基準描画位置（X, Y）
    const float DIGIT_OFFSET_X = 85.0f;               // 2桁表示時の数字同士の間隔(px)

    void DrawTimer() const;

    const DxPlus::Sprite::SpriteBase* backgroundSpr{ nullptr };
    const DxPlus::Sprite::SpriteBase* backgroundSpr2{ nullptr };

	Game_03 game_03;
	Game_02 game_02;
    

    int Isinit;
    Game_00 game_00;

    // ゲーム全体で引き継ぐステータス
    int playerHP{ 3 };
    int totalScore{ 0 };

    // --- タイマー設定 ---
    const float GAME_TIME_LIMIT{ 10.0f }; // 制限時間（秒）
    const float TIME_SPEED_RATE{ 1.0f };  // 1秒経過する感覚の倍率 (1.0 = 通常速度)
    float timer{ 10.0f };                  // カウントダウン用タイマー
};
