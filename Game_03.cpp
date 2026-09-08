#include "Game_03.h"
#include "ResourceKeys.h"
#include "ResourceManager.h"
#include <cstdlib>
#include <cmath>

namespace
{
    bool IsPointInTriangle(DxPlus::Vec2 p, DxPlus::Vec2 p0, DxPlus::Vec2 p1, DxPlus::Vec2 p2)
    {
        float area = 0.5f * (-p1.y * p2.x + p0.y * (-p1.x + p2.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y);
        float s = 1.0f / (2.0f * area) * (p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y);
        float t = 1.0f / (2.0f * area) * (p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y);
        return s >= 0.0f && t >= 0.0f && (1.0f - s - t) >= 0.0f;
    }



    bool IsPointInsideHeart(DxPlus::Vec2 point, DxPlus::Vec2 center, DxPlus::Vec2 size)
    {
        float w = size.x;
        float h = size.y;

        DxPlus::Vec2 leftCircleCenter = { center.x - w * 0.25f, center.y - h * 0.12f };
        float circleRadius = w * 0.27f;
        float dxL = point.x - leftCircleCenter.x;
        float dyL = point.y - leftCircleCenter.y;
        if (dxL * dxL + dyL * dyL <= circleRadius * circleRadius) return true;

        DxPlus::Vec2 rightCircleCenter = { center.x + w * 0.25f, center.y - h * 0.12f };
        float dxR = point.x - rightCircleCenter.x;
        float dyR = point.y - rightCircleCenter.y;
        if (dxR * dxR + dyR * dyR <= circleRadius * circleRadius) return true;

        DxPlus::Vec2 tLeft = { center.x - w * 0.48f, center.y - h * 0.05f };
        DxPlus::Vec2 tRight = { center.x + w * 0.48f, center.y - h * 0.05f };
        DxPlus::Vec2 tBottom = { center.x,             center.y + h * 0.48f };

        return IsPointInTriangle(point, tLeft, tRight, tBottom);
    }
}

void Game_03::Init()
{
    Herat1ID = RM().GridAt(ResourceKeys::game_1_heart_1); // 青
    Herat2ID = RM().GridAt(ResourceKeys::game_1_heart_2); // 赤
    Herat3ID = RM().GridAt(ResourceKeys::game_1_heart_3); // 緑

    // 6種の説明画像を ResourceManager から読み込み（キーは各プロジェクトの定義に合わせてください）
    explanationSprites[0] = RM().GridAt(ResourceKeys::game_setumei_1);
    explanationSprites[1] = RM().GridAt(ResourceKeys::game_setumei_2);
    explanationSprites[2] = RM().GridAt(ResourceKeys::game_setumei_3);
    explanationSprites[3] = RM().GridAt(ResourceKeys::game_setumei_4);
    explanationSprites[4] = RM().GridAt(ResourceKeys::game_setumei_5);
    explanationSprites[5] = RM().GridAt(ResourceKeys::game_setumei_6);

    poti = RM().GetSound(ResourceKeys::SE_poti);
    bubu = RM().GetSound(ResourceKeys::SE_bubu);
    good[0] = RM().GetSound(ResourceKeys::SE_GoodVoice1);
    good[1] = RM().GetSound(ResourceKeys::SE_GoodVoice2);
    good[2] = RM().GetSound(ResourceKeys::SE_GoodVoice3);
    perfect = RM().GetSound(ResourceKeys::SE_PerfectVoice);


    setumeivoice[0] = RM().GetSound(ResourceKeys::SE_GameVoice1_1);
    setumeivoice[1] = RM().GetSound(ResourceKeys::SE_GameVoice1_2);

    //Reset();
}

void Game_03::Reset()
{
    // 0~5 のランダムでルールを決定
    int ruleIdx = rand() % 6;
    currentRule = static_cast<RuleType>(ruleIdx);
    currentExplanationSpr = explanationSprites[ruleIdx];
    if (ruleIdx <= 2) {
        DxLib::PlaySoundMem(setumeivoice[0], DX_PLAYTYPE_BACK);
    }
    else if(ruleIdx >= 2){
        DxLib::PlaySoundMem(setumeivoice[1], DX_PLAYTYPE_BACK);
	}
    SpawnBalls();

}

void Game_03::SpawnBalls()
{
    balls.clear();

    int totalRed = 10;
    int totalBlue = 10;
    int totalGreen = 10;

    for (int i = 0; i < totalRed + totalBlue + totalGreen; ++i)
    {
        Object ball;

        if (i < totalRed) ball.type = BallType::Red;
        else if (i < totalRed + totalBlue) ball.type = BallType::Blue;
        else ball.type = BallType::Green;

        ball.size = { 112.0f, 112.0f };
        ball.active = true;

        float halfW = ball.size.x * 0.5f;
        float halfH = ball.size.y * 0.5f;

        ball.position.x = screenX + static_cast<float>(rand() % static_cast<int>(screenWidth - ball.size.x) + halfW);
        ball.position.y = screenY + static_cast<float>(rand() % static_cast<int>(screenHeight - ball.size.y) + halfH);

        float vx = static_cast<float>((rand() % 200 - 100) / 20.0f);
        float vy = static_cast<float>((rand() % 200 - 100) / 20.0f);
        if (vx == 0.0f) vx = 2.0f;
        if (vy == 0.0f) vy = 2.0f;
        ball.velocity = { vx, vy };

        balls.push_back(ball);
    }
}

void Game_03::Update(int& hp, int& score)
{
    using namespace DxPlus::Input;
    int button = GetButtonDown(PLAYER1);
    bool isClicked = (button & BUTTON_TRIGGER2);

    int mouseX = 0, mouseY = 0;
    if (isClicked)
    {
        PlaySoundMem(poti, DX_PLAYTYPE_BACK);
        DxLib::GetMousePoint(&mouseX, &mouseY);
    }

    // 移動および壁反射処理

    for (auto& ball : balls)
    {
        if (!ball.active) continue;

        float halfW = ball.size.x * 0.5f;
        float halfH = ball.size.y * 0.5f;

        ball.position.x += ball.velocity.x;
        ball.position.y += ball.velocity.y;

        if (ball.position.x - halfW < screenX || ball.position.x + halfW > screenX + screenWidth)
        {
            ball.velocity.x *= -1.0f;
        }
        if (ball.position.y - halfH < screenY || ball.position.y + halfH > screenY + screenHeight)
        {
            ball.velocity.y *= -1.0f;
        }
    }

    // クリック判定
    if (isClicked)
    {
        DxPlus::Vec2 mousePos = { static_cast<float>(mouseX), static_cast<float>(mouseY) };

        for (int i = static_cast<int>(balls.size()) - 1; i >= 0; --i)
        {
            auto& ball = balls[i];
            if (!ball.active) continue;

            if (IsPointInsideHeart(mousePos, ball.position, ball.size))
            {
                ball.active = false;

                // ルールに基づいて正解・不正解を判定
                bool isCorrect = false;

                switch (currentRule)
                {
                case RuleType::PickGreen:  isCorrect = (ball.type == BallType::Green); break;
                case RuleType::PickRed:    isCorrect = (ball.type == BallType::Red); break;
                case RuleType::PickBlue:   isCorrect = (ball.type == BallType::Blue); break;
                case RuleType::AvoidBlue:  isCorrect = (ball.type != BallType::Blue); break;
                case RuleType::AvoidRed:   isCorrect = (ball.type != BallType::Red); break;
                case RuleType::AvoidGreen: isCorrect = (ball.type != BallType::Green); break;
                }

                if (isCorrect)
                {
                    int a = GetRand(2);
                    PlaySoundMem(good[a], DX_PLAYTYPE_BACK);
                    score++;
                }
                else
                {
                    PlaySoundMem(bubu, DX_PLAYTYPE_BACK);
                    hp--;
                }

                break;
            }
        }
    }
}

void Game_03::Draw(int hp, int score) const
{

    for (const auto& ball : balls)
    {
        if (!ball.active) continue;

        DxPlus::Vec2 drawPos = ball.position;

        switch (ball.type)
        {
        case BallType::Blue:  Herat1ID->Draw(drawPos); break;
        case BallType::Red:   Herat2ID->Draw(drawPos); break;
        case BallType::Green: Herat3ID->Draw(drawPos); break;
        }
    }
}