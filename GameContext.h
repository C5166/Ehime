#pragma once
#include "DxPlus/DxPlus.h"
#include <memory>
#include <vector>
#include "Game_03.h"
#include "Game_02.h"
#include "Game_00.h"

enum GameNamber
{
    Game_3,
    Game_2,
    Game_0
};

class GameContext
{
public:
    // 演出フェーズの定義
    enum class SequenceState
    {
        Explanation, // 説明画像表示
        Countdown,   // 3, 2, 1 表示
        StartMsg,    // スタート！表示
        Playing      // メインゲームプレイ中
    };

	GameContext() = default;
    ~GameContext() = default;

    void Init();
    void Reset();
    void Update(bool & input);
    void Draw() const;

    int GetHP() const { return playerHP; }
    int GetScore() const { return totalScore; }
    int GetRemainingTime() const { return static_cast<int>(timer); }

    int GetCurrentMiniGame() const { return Isinit; }
    void SetCurrentMiniGame(int gameNum) { Isinit = gameNum; }

    // ゲッターを追加
    Game_00& GetGame00() { return game_00; }


private:
    // --- 【設定】演出位置・表示時間 ---
    const float EXPLANATION_DURATION = 2.0f; // 説明表示時間(秒)
    const float COUNTDOWN_STEP_TIME = 1.0f; // カウントダウンの1コマ(3, 2, 1)あたりの秒数
    const float START_MSG_DURATION = 0.8f; // 「スタート！」表示時間(秒)

    const DxPlus::Vec2 EXPLANATION_POS = { 0.0f, 0.0f }; // 説明画像位置
    const DxPlus::Vec2 COUNTDOWN_POS = { 960.0f, 540.0f }; // 321位置
    const DxPlus::Vec2 START_MSG_POS = { 960.0f, 540.0f }; // スタート！位置

    // --- タイマー描画位置の設定 ---
    const DxPlus::Vec2 TIMER_POS = { 960.0f, 50.0f };
    const float DIGIT_OFFSET_X = 85.0f;

    // --- HP表示設定 ---
    const int MAX_PLAYER_HP = 3;
    const DxPlus::Vec2 HP_POS = { 50.0f, 50.0f };
    const float HP_ICON_OFFSET_X = 80.0f;

    void DrawTimer() const;
    void DrawHP() const;
	
    void DrawSequenceUI() const; // 演出描画関数を追加

	void DrawGameOverUI() const; // ゲームオーバー描画関数を追加

    const DxPlus::Sprite::SpriteBase* backgroundSpr{ nullptr };
    const DxPlus::Sprite::SpriteBase* backgroundSpr2{ nullptr };
    const DxPlus::Sprite::SpriteBase* Game_start123{ nullptr };
    const DxPlus::Sprite::SpriteBase* Game_start{ nullptr };
    const DxPlus::Sprite::SpriteBase* Game_setumei_2{ nullptr };

    Game_03 game_03;
    Game_02 game_02;

    int Isinit{ 0 };
    Game_00 game_00;

    int playerHP{ 3 };
    int totalScore{ 0 };

    const float GAME_TIME_LIMIT{ 10000000.0f };
    const float TIME_SPEED_RATE{ 1.0f };
    float timer{ 10.0f };

    // --- 演出管理用変数 ---
    SequenceState sequenceState{ SequenceState::Explanation };
    float sequenceTimer{ 0.0f };

};