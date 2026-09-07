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
};