#pragma once
#include "DxPlus/DxPlus.h"
#include "GameObject.h"
#include <vector>

enum class Game02InstructionType
{
    More, // game_setumei_7 : 合計が多いほうを選択
    Less  // game_setumei_8 : 合計が少ないほうを選択
};

// 各問題の「左の合計値」と「右の合計値」を保持する構造体
struct ProblemValue {
    double leftValue;  // 左側の合計値
    double rightValue; // 右側の合計値
};

class Game_02
{
public:
    Game_02() = default;
    void Init();
    void Reset();
    void Update(int& hp, int& score);
    void Draw(int hp, int score) const;
    void DrawPerfectAnimation(int x, int y);

    // 全問正解したか
    bool AllCorrect() const { return completed; }

    // 指示画像（説明スプライト）を取得
    const DxPlus::Sprite::SpriteBase* GetExplanationSprite() const { return currentExplanationSpr; }

private:

    int background;

    int a[6];

    // 各問題 (game2_1 〜 game2_6) の合計値を直接指定
    // 例: { 左の合計値, 右の合計値 }
    ProblemValue problems[6] = {
        { 3.0 * 4.0,  5.0 * 3.0 },      // game2_1: 左(12), 右(15)
        { 100.0,      48.0 },           // game2_2: 直接数値指定も可能
        { 0.8 * 0.5,  1.0 },            // game2_3: 小数点や計算式もそのまま書けます
        { 2026.0 + 100.0, 20.0 + 10000.0 }, // game2_4
        { -100.0,      -90.0 },        // game2_5
        { 17.0 / 42.0-11/56, 1 }      // game2_6
    };

    // 画面の幅・高さ
    const float screenWidth = 1920.0f;
    const float screenHeight = 1080.0f;

    const DxPlus::Sprite::SpriteBase* mihon_3{ nullptr };

    int change = 0;

    // 指示（ルール）管理用
    Game02InstructionType instructionType{ Game02InstructionType::More };
    const DxPlus::Sprite::SpriteBase* currentExplanationSpr{ nullptr };

    //枠・エフェクト
    int waku;
    int perfectSheetID;
    int notsheetID;

    int poti;

    bool Animating = false;

    struct notFrameRect {
        int x, y, w, h;
    };

    int frame = 0;

    int bubu;

    int good[3];

    int perfect;

    int setumeivoice[2];

    // 正解カウントと完了フラグ
    int correctCount{ 0 };
    bool completed{ false };

    // 計算後の値を保持する配列 (小数対応のため double に変更)
    double leftValue[6] = { -1.0, -1.0, -1.0, -1.0, -1.0, -1.0 };
    double rightValue[6] = { -1.0, -1.0, -1.0, -1.0, -1.0, -1.0 };
};