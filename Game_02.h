#pragma once
#include "DxPlus/DxPlus.h"
#include "GameObject.h"
#include <vector>

enum class Game02InstructionType
{
    More, // game_setumei_7 : 合計が多いほうを選択
    Less  // game_setumei_8 : 合計が少ないほうを選択
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

    // 各問題(a[0]〜a[5])において「合計が多いほう」が左か右かを示すテーブル
    // true : 左(Left)が多い / false : 右(Right)が多い
    // NOTE: game2_2.png 表示時に左が正解になるよう 2 番目の値を true に設定
    bool leftIsMoreTable[6] = { true, true, true, true, false, true };

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

    // 各問題の左側/右側の正解判定に使う数値（左と右の合計や積など、問題に応じた値を設定してください）
    // デフォルトは -1（未設定）で、その場合は従来のテーブルを参照します。
    int leftValue[6]  = { -1, -1, -1, -1, -1, -1 };
    int rightValue[6] = { -1, -1, -1, -1, -1, -1 };
};