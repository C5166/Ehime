#pragma once
#include "Scene.h"
#include "GameContext.h"
#include <algorithm>

class GameOverScene final : public Scene
{
public:
    explicit GameOverScene(GameContext* context) : Scene(context) {}
    ~GameOverScene() override = default;

    void Init() override;
    void Update() override;
    void Render() const override;

private:
    void UpdateGameOverLogoAnimation(); // アニメーション計算用
    void DrawGameOverUI() const;

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
    bool isGameOverInput{ false };
    int isGameOverInputCount{ 0 };
    const int isGameOverInputMax{ 10 };

    // --- ロゴアニメーション・描画設定 ---
    DxPlus::Vec2 logoPos{ 960.0f, 740.0f };    // ロゴの表示位置
    DxPlus::Vec2 logoScale{ 1.5f, 1.5f };      // ロゴの拡大率（1.5倍など）
    int animFrameInterval{ 6 };                // 何フレームごとにコマを進めるか（大きいほど遅くなる）
    const int totalLogoFrames{ 26 };           // 総コマ数
    const int logoColumns{ 10 };               // スプライトシートの横コマ数

    // アニメーション制御用変数
    int logoAnimTimer{ 0 };
    int currentLogoIndex{ 0 };

    const DxPlus::Sprite::SpriteBase* backgroundSpr{ nullptr };
    const DxPlus::Sprite::SpriteBase* backgroundSpr2{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameover_background{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameover_character_1{ nullptr };
    const DxPlus::Sprite::SpriteBase* gameover_character_2{ nullptr };

    int bgm;

    int touch[5];
    int a;
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