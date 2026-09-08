#pragma once
#include "Scene.h"
#include "GameContext.h"
#include <algorithm>

class GameClearScene final : public Scene
{
public:
    explicit GameClearScene(GameContext* context) : Scene(context) {}
    ~GameClearScene() override = default;

    void Init() override;
    void Update() override;
    void Render() const override;

private:
    void UpdateGameClearLogoAnimation();
    void DrawGameClearUI() const;

    // マウス中心点と円形オブジェクトの当たり判定
    bool CheckMouseCircleCollision(int mouseX, int mouseY, float mouseRadius, float targetX, float targetY, float targetRadius) const
    {
        float dx = static_cast<float>(mouseX) - targetX;
        float dy = static_cast<float>(mouseY) - targetY;
        float distanceSq = dx * dx + dy * dy;
        float radiusSum = mouseRadius + targetRadius;
        return distanceSq <= (radiusSum * radiusSum);
    }

private:
    bool isGameClearInput{ false };
    int isGameClearInputCount{ 0 };
    const int isGameClearInputMax{ 10 };

    // --- ロゴアニメーション・描画設定 ---
    DxPlus::Vec2 logoPos{ 960.0f, 740.0f };
    DxPlus::Vec2 logoScale{ 1.5f, 1.5f };
    int animFrameInterval{ 6 };
    const int totalLogoFrames{ 26 };
    const int logoColumns{ 10 };

    // アニメーション制御用変数
    int logoAnimTimer{ 0 };
    int currentLogoIndex{ 0 };

    const DxPlus::Sprite::SpriteBase* backgroundSpr{ nullptr };
    const DxPlus::Sprite::SpriteBase* backgroundSpr2{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameclear_background{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameclear_character_1{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameclear_character_2{ nullptr };

    int bgm;
    int touch[5];
    int a{ 0 };
    int poti;
    int kirakira;
    int voice;

    GameContext gameContext;

    // --- 当たり判定設定（調整可能パラメータ） ---
    float mouseCollisionRadius = 50.0f;       // マウスの判定半径
    float characterX = 960.0f;                // キャラクター基準位置X
    float characterY = 540.0f;                // キャラクター基準位置Y
    float characterOffsetX = 0.0f;            // 判定円のオフセットX
    float characterOffsetY = 0.0f;            // 判定円のオフセットY
    float characterRadius = 400.0f;           // キャラクターの判定半径
};