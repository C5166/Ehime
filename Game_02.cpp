#include "Game_02.h"
#include "ResourceKeys.h"
#include "ResourceManager.h"
#include <cstdlib>

namespace {
    struct perfectFrameRect {
        int x, y, w, h;
    };

    static const perfectFrameRect frames[33] = {
        { 0     , 0     , 1391, 604 },
        { 1391  , 0     , 1391, 604 },
        { 2782  , 0     , 1391, 604 },
        { 4173  , 0     , 1391, 604 },
        { 5564  , 0     , 1391, 604 },
        { 6955  , 0     , 1391, 604 },
        { 8346  , 0     , 1391, 604 },
        { 1391  , 0     , 1391, 604 },
        { 9737  , 0     , 1391, 604 },
        { 11128 , 0     , 1391, 604 },
        { 0     , 604   , 1391, 604 },
        { 1391  , 604   , 1391, 604 },
        { 2782  , 604   , 1391, 604 },
        { 4173  , 604   , 1391, 604 },
        { 5564  , 604   , 1391, 604 },
        { 6955  , 604   , 1391 ,604 },
        { 8346  , 604   , 1391 ,604 },
        { 1391  , 604   , 1391 ,604 },
        { 9737  , 604   , 1391 ,604 },
        { 11128 , 604   , 1391 ,604 },
        { 0     , 1208  , 1391 ,604 },
        { 1391  , 1208  , 1391 ,604 },
        { 2782  , 1208  , 1391 ,604 },
        { 4173  , 1208  , 1391 ,604 },
        { 5564  , 1208  , 1391 ,604 },
        { 6955  , 1208  , 1391 ,604 },
        { 8346  , 1208  , 1391 ,604 },
        { 1391  , 1208  , 1391 ,604 },
        { 9737  , 1208  , 1391 ,604 },
        { 11128 , 1208  , 1391 ,604 },
        { 0     , 1812  , 1391 ,604 },
        { 1391  , 1812  , 1391 ,604 },
        { 2782  , 1812  , 1391 ,604 },
    };
}

void Game_02::DrawPerfectAnimation(int x, int y)
{
    DrawRectGraph(
        x, y,
        frames[frame].x, frames[frame].y,
        frames[frame].w, frames[frame].h,
        perfectSheetID,
        TRUE
    );

    if (frame >= 33)
    {
        frame = 0;
        Animating = false;
    }
}

void Game_02::Init()
{
    a[0] = DxLib::LoadGraph(L"./Data/Images/game2_1.png");
    a[1] = DxLib::LoadGraph(L"./Data/Images/game2_2.png");
    a[2] = DxLib::LoadGraph(L"./Data/Images/game2_3.png");
    a[3] = DxLib::LoadGraph(L"./Data/Images/game2_4.png");
    a[4] = DxLib::LoadGraph(L"./Data/Images/game2_5.png");
    a[5] = DxLib::LoadGraph(L"./Data/Images/game2_6.png");

    good[0] = RM().GetSound(ResourceKeys::SE_GoodVoice1);
    good[1] = RM().GetSound(ResourceKeys::SE_GoodVoice2);
    good[2] = RM().GetSound(ResourceKeys::SE_GoodVoice3);

    waku = DxLib::LoadGraph(L"./Data/Images/title_frame_2.png");

    perfectSheetID = DxLib::LoadGraph(L"./Data/Images/game_perfect.png");
    notsheetID = DxLib::LoadGraph(L"./Data/Images/game_error.png");

    poti = RM().GetSound(ResourceKeys::SE_poti);
    bubu = RM().GetSound(ResourceKeys::SE_bubu);
    perfect = RM().GetSound(ResourceKeys::SE_PerfectVoice);
    setumeivoice = RM().GetSound(ResourceKeys::SE_GameVoice2);

    Reset();
}

void Game_02::Reset()
{
    change = 0;

    // 指示画像をランダムで切り替え（game_setumei_7: 多いほう / game_setumei_8: 少ないほう）
    if (rand() % 2 == 0)
    {
        instructionType = Game02InstructionType::More;
        currentExplanationSpr = RM().GridAt(ResourceKeys::game_setumei_7);
    }
    else
    {
        instructionType = Game02InstructionType::Less;
        currentExplanationSpr = RM().GridAt(ResourceKeys::game_setumei_8);
    }

    // TODO: leftValue/rightValue に問題ごとの数値（合計や積）を設定してください。
    // 例: 左が 5x3, 右が 3x4 の場合は leftValue[0]=15, rightValue[0]=12 のように設定します。
    // デフォルトでは -1 のままで、既存の leftIsMoreTable が使用されます。
    leftValue[0] = 12; rightValue[0] = 15;
    leftValue[1] = 100; rightValue[1] = 48;
    leftValue[2] = 1.3; rightValue[2] = 2;
    leftValue[3] = 2126; rightValue[3] = 10020;
    leftValue[4] = -100; rightValue[4] = -90;
    leftValue[5] = 0.20833333333; rightValue[5] = 1;
}

void Game_02::Update(int& hp, int& score)
{
    frame = 0;
    using namespace DxPlus::Input;
    int button = GetButtonDown(PLAYER1);
    bool isClicked = (button & BUTTON_TRIGGER2);

    int mouseX = 0, mouseY = 0;
    frame++;

    if (isClicked)
    {
        PlaySoundMem(poti, DX_PLAYTYPE_BACK);
        DxLib::GetMousePoint(&mouseX, &mouseY);

        bool isLeftClicked = (mouseX >= 157 && mouseX <= 157 + 751 && mouseY >= 320 && mouseY <= 320 + 482);
        bool isRightClicked = (mouseX >= 855 && mouseX <= 855 + 751 && mouseY >= 320 && mouseY <= 320 + 482);

        if (isLeftClicked || isRightClicked)
        {
            // 現在の画像で「合計が多いほう」は左か？
            bool leftIsMore = false;
            if ((leftValue[change] >= 0) || (rightValue[change] >= 0))
            {
                int lv = (leftValue[change] >= 0) ? leftValue[change] : 0;
                int rv = (rightValue[change] >= 0) ? rightValue[change] : 0;
                leftIsMore = (lv > rv);
            }
            else
            {
                // フォールバック: 既存のテーブルを使用
                leftIsMore = leftIsMoreTable[change];
            }

            // 今回の指示における「正解」は左かどうか
            // More（多いほう）なら leftIsMore、Less（少ないほう）ならその逆
            bool leftIsCorrect = (instructionType == Game02InstructionType::More) ? leftIsMore : !leftIsMore;

            // プレイヤーが正解を選べたか判定
            bool isCorrect = (isLeftClicked && leftIsCorrect) || (isRightClicked && !leftIsCorrect);

            if (isCorrect)
            {
                int soundIdx = GetRand(2);
                PlaySoundMem(good[soundIdx], DX_PLAYTYPE_BACK);
                score++;
            }
            else
            {
                PlaySoundMem(bubu, DX_PLAYTYPE_BACK);
                hp--;
            }

            Animating = true;

            // 次の問題へ進む
            change++;
            if (change >= 6)
            {
                change = 0;
            }
        }
    }

    if (Animating)
    {
        DrawPerfectAnimation(100, 100);
    }
}

void Game_02::Draw(int hp, int score) const
{
    DrawGraph(157, 320, a[change], TRUE);
}