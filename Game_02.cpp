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
    setumeivoice[0] = RM().GetSound(ResourceKeys::SE_GameVoice2_1);
    setumeivoice[1] = RM().GetSound(ResourceKeys::SE_GameVoice2_2);

}

void Game_02::Reset()
{
    change = 0;
    int ruleIdx = rand() % 2 == 0;
    // 指示画像をランダムで切り替え（game_setumei_7: 多いほう / game_setumei_8: 少ないほう）
    if (ruleIdx == 0)
    {
        instructionType = Game02InstructionType::More;
        currentExplanationSpr = RM().GridAt(ResourceKeys::game_setumei_7);
        PlaySoundMem(setumeivoice[0], DX_PLAYTYPE_BACK);
    }
    else if (ruleIdx == 1)
    {
        instructionType = Game02InstructionType::Less;
        currentExplanationSpr = RM().GridAt(ResourceKeys::game_setumei_8);
        PlaySoundMem(setumeivoice[1], DX_PLAYTYPE_BACK);
    }

    // .h で指定した problems 配列の合計値をそのままセット
    for (int i = 0; i < 6; ++i) {
        leftValue[i] = problems[i].leftValue;
        rightValue[i] = problems[i].rightValue;
    }

    // リセット時に正解カウント等をクリア
    correctCount = 0;
    completed = false;
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
            // 1. 現在の問題の左右の合計値を比較して「左が多いか」を判定
            bool leftIsMore = (leftValue[change] > rightValue[change]);

            // 2. 指示（More: 多いほうを選べ / Less: 少ないほうを選べ）に応じて正解の左右を決定
            bool leftIsCorrect = (instructionType == Game02InstructionType::More) ? leftIsMore : !leftIsMore;

            // 3. プレイヤーがクリックした側と正解が一致しているか判定
            bool isCorrect = (isLeftClicked && leftIsCorrect) || (isRightClicked && !leftIsCorrect);

            if (isCorrect)
            {
                int soundIdx = GetRand(2);
                PlaySoundMem(good[soundIdx], DX_PLAYTYPE_BACK);
                score++;
                correctCount++;
                if (correctCount >= 6)
                {
                    completed = true;
                    // play perfect sound
                    if (perfect >= 0) PlaySoundMem(perfect, DX_PLAYTYPE_BACK);
                }
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