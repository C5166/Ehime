#include "Game_03.h"
#include "ResourceKeys.h"
#include "ResourceManager.h"
#include <cstdlib>
#include <cmath>

namespace
{
    // 点 (px, py) が三角形 (p0, p1, p2) の内部にあるか判定する外積計算
    bool IsPointInTriangle(DxPlus::Vec2 p, DxPlus::Vec2 p0, DxPlus::Vec2 p1, DxPlus::Vec2 p2)
    {
        float area = 0.5f * (-p1.y * p2.x + p0.y * (-p1.x + p2.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y);
        float s = 1.0f / (2.0f * area) * (p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y);
        float t = 1.0f / (2.0f * area) * (p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y);
        return s >= 0.0f && t >= 0.0f && (1.0f - s - t) >= 0.0f;
    }

    // ハート形状の当たり判定 (2つの円 + 下部三角形)
    bool IsPointInsideHeart(DxPlus::Vec2 point, DxPlus::Vec2 center, DxPlus::Vec2 size)
    {
        float w = size.x;
        float h = size.y;

        // 1. 左上の円判定
        DxPlus::Vec2 leftCircleCenter = { center.x - w * 0.25f, center.y - h * 0.12f };
        float circleRadius = w * 0.27f;
        float dxL = point.x - leftCircleCenter.x;
        float dyL = point.y - leftCircleCenter.y;
        if (dxL * dxL + dyL * dyL <= circleRadius * circleRadius)
        {
            return true;
        }

        // 2. 右上の円判定
        DxPlus::Vec2 rightCircleCenter = { center.x + w * 0.25f, center.y - h * 0.12f };
        float dxR = point.x - rightCircleCenter.x;
        float dyR = point.y - rightCircleCenter.y;
        if (dxR * dxR + dyR * dyR <= circleRadius * circleRadius)
        {
            return true;
        }

        // 3. 下部の逆三角形判定
        DxPlus::Vec2 tLeft = { center.x - w * 0.48f, center.y - h * 0.05f };
        DxPlus::Vec2 tRight = { center.x + w * 0.48f, center.y - h * 0.05f };
        DxPlus::Vec2 tBottom = { center.x,             center.y + h * 0.48f };

        if (IsPointInTriangle(point, tLeft, tRight, tBottom))
        {
            return true;
        }

        return false;
    }
}

void Game_03::Init()
{
    Herat1ID = RM().GridAt(ResourceKeys::game_1_heart_1);
    Herat2ID = RM().GridAt(ResourceKeys::game_1_heart_2);
    Herat3ID = RM().GridAt(ResourceKeys::game_1_heart_3);

    Reset();
}

void Game_03::Reset()
{
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

        if (i < totalRed)
        {
            ball.type = BallType::Red;
        }
        else if (i < totalRed + totalBlue)
        {
            ball.type = BallType::Blue;
        }
        else
        {
            ball.type = BallType::Green;
        }

        ball.size = { 112.0f, 112.0f };
        ball.active = true;

        float halfW = ball.size.x * 0.5f;
        float halfH = ball.size.y * 0.5f;

        // screenX / screenY を基準座標として生成位置を計算
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
        DxLib::GetMousePoint(&mouseX, &mouseY);
    }

    // 1. 位置更新 ＆ 画面端バウンド
    for (auto& ball : balls)
    {
        if (!ball.active) continue;

        float halfW = ball.size.x * 0.5f;
        float halfH = ball.size.y * 0.5f;

        ball.position.x += ball.velocity.x;
        ball.position.y += ball.velocity.y;

        // 左右の壁バウンド判定（screenX 基準）
        if (ball.position.x - halfW < screenX || ball.position.x + halfW > screenX + screenWidth)
        {
            ball.velocity.x *= -1.0f;
        }
        // 上下の壁バウンド判定（screenY 基準）
        if (ball.position.y - halfH < screenY || ball.position.y + halfH > screenY + screenHeight)
        {
            ball.velocity.y *= -1.0f;
        }
    }

    // 2. クリック判定（手前にある個体のみを1つ選択して消去）
    if (isClicked)
    {
        DxPlus::Vec2 mousePos = { static_cast<float>(mouseX), static_cast<float>(mouseY) };

        // 手前に描画されているもの（配列の後ろ）から優先して判定
        for (int i = static_cast<int>(balls.size()) - 1; i >= 0; --i)
        {
            auto& ball = balls[i];
            if (!ball.active) continue;

            // ハートの形状範囲内をクリックしたか判定
            if (IsPointInsideHeart(mousePos, ball.position, ball.size))
            {
                ball.active = false; // 1つだけ消す

                if (ball.type == BallType::Red)
                {
                    score++;
                }
                else if (ball.type == BallType::Blue || ball.type == BallType::Green)
                {
                    hp--;
                }

                // 1つクリックしたらループを抜けて同時に複数が消えるのを防ぐ
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
        case BallType::Blue:
            Herat1ID->Draw(drawPos);
            break;
        case BallType::Red:
            Herat2ID->Draw(drawPos);
            break;
        case BallType::Green:
            Herat3ID->Draw(drawPos);
            break;
        }

        // --------------------------------------------------
        // デバッグ用：ハート形状の判定可視化
        // --------------------------------------------------

#ifdef _DEBUG


        float w = ball.size.x;
        float h = ball.size.y;
        unsigned int debugColor = DxLib::GetColor(255, 0, 0);

        // 左右の円枠
        DxLib::DrawCircle(static_cast<int>(ball.position.x - w * 0.25f), static_cast<int>(ball.position.y - h * 0.12f), static_cast<int>(w * 0.27f), debugColor, FALSE);
        DxLib::DrawCircle(static_cast<int>(ball.position.x + w * 0.25f), static_cast<int>(ball.position.y - h * 0.12f), static_cast<int>(w * 0.27f), debugColor, FALSE);

        // 下部三角形枠
        int x1 = static_cast<int>(ball.position.x - w * 0.48f);
        int y1 = static_cast<int>(ball.position.y - h * 0.05f);
        int x2 = static_cast<int>(ball.position.x + w * 0.48f);
        int y2 = static_cast<int>(ball.position.y - h * 0.05f);
        int x3 = static_cast<int>(ball.position.x);
        int y3 = static_cast<int>(ball.position.y + h * 0.48f);

        DxLib::DrawTriangle(x1, y1, x2, y2, x3, y3, debugColor, FALSE);
#endif
    }

}