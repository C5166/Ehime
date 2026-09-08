#pragma once
#include "DxPlus/DxPlus.h"
#include <memory>
#include <vector>
#include "Game_03.h"
#include "Game_02.h"
#include "Game_00.h"
#include "TitleScene.h"

enum GameNamber
{
    Game_0 = 0,
    Game_2 = 2,
    Game_3 = 3,
    Title = 4,
};

class GameContext
{
public:
    enum class SequenceState
    {
        Explanation,
        Countdown,
        StartMsg,
        Playing
    };

    GameContext() = default;
    ~GameContext() = default;

    void Init();
    void Reset();
    void Update(bool& input);
    void Draw() const;

    bool IsSequenceFinished() const { return sequenceFinished; }

    int GetHP() const { return playerHP; }
    int GetScore() const { return totalScore; }
    int GetRemainingTime() const { return static_cast<int>(timer); }

    int GetCurrentMiniGame() const { return Isinit; }
    void SetCurrentMiniGame(int gameNum) { Isinit = gameNum; }

    Game_00& GetGame00() { return game_00; }

private:
    const float EXPLANATION_DURATION = 2.0f;
    const float COUNTDOWN_STEP_TIME = 1.0f;
    const float START_MSG_DURATION = 0.8f;

    const DxPlus::Vec2 EXPLANATION_POS = { 0.0f, 0.0f };
    const DxPlus::Vec2 COUNTDOWN_POS = { 960.0f, 540.0f };
    const DxPlus::Vec2 START_MSG_POS = { 960.0f, 540.0f };

    const DxPlus::Vec2 TIMER_POS = { 960.0f, 50.0f };
    const float DIGIT_OFFSET_X = 65.0f;

    const int MAX_PLAYER_HP = 3;
    const DxPlus::Vec2 HP_POS = { 80.5f, 50.0f };
    const float HP_ICON_OFFSET_X = 80.0f;

    void DrawTimer() const;
    void DrawHP() const;
    void DrawSequenceUI() const;
    void DrawGameOverUI() const;

    void GenerateRandomGameQueue();
    void SetupCurrentGame();
    void AdvanceToNextGame(bool perfectAchieved);

    const DxPlus::Sprite::SpriteBase* backgroundSpr{ nullptr };
    const DxPlus::Sprite::SpriteBase* backgroundSpr2{ nullptr };
    const DxPlus::Sprite::SpriteBase* Game_start123{ nullptr };
    const DxPlus::Sprite::SpriteBase* Game_start{ nullptr };
    const DxPlus::Sprite::SpriteBase* Game_setumei_2{ nullptr };

    Game_03 game_03;
    Game_02 game_02;
    Game_00 game_00;

    int Isinit{ 0 };

    // 5ゲーム管理用
    std::vector<GameNamber> gameQueue;
    int currentGameIndex{ 0 };
    const int TOTAL_MINI_GAMES{ 5 };

    // シーケンス完了フラグ（5つのミニゲームを終えたら true）
    bool sequenceFinished{ false };

    // game_perfect アニメーション制御
    bool showPerfect{ false };
    int perfectSheetID{ -1 };
    int perfectFrame{ 0 };
    int perfectTimer{ 0 };
    int perfectAnimInterval{ 6 };
    const int perfectTotalFrames{ 17 };
    const int perfectColumns{ 10 };
    int perfectFrameW{ 0 };
    int perfectFrameH{ 0 };
    // 完了演出（game_perfect）の表示時間とフェード設定
    int perfectDisplayFrames{ 120 }; // 表示時間（フレーム、60fpsで2秒）
    int perfectFadeFrames{ 12 };     // フェードイン／アウト時間（フレーム）
    int perfectElapsedFrames{ 0 };
    int perfectAlpha{ 255 };

    int playerHP{ 3 };
    int totalScore{ 0 };

    const float GAME_TIME_LIMIT{ 10.0f };
    const float TIME_SPEED_RATE{ 1.0f };
    float timer{ 10.0f };

    int gamestart;

    int CountSE_1;
    int CountSE_2;

    int lastPlayTime{ -1 };

    SequenceState sequenceState{ SequenceState::Explanation };
    float sequenceTimer{ 0.0f };
};