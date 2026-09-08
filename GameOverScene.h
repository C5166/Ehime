#pragma once
#include "Scene.h"
#include "GameContext.h"

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

    int  touch[5];
    int a;
    int poti;
    int kirakira;

    int voice;

	GameContext gameContext;

};