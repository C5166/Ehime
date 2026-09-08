// =============================
// Scenes/Title/TitleScene.h
// =============================
#pragma once
#include "Scene.h"
#include "GameContext.h"

class TitleScene final : public Scene
{
public:
    explicit TitleScene(GameContext* context) : Scene(context) {}
    void Init() override;
    void Update() override;
    void Render() const override;
    void Draw() const;

private:
    void UpdateTitleBgAnimation(); // アニメーション更新関数

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
    // 10回カウント
    int TenCount{ 1 };
    int frameCount{ 0 };
    int fontHandle{ -1 };

    bool isTitleInput{ false };
    int isTitleInputCount{ 0 };
    int isTitleInputMax{ 10 };

    // --- タイトル背景アニメーション設定 ---
    DxPlus::Vec2 bgPos{ 960.0f, 1080.0f };         // 位置
    DxPlus::Vec2 bgScale{ 2.5f, 2.5f };       // スケール（サイズ）
    int animFrameInterval{ 2 };               // 何フレーム毎に1コマ進めるか（速度）
    const int totalBgFrames{ 37 };            // 総フレーム数（37コマ）
    const int bgColumns{ 10 };                // スプライトシートの横コマ数（10列）

    // 背景アニメーション制御用変数
    int bgAnimTimer{ 0 };
    int currentBgIndex{ 0 };

    const DxPlus::Sprite::SpriteBase* bgSprite{ nullptr };
    const DxPlus::Sprite::SpriteBase* title_character{ nullptr };
    const DxPlus::Sprite::SpriteBase* title_character2{ nullptr };

    int touch[5];
    int poti;
    int kirakira;
    int a = 0;
    int startvoice;

    // --- 当たり判定設定（調整可能パラメータ） ---
    float mouseCollisionRadius = 50.0f;       // マウスの判定半径
    float characterX = 960.0f;                // キャラクター基準位置X
    float characterY = 540.0f;                // キャラクター基準位置Y
    float characterOffsetX = 45.0f;            // 判定円のオフセットX
    float characterOffsetY = 100.0f;            // 判定円のオフセットY
    float characterRadius = 250.0f;           // キャラクターの判定半径
};