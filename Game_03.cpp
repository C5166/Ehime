#include "Game_03.h"
#include <cstdlib>

void Game_03::Init()
{
    Reset();
}

void Game_03::Reset()
{
    number = 0;
    HP = 3;
    SpawnBalls();
}

void Game_03::SpawnBalls()
{
    balls.clear();

    // 例：赤10個、青10個をランダム生成
    int totalRed = 10;
    int totalBlue = 10;

    for (int i = 0; i < totalRed + totalBlue; ++i)
    {
        Object ball;
        ball.type = (i < totalRed) ? BallType::Red : BallType::Blue;
        ball.size = { 40.0f, 40.0f }; // 直径40（半径20）
        ball.active = true;

        // ランダムな位置 (画面内)
        ball.position.x = static_cast<float>(rand() % static_cast<int>(screenWidth - 100) + 50);
        ball.position.y = static_cast<float>(rand() % static_cast<int>(screenHeight - 100) + 50);

        // ランダムな速度（それぞれ違う速度で動く）
        float vx = static_cast<float>((rand() % 200 - 100) / 20.0f); // -5.0 〜 5.0
        float vy = static_cast<float>((rand() % 200 - 100) / 20.0f);
        if (vx == 0.0f) vx = 2.0f;
        if (vy == 0.0f) vy = 2.0f;
        ball.velocity = { vx, vy };

        balls.push_back(ball);
    }
}

void Game_03::Update()
{
    using namespace DxPlus::Input;
    int button = GetButtonDown(PLAYER1);
    bool isClicked = (button & BUTTON_TRIGGER2);

    int mouseX = 0, mouseY = 0;
    if (isClicked)
    {
        DxLib::GetMousePoint(&mouseX, &mouseY);
    }

    float radius = 20.0f; // 的の半径

    for (auto& ball : balls)
    {
        if (!ball.active) continue;

        // 1. 位置の更新（移動処理）
        ball.position.x += ball.velocity.x;
        ball.position.y += ball.velocity.y;

        // 2. 画面端での跳ね返り処理
        if (ball.position.x - radius < 0 || ball.position.x + radius > screenWidth)
        {
            ball.velocity.x *= -1.0f;
        }
        if (ball.position.y - radius < 0 || ball.position.y + radius > screenHeight)
        {
            ball.velocity.y *= -1.0f;
        }

        // 3. クリック判定（円判定：中心からの距離の2乗で比較）
        if (isClicked)
        {
            float dx = static_cast<float>(mouseX) - ball.position.x;
            float dy = static_cast<float>(mouseY) - ball.position.y;
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                ball.active = false; // 消す

                if (ball.type == BallType::Red)
                {
                    number++; // 赤を選んだらスコア加点
                }
                else if (ball.type == BallType::Blue)
                {
                    HP--;    // 青を選んだらHP減少
                }
            }
        }
    }
}

void Game_03::Draw() const
{
    float radius = 20.0f;

    for (const auto& ball : balls)
    {
        if (!ball.active) continue;

        // 的の種類に応じた色設定（デザイン画像に合わせて円で描画）
        unsigned int color = (ball.type == BallType::Red)
            ? DxLib::GetColor(230, 30, 30)
            : DxLib::GetColor(0, 100, 220);

        DxLib::DrawCircle(
            static_cast<int>(ball.position.x),
            static_cast<int>(ball.position.y),
            static_cast<int>(radius),
            color, TRUE
        );
    }

    // UI表示
    DxLib::DrawFormatString(10, 10, DxLib::GetColor(255, 255, 255), L"Score: %d", number);
    DxLib::DrawFormatString(10, 30, DxLib::GetColor(255, 255, 255), L"HP: %d", HP);
}